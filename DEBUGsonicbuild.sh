rm -rf build
mkdir build
cd build
cmake -DDEFINE_DEBUG=ON ..
make -j64
cd ..

cp build/vita_cord.vpk ./vita_cord_DEBUG.vpk


echo ""
echo "End of my buildscript!"


