#!/bin/bash

# ---------------------------------------------------------
# 1. Verilate & generate model
# ---------------------------------------------------------
verilator -Wall --Wno-fatal --trace \
    --cc cu_signextend_top.sv instruction.sv CU.sv signextend.sv \
    --exe ../verify_green.cpp

# ---------------------------------------------------------
# 2. Build verilated model
# ---------------------------------------------------------
make -C obj_dir -f Vcu_signextend_top.mk

# ---------------------------------------------------------
# 3. Build + link GoogleTest
# ---------------------------------------------------------
g++ -std=gnu++17 \
    obj_dir/Vcu_signextend_top__ALL.o \
    obj_dir/verilated.o \
    obj_dir/verilated_vcd_c.o \
    obj_dir/verify_green.o \
    -lgtest -lgtest_main -lpthread \
    -o obj_dir/Vgreen

# ---------------------------------------------------------
# 4. Run the test suite
# ---------------------------------------------------------
./obj_dir/Vgreen
