rm -rf build
mkdir build && cd build
cmake -S ../ -B . -DCMAKE_CXX_COMPILER=clang++ -Dtest=ON
cmake --build .
