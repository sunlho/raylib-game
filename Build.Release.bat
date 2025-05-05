RMDIR /Q /S build

conan install . --build=missing
cmake --preset conan-default
cd build
cmake --build . --config Release
Release\RaylibGame.exe