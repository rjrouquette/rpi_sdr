#!/bin/bash

cd "$(dirname "${BASH_SOURCE[0]}")"

mkdir -p dist/
mkdir -p build/
rm dist/*
cd build

CORES=$(nproc)
echo "Running build with $CORES threads."

rm -rf *
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
#make VERBOSE=1
make -j$CORES
mv status_cli ../dist

rm -rf *
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
#make VERBOSE=1
make -j$CORES
mv status_cli ../dist/status_cli-dbg

cd ..
rm -rf build

if [ ! -f dist/status_cli ]; then
    echo "Failed to build status_cli"
    exit 1
fi

if [ ! -f dist/status_cli-dbg ]; then
    echo "Failed to build status_cli-dbg"
    exit 1
fi

echo "Successfully built: status_cli status_cli-dbg"
exit 0
