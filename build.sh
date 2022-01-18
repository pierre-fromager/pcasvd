#!/bin/bash

if [ ! -d "./build" ]; then
    mkdir -p ./build
fi

cd build
cmake configure ..
cmake generate ..
make -j 2
cd ..
