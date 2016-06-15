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
  echo "Running unittests Release version with vectorchecks enabled"
  echo "======================================================================"
  echo -e "${NC}"
  
  
  if [ ! -d build-release-$BUILDPATH-Vector-Checks ]; then
    echo "Unable to find artifacts in  build-release-$BUILDPATH-Vector-Checks "
    exit 1
  fi
  
  cd build-release-$BUILDPATH-Vector-Checks

  cd Unittests

  #execute tests
  export LD_LIBRARY_PATH=../Build/lib
  ./unittests --gtest_color=yes --gtest_output=xml

  cd ..
  cd ..
  

  echo -e "${OUTPUT}"
  echo ""
  echo "======================================================================"
  echo "Running Python unittests Release version "
  echo "======================================================================"
  echo -e "${NC}"

  if [ ! -d build-release-$BUILDPATH ]; then
    echo "Unable to find artifacts in build-release-$BUILDPATH "
    exit 1
  fi

  cd build-release-$BUILDPATH

 
  if [ "$LANGUAGE" == "C++11" ]; then

    # Execute Python unittests
    cd Python-Unittests

    rm -f openmesh.so
    cp ../Build/python/openmesh.so .
    rm -f libOpenMesh*
    cp ../Build/lib/lib* .
    export LD_LIBRARY_PATH=../Build/lib:./:../Build/python/
    python -m unittest discover -v

    cd ..

  else
    echo -e "${WARNING}"
    echo "WARNING! Python unittests disabled for clang on Mac with c++98 !!"
    echo -e "${NC}"
  fi
 

  cd ..
fi

if [ $TYPE = "Debug" ]; then

  echo -e "${OUTPUT}"
  echo ""
  echo "======================================================================"
  echo "Running unittests Debug version with vectorchecks enabled"
  echo "======================================================================"
  echo -e "${NC}"
  
  
  if [ ! -d build-debug-$BUILDPATH-Vector-Checks ]; then
    echo "Unable to find artifacts in  build-debug-$BUILDPATH-Vector-Checks "
    exit 1
  fi
  
  cd build-debug-$BUILDPATH-Vector-Checks

  cd Unittests

  #execute tests
  export LD_LIBRARY_PATH=../Build/lib
  ./unittests --gtest_color=yes --gtest_output=xml

  cd ..
  cd ..
  

  echo -e "${OUTPUT}"
  echo ""
  echo "======================================================================"
  echo "Running Python unittests Debug version "
  echo "======================================================================"
  echo -e "${NC}"

  if [ ! -d build-debug-$BUILDPATH ]; then
    echo "Unable to find artifacts in build-debug-$BUILDPATH "
    exit 1
  fi

  cd build-debug-$BUILDPATH

 
  if [ "$LANGUAGE" == "C++11" ]; then

    # Execute Python unittests
    cd Python-Unittests

    rm -f openmesh.so
    cp ../Build/python/openmesh.so .
    rm -f libOpenMesh*
    cp ../Build/lib/lib* .
    export LD_LIBRARY_PATH=../Build/lib
    python -m unittest discover -v

    cd ..

  else
    echo -e "${WARNING}"
    echo "WARNING! Python unittests disabled for clang on Mac with c++98 !!"
    echo -e "${NC}"
  fi
 

  cd ..

fi



