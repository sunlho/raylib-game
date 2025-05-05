RMDIR /Q /S build

conan install . --build=missing -s build_type=Debug
cmake --preset conan-default
cd build
cmake --build . --config Debug
