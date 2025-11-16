#!/bin/bash

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace \
    -cc signextend.sv \
    --exe verify_signextend.cpp \
    --prefix Vsignextend \
    -o Vsignextend \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

make -j -C obj_dir -f Vsignextend.mk

./obj_dir/Vsignextend

