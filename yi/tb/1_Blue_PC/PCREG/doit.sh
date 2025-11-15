#!/bin/bash

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Run Verilator: compile SystemVerilog + C++ testbench
verilator -Wall --trace \
    -cc pcreg.sv \
    --exe verify.cpp \
    --prefix "Vpcreg" \
    -o Vpcreg \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

# Build using generated makefile
make -j -C obj_dir -f Vpcreg.mk

# Run simulation
./obj_dir/Vpcreg
