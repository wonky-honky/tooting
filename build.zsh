#!/usr/bin/env zsh
cmake -GNinja -S . -B build -DCMAKE_BUILD_TYPE=Debug -DTOOTING_WARNING_AS_ERROR=OFF
cmake --build build --parallel 8
