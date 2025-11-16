#!/bin/bash

rm -rf obj_dir
rm -f cu_wave.vcd

verilator -Wall --trace \
    -cc cu.sv \
    --exe verify_cu.cpp \
    --prefix "Vcu" \
    -o Vcu \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

make -j -C obj_dir -f Vcu.mk

./obj_dir/Vcu
