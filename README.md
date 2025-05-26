# Godot Slang

Integration of the Slang shader language into Godot.

**WORK IN PROGRESS!**

## How to build for macOS

1. Clone repository recursively
2. Build godot-cpp according to docs
3. Build Slang like this:
```
# Build the generators
cmake --workflow --preset generators --fresh
mkdir build-platform-generators
cmake --install build --config Release --prefix ../build-platform-generators --component generators

# Set up compiling environment
cmake \
  --preset default \
  --fresh \
  -DSLANG_LIB_TYPE=STATIC \
  -DSLANG_GENERATORS_PATH=../build-platform-generators/bin \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
  -Dwhatever-other-necessary-options-for-your-cross-build \
  # for example \
  -DCMAKE_C_COMPILER=my-arch-gcc \
  -DCMAKE_CXX_COMPILER=my-arch-g++

# Build slang library
cmake --build --preset release --target slang
```
