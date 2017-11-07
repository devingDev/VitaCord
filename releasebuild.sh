rm -rf build
mkdir build
cd build
cmake ..
make
cd ..

cp build/vita_cord.vpk ./


echo ""
echo "End of my buildscript!"


