#include "gtest/gtest.h"
#include "Vcu_signextend_top.h"
#include "verilated.h"

static Vcu_signextend_top* top;

void eval() {
    top->eval();
}

// ---------------------------------------------------------
// Test fixture
// ---------------------------------------------------------
class GreenTest : public ::testing::Test {
protected:
    void SetUp() override {
        Verilated::traceEverOn(true);
        top = new Vcu_signextend_top;
    }

    void TearDown() override {
        delete top;
    }

    void step_pc(uint32_t pc) {
        top->pc = pc;
        eval();
    }
};

// ---------------------------------------------------------
// Test 1: Correct instruction fetch
// ---------------------------------------------------------
TEST_F(GreenTest, FetchCorrectInstruction) {
    // PC = 0 → index 0
    step_pc(0);
    EXPECT_EQ(top->instr, 0x00000013u);

    // PC = 4 → index 1
    step_pc(4);
    EXPECT_EQ(top->instr, 0x00100093u);

    // PC = 8 → index 2
    step_pc(8);
    EXPECT_EQ(top->instr, 0x00200113u);

    // PC = 12 → index 3
    step_pc(12);
    EXPECT_EQ(top->instr, 0x00308193u);
}

// ---------------------------------------------------------
// Test 2: Check CU + signextend outputs
// ---------------------------------------------------------
TEST_F(GreenTest, CUAndImmediateLogic) {
    // ADDI x1,x0,1
    step_pc(4);

    // Expected: I-type
    EXPECT_EQ(top->ALUSrc, 1);
    EXPECT_EQ(top->RegWrite, 1);
    EXPECT_EQ(top->PCSrc, 0);
    EXPECT_EQ(top->ALUCtrl, 0b000);

    // Immediate = 1
    EXPECT_EQ(top->ImmOp, 1);
}

// ---------------------------------------------------------
// Test main
// ---------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
