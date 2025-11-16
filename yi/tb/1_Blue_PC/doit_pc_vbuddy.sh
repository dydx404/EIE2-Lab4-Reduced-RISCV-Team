#!/bin/bash

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --trace \
    -cc pc.sv pcreg.sv mux.sv \
    --exe verify_pc_vbuddy.cpp \
    --prefix "Vpc" \
    -o Vpc_vbuddy \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lpthread"

make -j -C obj_dir -f Vpc.mk

./obj_dir/Vpc_vbuddy
