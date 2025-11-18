#!/bin/bash

# Exit on error
set -e

# Clean old build
rm -rf obj_dir

verilator -Wall --Wno-UNUSED --cc --trace \
    top.sv pc.sv pcreg.sv instruction.sv \
    regfile.sv alu.sv regalu.sv signextend.sv CU.sv mux.sv \
    --exe verify.cpp

# Build C++ executable
make -C obj_dir -f Vtop.mk

# Run simulation
./obj_dir/Vtop
