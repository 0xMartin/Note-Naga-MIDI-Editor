rm -rf build

cmake -S . -B build

cp FluidR3_GM.sf2 build/FluidR3_GM.sf2

cd build

make -j8