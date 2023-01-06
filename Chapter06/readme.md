# Benchmark library with Visual Studio

[Get started with vcpkg](https://vcpkg.io/en/getting-started.html)

## Install vcpkg

Step 1: Clone the vcpkg repo

```
git clone https://github.com/Microsoft/vcpkg.git
```

Step 2: Run the bootstrap script to build vcpkg

```
.\vcpkg\bootstrap-vcpkg.bat
```

## Install library for your project

```
vcpkg install benchmark --triplet x64-windows
```

## Using vcpkg with MSBuild / Visual Studio (may require elevation)

```
vcpkg integrate install
```

## Using vcpkg with CMake

```
cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build [build directory]
```
