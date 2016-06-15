#!/bin/bash

COMPILER=$1
LANGUAGE=$2


# Exit script on any error
set -e 

source linux-settings.sh


echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Building Release version with vectorchecks enabled"
echo "======================================================================"
echo -e "${NC}"


if [ ! -d build-release-$BUILDPATH-Vector-Checks ]; then
  mkdir build-release-$BUILDPATH-Vector-Checks
fi

cd build-release-$BUILDPATH-Vector-Checks

cmake -DCMAKE_BUILD_TYPE=Release -DOPENMESH_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON -DOPENMESH_BUILD_PYTHON_BINDINGS=OFF $OPTIONS ../

#build it
make $MAKE_OPTIONS

#build the unit tests
make  $MAKE_OPTIONS unittests

cd ..


echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Building Release version with vectorchecks disabled for python tests"
echo "======================================================================"
echo -e "${NC}"

if [ ! -d build-release-$BUILDPATH ]; then
  mkdir build-release-$BUILDPATH
fi

cd build-release-$BUILDPATH

cmake -DCMAKE_BUILD_TYPE=Release -DOPENMESH_BUILD_PYTHON_UNIT_TESTS=ON -DBUILD_APPS=OFF $OPTIONS ../

#build it
make $MAKE_OPTIONS

cd ..


echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Building Debug version with vectorchecks enabled"
echo "======================================================================"
echo -e "${NC}"


if [ ! -d build-debug-$BUILDPATH-Vector-Checks ]; then
  mkdir build-debug-$BUILDPATH-Vector-Checks
fi

cd build-debug-$BUILDPATH-Vector-Checks

cmake -DCMAKE_BUILD_TYPE=Debug -DOPENMESH_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON -DOPENMESH_BUILD_PYTHON_BINDINGS=OFF $OPTIONS ../

#build it
make $MAKE_OPTIONS

#build the unit tests
make  $MAKE_OPTIONS unittests

cd ..


echo -e "${OUTPUT}"
echo ""
echo "======================================================================"
echo "Building Debug version with vectorchecks disabled for python tests"
echo "======================================================================"
echo -e "${NC}"

if [ ! -d build-debug-$BUILDPATH ]; then
  mkdir build-debug-$BUILDPATH
fi

cd build-debug-$BUILDPATH

cmake -DCMAKE_BUILD_TYPE=DEBUG -DOPENMESH_BUILD_PYTHON_UNIT_TESTS=ON -DBUILD_APPS=OFF $OPTIONS ../

#build it
make $MAKE_OPTIONS

cd ..

