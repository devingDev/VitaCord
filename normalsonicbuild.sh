rm -rf build
mkdir build
cd build
cmake ..
make -j64
cd ..

cp build/vita_cord.vpk ./


echo ""
echo "End of my buildscript!"


