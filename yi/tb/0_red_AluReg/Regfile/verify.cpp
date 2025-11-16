#include "gtest/gtest.h"
#include "Vregfile.h"            // Verilated model of the register file
#include "verilated.h"
#include "verilated_vcd_c.h"

// Global DUT + waveform tracer.
// Keeping these outside the fixture keeps the tests uncluttered.
Vregfile *top;
VerilatedVcdC *tfp;
unsigned long tickcount = 0;

// Basic clock tick: evaluate, dump, toggle clk.
// The regfile is synchronous, so toggling the clock matters here.
void tick() {
    top->eval();
    tfp->dump(tickcount++);
    top->clk = !top->clk;

    top->eval();
    tfp->dump(tickcount++);
    top->clk = !top->clk;
}

// ------------------ GTEST FIXTURE ------------------
// The shared setup for every test. Makes sure each test
// starts from a consistent, predictable state.
class RegfileTest : public ::testing::Test {
protected:
    void SetUp() override {
        top->clk = 0;
        top->we3 = 0;
        top->ad1 = 0;
        top->ad2 = 0;
        top->ad3 = 0;
        top->wd3 = 0;
        tick();      // Let everything settle
    }

    void TearDown() override {
        // No persistent state to clean up
    }

    // A tiny helper 
    void write_reg(int rd, int value) {
        top->we3 = 1;      // enable write
        top->ad3 = rd;     // destination register
        top->wd3 = value;  // data to write
        tick();            // write happens on clock edge
        top->we3 = 0;      // disable writes afterwards
    }
};

// ------------------ TEST CASES ------------------
// Each test checks one specific behaviour of the regfile.

TEST_F(RegfileTest, X0AlwaysZero) {
    // Writes to x0 must be ignored by design.
    write_reg(0, 0xDEADBEEF);
    top->ad1 = 0;
    top->eval();
    EXPECT_EQ(top->rd1, 0u);
}

TEST_F(RegfileTest, WriteAndReadRegister) {
    // Basic sanity check: write a value, then read it back.
    write_reg(5, 1234);
    top->ad1 = 5;
    top->eval();
    EXPECT_EQ(top->rd1, 1234);
}

TEST_F(RegfileTest, DualReadPorts) {
    // The two read ports must operate independently.
    write_reg(3, 99);
    write_reg(7, 123);

    top->ad1 = 3;
    top->ad2 = 7;
    top->eval();

    EXPECT_EQ(top->rd1, 99);
    EXPECT_EQ(top->rd2, 123);
}

TEST_F(RegfileTest, A0MatchesX10) {
    // a0 is a convenience alias for x10.
    write_reg(10, 0xBEEF);
    EXPECT_EQ(top->a0, 0xBEEF);
}

TEST_F(RegfileTest, RandomAccess) {
    // Light fuzzing across all general-purpose registers.
    for (int i = 1; i < 32; i++) {   // skip x0, already tested
        int v = rand();
        write_reg(i, v);

        top->ad1 = i;
        top->eval();
        EXPECT_EQ(top->rd1, (uint32_t)v);
    }
}

// ------------------ MAIN ------------------
// Standard Verilator + GTest startup sequence.
// This is the small bootstrapping ritual that makes the testbench run.
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    top = new Vregfile;

    // Enable waveform tracing.
    tfp = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("regfile.vcd");

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    top->final();
    tfp->close();
    delete top;
    delete tfp;

    return result;
}
