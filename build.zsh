#!/usr/bin/env zsh
cmake -GNinja -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 8
