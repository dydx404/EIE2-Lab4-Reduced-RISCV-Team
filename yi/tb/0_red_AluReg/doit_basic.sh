#!/bin/bash

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Verilate the entire RED block subsystem
verilator -Wall --trace \
    -cc regalu.sv alu.sv regfile.sv mux.sv \
    --exe verify_basic.cpp \
    --prefix "Vregalu" \
    -o Vregalu_basic \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

# build
make -j -C obj_dir -f Vregalu.mk

# run executable
./obj_dir/Vregalu_basic
