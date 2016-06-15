#!/bin/bash

#Exit on any error
set -e 

LANGUAGE=$1
TYPE="Release"

source CI/mac-settings.sh


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



