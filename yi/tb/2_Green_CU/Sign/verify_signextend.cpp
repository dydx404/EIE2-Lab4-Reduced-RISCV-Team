#include "gtest/gtest.h"
#include "Vsignextend.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

Vsignextend *top;
VerilatedVcdC *tfp;
unsigned long tickcount = 0;

// Simple combinational tick helper
void tick() {
    top->eval();
    tfp->dump(tickcount++);
    top->eval();
    tfp->dump(tickcount++);
}

// Build I-type (addi) instruction
uint32_t make_addi(int rd, int rs1, int imm12) {
    uint32_t opcode = 0b0010011;
    uint32_t funct3 = 0;

    imm12 &= 0xFFF;  // 12-bit mask

    return (imm12 << 20) |
           (rs1   << 15) |
           (funct3 << 12) |
           (rd    << 7) |
           opcode;
}

// Build B-type (bne) instruction
uint32_t make_bne(int rs1, int rs2, int imm13) {
    uint32_t opcode = 0b1100011;
    uint32_t funct3 = 0b001;

    uint32_t imm = imm13 & 0x1FFF; // enforce 13 bits

    uint32_t imm12    = (imm >> 12) & 1;
    uint32_t imm11    = (imm >> 11) & 1;
    uint32_t imm10_5  = (imm >> 5)  & 0x3F;
    uint32_t imm4_1   = (imm >> 1)  & 0xF;

    return (imm12   << 31) |
           (imm10_5 << 25) |
           (rs2     << 20) |
           (rs1     << 15) |
           (funct3  << 12) |
           (imm4_1  << 8)  |
           (imm11   << 7)  |
           opcode;
}


// ---------------------- FIXTURE ----------------------
class SignExtendTest : public ::testing::Test {
protected:
    void SetUp() override {
        top->instr  = 0;
        top->ImmSrc = 0;
        tick();
    }
};


// ---------------------- TESTS ----------------------

// I-type: positive number
TEST_F(SignExtendTest, ITypePositive) {
    top->instr  = make_addi(3, 2, 7);
    top->ImmSrc = 0;
    tick();
    EXPECT_EQ((int32_t)top->Immout, 7);
}

// I-type: negative number
TEST_F(SignExtendTest, ITypeNegative) {
    top->instr  = make_addi(3, 2, -4);
    top->ImmSrc = 0;
    tick();
    EXPECT_EQ((int32_t)top->Immout, -4);
}

// B-type: forward branch
TEST_F(SignExtendTest, BTypeForward) {
    top->instr  = make_bne(1, 2, 16);
    top->ImmSrc = 1;
    tick();
    EXPECT_EQ((int32_t)top->Immout, 16);
}

// B-type: backward branch
TEST_F(SignExtendTest, BTypeBackward) {
    top->instr  = make_bne(1, 2, -8);
    top->ImmSrc = 1;
    tick();
    EXPECT_EQ((int32_t)top->Immout, -8);
}

// Fuzz I-type
TEST_F(SignExtendTest, ITypeRandom) {
    for (int i = 0; i < 50; i++) {
        int imm = (rand() % 4096) - 2048;
        top->instr  = make_addi(5, 1, imm);
        top->ImmSrc = 0;
        tick();
        EXPECT_EQ((int32_t)top->Immout, (int32_t)imm);
    }
}

// Fuzz B-type
TEST_F(SignExtendTest, BTypeRandom) {
    for (int i = 0; i < 50; i++) {
        int imm = (rand() % 4096) - 2048;
        top->instr  = make_bne(1, 3, imm);
        top->ImmSrc = 1;
        tick();
        EXPECT_EQ((int32_t)top->Immout, (int32_t)imm);
    }
}


// ---------------------- MAIN ----------------------
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    top = new Vsignextend;
    tfp = new VerilatedVcdC;

    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("signextend_wave.vcd");

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    top->final();
    tfp->close();
    delete top;
    delete tfp;

    return result;
}
