# Prerequisites
- MSVC 2019+
- Clang 7.0+
- GCC 8.3+
- Perl (for mbedtls subproject)
- Ninja build

# Building
Install Gtest library (you can use [vcpkg](https://github.com/microsoft/vcpkg/) or [conan](https://github.com/conan-io/conan) package managers)

###### Windows
```
cmake -G "Visual Studio 16" -DCMAKE_GENERATOR_PLATFORM=x64 ..
```

###### Linux/Mac
```
cd signature && mkdir build && cd build
cmake -G "Ninja" ..
ninja
```

# Memory usage
Memory usage - 64mb (see HashCalc::max_buffer_size)
