#include "gtest/gtest.h"
#include "Vcu.h"               // Verilated model of the Control Unit
#include "verilated.h"
#include "verilated_vcd_c.h"

// Global DUT and tracer
Vcu *top;
VerilatedVcdC *tfp;
unsigned long tickcount = 0;

// For a purely combinational DUT, tick() just forces
// propagation and dumps the waveform.
void tick() {
    top->eval();
    tfp->dump(tickcount++);
    top->eval();
    tfp->dump(tickcount++);
}

// Helper: construct a 32-bit RISC-V instruction
uint32_t make_addi(int rd, int rs1, int imm12) {
    uint32_t opcode = 0b0010011;
    uint32_t funct3 = 0b000;
    imm12 &= 0xFFF;    // sign-extend later happens in immediate unit

    return (imm12 << 20) |
           (rs1  << 15) |
           (funct3 << 12) |
           (rd << 7) |
           opcode;
}

uint32_t make_bne(int rs1, int rs2, int imm13) {
    // imm13 is raw immediate (signed). Hardware extracts B-type bits itself.
    uint32_t opcode = 0b1100011;
    uint32_t funct3 = 0b001; // BNE

    uint32_t imm = imm13 & 0x1FFF;

    // B-type immediate split into several fields:
    uint32_t imm_12   = (imm >> 12) & 0x1;
    uint32_t imm_10_5 = (imm >> 5)  & 0x3F;
    uint32_t imm_4_1  = (imm >> 1)  & 0xF;
    uint32_t imm_11   = (imm >> 11) & 0x1;

    return (imm_12   << 31) |
           (imm_10_5 << 25) |
           (rs2      << 20) |
           (rs1      << 15) |
           (funct3   << 12) |
           (imm_4_1  << 8)  |
           (imm_11   << 7)  |
           opcode;
}

// ---------------- FIXTURE ----------------

class CUTest : public ::testing::Test {
protected:
    void SetUp() override {
        top->instr = 0;
        top->eq    = 0;
        tick();
    }

    void TearDown() override { }
};


// --------------- TEST CASES ---------------

// Check that default values are stable when opcode is unknown.
TEST_F(CUTest, DefaultState) {
    top->instr = 0xFFFFFFFF;  // illegal opcode
    top->eq    = 0;
    tick();

    EXPECT_EQ(top->RegWrite, 0);
    EXPECT_EQ(top->ALUSrc,   0);
    EXPECT_EQ(top->ImmSrc,   0);
    EXPECT_EQ(top->ALUCtrl,  0);
    EXPECT_EQ(top->PCSrc,    0);
}


// ADDI test: verify full control signal pattern.
TEST_F(CUTest, AddiBasic) {
    top->instr = make_addi(6, 6, 1); // addi x6,x6,1
    top->eq = 0;
    tick();

    EXPECT_EQ(top->RegWrite, 1); // rd must be written
    EXPECT_EQ(top->ALUSrc,   1); // ALU operand from imm
    EXPECT_EQ(top->ImmSrc,   0); // I-type immediate
    EXPECT_EQ(top->ALUCtrl,  0); // ADD
    EXPECT_EQ(top->PCSrc,    0); // sequential PC
}


// BNE test when equal → do NOT branch
TEST_F(CUTest, BneEqual_NoBranch) {
    top->instr = make_bne(5, 3, 16); // dummy offset
    top->eq = 1;                     // rs1 == rs2
    tick();

    EXPECT_EQ(top->RegWrite, 0);
    EXPECT_EQ(top->ALUSrc,   0);
    EXPECT_EQ(top->ImmSrc,   1);
    EXPECT_EQ(top->ALUCtrl,  1); // SUB for comparison
    EXPECT_EQ(top->PCSrc,    0); // ~eq = 0 → no branch
}


// BNE test when not equal → BRANCH
TEST_F(CUTest, BneNotEqual_TakesBranch) {
    top->instr = make_bne(5, 3, -8); // backward jump
    top->eq = 0;
    tick();

    EXPECT_EQ(top->PCSrc, 1); // ~eq = 1 → branch taken
}


// Fuzz addi instructions with random immediates
TEST_F(CUTest, AddiRandomImmediates) {
    for (int i = 0; i < 50; i++) {
        int imm = (rand() % 4096) - 2048; // signed 12-bit
        top->instr = make_addi(3, 2, imm);
        top->eq = rand() & 1;
        tick();

        EXPECT_EQ(top->RegWrite, 1);
        EXPECT_EQ(top->ALUSrc,   1);
        EXPECT_EQ(top->ImmSrc,   0);
        EXPECT_EQ(top->ALUCtrl,  0);
        EXPECT_EQ(top->PCSrc,    0);
    }
}


// Fuzz BNE instructions with random EQ values
TEST_F(CUTest, BneRandom) {
    for (int i = 0; i < 50; i++) {
        int imm = (rand() % 4096) - 2048;
        top->instr = make_bne(1, 2, imm);

        int e = rand() & 1;
        top->eq = e;

        tick();

        EXPECT_EQ(top->RegWrite, 0);
        EXPECT_EQ(top->ALUSrc,   0);
        EXPECT_EQ(top->ImmSrc,   1);
        EXPECT_EQ(top->ALUCtrl,  1);

        EXPECT_EQ(top->PCSrc, (e ? 0 : 1)); // bne logic
    }
}


// ------------------ MAIN ------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    top = new Vcu;
    tfp = new VerilatedVcdC;

    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("cu_wave.vcd");

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    top->final();
    tfp->close();
    delete top;
    delete tfp;
    return result;
}
