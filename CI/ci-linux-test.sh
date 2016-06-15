#!/bin/bash

COMPILER=$1
LANGUAGE=$2
TYPE=$3

# Exit script on any error
set -e 

source CI/linux-settings.sh

if [ $TYPE = "Release" ]; then
  echo -e "${OUTPUT}"
  echo ""
  echo "======================================================================"
  echo "Running unittests Release version with vectorchecks enabled"
  echo "======================================================================"
  echo -e "${NC}"

  if [ ! -d build-release-$BUILDPATH-Vector-Checks ]; then
    echo "Build Artifacts not found in: build-release-$BUILDPATH-Vector-Checks"
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
    echo "Build Artifacts not found in: build-release-$BUILDPATH"
    exit 1
  fi

  cd build-release-$BUILDPATH

  if [ "$LANGUAGE" == "C++11"  ] || [ "$COMPILER" == "gcc" ] ; then

    # Execute Python unittests
    cd Python-Unittests

    python -m unittest discover -v

    cd ..

  else
    echo -e "${WARNING}"
    echo "WARNING! Python unittests disabled !!"
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
    echo "Build Artifacts not found in: build-debug-$BUILDPATH-Vector-Checks"
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
    echo "Build Artifacts not found in: build-debug-$BUILDPATH"
    exit 1
  fi

  cd build-debug-$BUILDPATH

  if [ "$LANGUAGE" == "C++11"  ] || [ "$COMPILER" == "gcc" ] ; then

    # Execute Python unittests
    cd Python-Unittests

    python -m unittest discover -v
  else

    echo -e "${WARNING}"
    echo "WARNING! Python unittests disabled !!"
    echo -e "${NC}"

  fi
fi  
