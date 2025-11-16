#include "gtest/gtest.h"
#include "Valu.h"               // Verilated model of the ALU module
#include "verilated.h"
#include "verilated_vcd_c.h"

// Global pointers to the DUT and waveform tracer.
// Keeping them global keeps the GTest fixture clean.
Valu *top;
VerilatedVcdC *tfp;
unsigned long tickcount = 0;

// A minimal “tick” helper.
// The ALU is combinational, but calling eval/dump twice maintains
// a familiar waveform structure (similar to sequential designs).
void tick() {
    top->eval();
    tfp->dump(tickcount);
    tickcount++;

    top->eval();
    tfp->dump(tickcount);
    tickcount++;
}

// ------------------ GTEST FIXTURE ------------------
// A test fixture to reuse setup logic across all tests.
//
class ALUTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Put the DUT in a clean, deterministic state.
        top->aluop1 = 0;
        top->aluop2 = 0;
        top->aluctrl = 0;
        tick();    // Ensure changes propagate into the trace.
    }

    void TearDown() override {
        // ALU has no internal state, so nothing to clean up.
    }

    // Small helper so the test code looks cleaner.
    void drive(int a, int b, int ctrl) {
        top->aluop1  = a;
        top->aluop2  = b;
        top->aluctrl = ctrl;
        tick();
    }
};

// ------------------ TEST CASES ------------------
// Each test targets one behaviour of the ALU. These are all
// quick sanity checks — they don't try to exhaustively prove correctness.

TEST_F(ALUTest, AddBasic) {
    drive(10, 5, 0);         // ctrl=0 ⇒ ADD
    EXPECT_EQ(top->aluout, 15);
    EXPECT_EQ(top->eq, 0);   // eq==1 only when result is zero
}

TEST_F(ALUTest, SubBasic) {
    drive(20, 7, 1);         // ctrl=1 ⇒ SUB
    EXPECT_EQ(top->aluout, 13);
    EXPECT_EQ(top->eq, 0);
}

TEST_F(ALUTest, ZeroOnSub) {
    drive(42, 42, 1);        // Equal operands ⇒ result zero
    EXPECT_EQ(top->aluout, 0);
    EXPECT_EQ(top->eq, 1);
}

TEST_F(ALUTest, ZeroOnAdd) {
    drive(-5, 5, 0);
    EXPECT_EQ(top->aluout, 0);
    EXPECT_EQ(top->eq, 1);
}

TEST_F(ALUTest, OverflowWrap) {
    // Classic unsigned overflow: 0xFFFFFFFF + 1 → wraps to 0
    drive(0xFFFFFFFF, 1, 0);
    EXPECT_EQ(top->aluout, 0);
    EXPECT_EQ(top->eq, 1);
}

TEST_F(ALUTest, RandomNumbers) {
    // A bit of fuzzing for the adder path.
    for (int i = 0; i < 100; i++) {
        int a = rand();
        int b = rand();
        drive(a, b, 0);
        EXPECT_EQ(top->aluout, (int32_t)(a + b));
    }
}


// Standard Verilator + GoogleTest bootstrapping.
//
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    top = new Valu;              // Instantiate the DUT
    tfp = new VerilatedVcdC;     // Create the VCD tracer

    Verilated::traceEverOn(true);
    top->trace(tfp, 99);         // Allow deep tracing
    tfp->open("alu_wave.vcd");   // Output waveform file

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();  // Run the entire suite

    // Tidy up before exiting.
    top->final();
    tfp->close();
    delete top;
    delete tfp;

    return result;
}
