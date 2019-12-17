# Build

## Build with cmake

`mkdir build`
`cd build`
`PKG_CONFIG_PATH=<path to your libbitcoin pkgconfig files> cmake ../`
`cmake --build .`

## Run

Stay in the build directory from above

`./bm` or `./bm --benchmark-reptitions=N`

## Requirements

1. The libraries being benchmarked are installed in a directory
   pointed to by `PKG_CONFIG_PATH`
2. Google benchmark is already installed somewhere in the system's
   pkgconfig path

---

## Benchmark script verification

In directory `src/script/`

Build target called as `bm-script-verify`

1. `verify_script_benchmark.cpp` benchmarks the CPU time taken to
   verify a P2WPKH spending transaction with only one input.

---

## Benchmark libbitcoin-database

### Inputs

1. Have a source of say first N blocks, including all the transactions.
2. Skip validation, but instead, call the various `store` API to store
   the above input data.
3. Once stored, have another fixed random read queries run for a few
   seconds.
4. The read queries should look for blocks, tx, inputs and outputs.

### Measure

1. Time required to store
2. Time required to read various artifacts.

### Performance measurements

Once benchmark can be used to run the above measurements, the next
goal would be to use `perf` to find problem areas to work on.
