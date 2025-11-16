#include "gtest/gtest.h"
#include "Vregalu.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

// ------------------ GLOBALS ------------------
Vregalu* top;
VerilatedVcdC* tfp;
unsigned long tickcount = 0;

// A single tick = rising edge + falling edge.
void tick() {
    // Evaluate before rising edge
    top->eval();
    tfp->dump(tickcount++);

    // Rising edge
    top->clk = 1;
    top->eval();
    tfp->dump(tickcount++);

    // Falling edge
    top->clk = 0;
    top->eval();
    tfp->dump(tickcount++);
}

// ------------------ FIXTURE ------------------
class RedALUTest : public ::testing::Test {
protected:
    void SetUp() override {
        top->clk = 0;

        // Control defaults
        top->regwrite = 0;
        top->alusrc   = 0;
        top->aluctrl  = 0;

        // Register fields
        top->rs1 = 0;
        top->rs2 = 0;
        top->rd  = 0;
        top->imm = 0;

        tick(); // settle
    }

    void TearDown() override {}
};


// ========================================================
// TEST 1: addi — writing immediate into a register
// x1 = x0 + 5
// ========================================================
TEST_F(RedALUTest, AddiBasic) {
    top->rs1 = 0;       // x0
    top->rd  = 1;       // x1
    top->imm = 5;
    top->alusrc = 1;    // use immediate
    top->aluctrl = 0;   // ADD
    top->regwrite = 1;

    tick();             // perform write on posedge

    EXPECT_EQ(top->aluout, 5u);
}


// ========================================================
// TEST 2: register + register add
// x1 = 10
// x2 = 3
// x3 = x1 + x2 = 13
// ========================================================
TEST_F(RedALUTest, AddRegReg) {
    // write x1 = 10
    top->rd = 1;
    top->imm = 10;
    top->alusrc = 1;
    top->aluctrl = 0;   // ADD
    top->regwrite = 1;
    tick();

    // write x2 = 3
    top->rd = 2;
    top->imm = 3;
    tick();

    // x3 = x1 + x2
    top->rs1 = 1;
    top->rs2 = 2;
    top->rd  = 3;
    top->alusrc = 0;    // use register RD2
    top->regwrite = 1;

    tick();

    EXPECT_EQ(top->aluout, 13u);
}


// ========================================================
// TEST 3: EQ flag behaviour
// x4 = 12
// x5 = 12
// SUB -> zero → eq=1
// ========================================================
TEST_F(RedALUTest, EQFlag) {
    // write x4 = 12
    top->rd = 4;
    top->imm = 12;
    top->alusrc = 1;
    top->aluctrl = 0;   // ADD for addi
    top->regwrite = 1;
    tick();

    // write x5 = 12
    top->rd = 5;
    top->imm = 12;
    tick();

    // compare x4 and x5 using SUB
    top->regwrite = 0;
    top->rs1 = 4;
    top->rs2 = 5;
    top->alusrc = 0;
    top->aluctrl = 1;   // SUB = comparison
    tick();

    EXPECT_EQ(top->eq, 1);
}


// ------------------------ MAIN ------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    top = new Vregalu;

    tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("red_basic.vcd");

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    top->final();
    tfp->close();
    delete top;
    delete tfp;

    return result;
}
