#!/bin/bash

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace \
    -cc regalu.sv alu.sv regfile.sv mux.sv \
    --exe verify_vbuddy.cpp \
    --prefix "Vregalu" \
    -o Vregalu_vbuddy \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lpthread"

make -j -C obj_dir -f Vregalu.mk

./obj_dir/Vregalu_vbuddy
