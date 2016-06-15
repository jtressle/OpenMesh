#!/bin/bash

#Exit on any error
set -e 

LANGUAGE=$1
TYPE="Release"

source CI/mac-settings.sh


echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Building Debug version with vectorchecks disabled"
echo "======================================================================"
echo -e "${NC}"

if [ ! -d build-debug-$BUILDPATH ]; then
  mkdir build-debug-$BUILDPATH
fi

cd build-debug-$BUILDPATH

cmake -DCMAKE_BUILD_TYPE=DEBUG -DOPENMESH_BUILD_PYTHON_UNIT_TESTS=ON -DBUILD_APPS=OFF $OPTIONS ../

#build it
make

cd ..


echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Building Release version with vectorchecks disabled"
echo "======================================================================"
echo -e "${NC}"

if [ ! -d build-release-$BUILDPATH ]; then
  mkdir build-release-$BUILDPATH
fi

cd build-release-$BUILDPATH

cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_APPS=OFF -DCPACK_BINARY_DRAGNDROP=ON $OPTIONS ../

#build it
make

cd ..



echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Package creation (DMG and tarball)"
echo "======================================================================"
echo -e "${NC}"

cd build-release-$BUILDPATH
cp ../build-debug-$BUILDPATH/Build/lib/* ./Build/lib/
cmake .
make package
