#!/bin/bash
if [ ! -f "./vendor/lib/libsla.a" ]; then
    git submodule init
    git submodule update
    cd deps/sleigh
    cmake -B build -S .
    cmake --build build --parallel 8
    cmake --install build --prefix ./../../vendor/
fi