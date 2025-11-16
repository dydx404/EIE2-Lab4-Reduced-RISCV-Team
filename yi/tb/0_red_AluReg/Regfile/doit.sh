#!/bin/bash

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Verilate + compile
verilator -Wall --trace \
    -cc regfile.sv \
    --exe verify.cpp \
    --prefix "Vregfile" \
    -o Vregfile \
    -CFLAGS "-std=c++17 -isystem /opt/homebrew/include" \
    -LDFLAGS "-L/opt/homebrew/lib -lgtest -lgtest_main -lpthread"

# Build using automatically generated makefile
make -j -C obj_dir -f Vregfile.mk

# run executable
./obj_dir/Vregfile
