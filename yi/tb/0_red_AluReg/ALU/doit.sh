#!/bin/bash

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Translate Verilog -> C++ including testbench
verilator -Wall --trace \
    -cc alu.sv \
    --exe verify.cpp \
    --prefix "Valu" \
    -o Valu \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

# Build C++ project with automatically generated Makefile
make -j -C obj_dir -f Valu.mk

# Run executable simulation file
./obj_dir/Valu
