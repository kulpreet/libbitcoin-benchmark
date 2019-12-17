/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>

#include <benchmark/benchmark.h>

#include "libbitcoin_benchmark.hpp"
#include <bitcoin/system.hpp>

using namespace libbitcoin;
using namespace system;
using namespace machine;
using namespace wallet;
using namespace chain;

class script_accessor
  : public script {
public:
    static code verify(const transaction& tx, uint32_t input_index,
        uint32_t forks, const script& prevout_script, uint64_t value) {
        return script::verify(tx, input_index, forks, prevout_script, value);
    }
};

static void BM_BeginVerifyScript(benchmark::State& state) {

// Testnet wallets.
    auto my_secret0 = base16_literal("b7423c94ab99d3295c1af7e7bbea47c75d298f7190ca2077b53bae61299b70a5");
    ec_private my_private0(my_secret0, ec_private::testnet, true);
    ec_compressed pubkey0 = my_private0.to_public().point();

// Witness aware testnet wallets.
// ("Witness aware" is an arbitrary assignment for illustration purposes)
    auto my_secret_witness_aware = base16_literal("0a44957babaa5fd46c0d921b236c50b1369519c7032df7906a18a31bb905cfdf");
    ec_private my_private_witness_aware(my_secret_witness_aware,
        ec_private::testnet, true);
    ec_compressed pubkey_witness_aware = my_private_witness_aware
        .to_public().point();

//**************************************************************
  // SEND TO P2WPKH.
  //**************************************************************

  // Output Script.
  // 	  0 [20-byte hash160(public key)]
  // Input Script.
  //    According to previous output.
  // Script Code.
  // 	  According to previous output.

  // P2PKH Input.
  // Previous TX hash.
  std::string prev_tx =
      "44101b50393d01de1e113b17eb07e8a09fbf6334e2012575bc97da227958a7a5";
  hash_digest prev_tx_hash;
  decode_hash(prev_tx_hash, prev_tx);
  // Previous UXTO index.
  uint32_t index = 0;
  output_point uxto_to_spend(prev_tx_hash, index);
  // Build input0 object.
  input input0;
  input0.set_previous_output(uxto_to_spend);
  input0.set_sequence(max_input_sequence);
  // Previous embedded script.
  script prev_p2pkh_output_script = script::to_pay_key_hash_pattern(bitcoin_short_hash(pubkey0));

  // P2WPKH output script.
  // 0 [20-byte hash160(public key)]
  operation::list p2wpkh_operations;
  p2wpkh_operations.push_back(operation(opcode::push_size_0));
  p2wpkh_operations.push_back(operation(to_chunk(bitcoin_short_hash(pubkey_witness_aware))));
  // Build P2WPKH output.
  std::string btc_amount = "0.995";
  uint64_t output_amount;
  decode_base10(output_amount, btc_amount, btc_decimal_places);
  output p2wpkh_output(output_amount, p2wpkh_operations);

  // Build & Sign Transaction.
  transaction tx;
  tx.set_version(1u);
  tx.inputs().push_back(input0);
  tx.outputs().push_back(p2wpkh_output);

  // Signature of input 0.
  endorsement sig_0;
  uint8_t input0_index(0u);
  script::create_endorsement(sig_0, my_secret0, prev_p2pkh_output_script, tx, input0_index, sighash_algorithm::all);

  // Input Script.
  operation::list p2pkh_operations;
  p2pkh_operations.push_back(operation(sig_0));
  p2pkh_operations.push_back(operation(to_chunk(pubkey0)));
  script p2pkh_input_script(p2pkh_operations);

  // Complete Transaction.
  tx.inputs()[0].set_script(p2pkh_input_script);

  for (auto _ : state) {
      script_accessor::verify(tx, input0_index, rule_fork::bip141_rule | rule_fork::bip143_rule, prev_p2pkh_output_script, output_amount);
    }
}

// Register the function as a benchmark
BENCHMARK(BM_BeginVerifyScript);

BENCHMARK_MAIN();
