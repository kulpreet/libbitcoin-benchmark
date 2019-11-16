# Build with cmake

`mkdir build`
`cd build`
`PKG_CONFIG_PATH=<path to your libbitcoin pkgconfig files> cmake ../`
`cmake --build .`

# Run

Stay in the build directory from above

`./bm`

# Requirements

1. The libraries being benchmarked are installed in a directory
   pointed to by `PKG_CONFIG_PATH`
2. Google benchmark is already installed somewhere in the system's
   pkgconfig path
