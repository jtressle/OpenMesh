#!/bin/bash

#Exit on any error
set -e 

LANGUAGE=$1
TYPE=$2

source CI/mac-settings.sh


if [ $TYPE = "Release" ]; then
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

  cmake -DCMAKE_BUILD_TYPE=Release -DOPENMESH_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON -DOPENMESH_BUILD_PYTHON_UNIT_TESTS=OFF $OPTIONS ../

  #build it
  make

  #build the unit tests
  make unittests
  
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

  cmake -DCMAKE_BUILD_TYPE=Release -DOPENMESH_BUILD_PYTHON_UNIT_TESTS=ON -DBUILD_APPS=OFF -DCPACK_BINARY_DRAGNDROP=ON $OPTIONS ../

  #build it
  make

  cd ..
fi

if [ $TYPE = "Debug" ]; then

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

  cmake -DCMAKE_BUILD_TYPE=Debug -DOPENMESH_BUILD_UNIT_TESTS=TRUE -DSTL_VECTOR_CHECKS=ON -DOPENMESH_BUILD_PYTHON_UNIT_TESTS=OFF $OPTIONS ../

  #build it
  make

  #build the unit tests
  make unittests

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
  make
  
  cd ..

fi



