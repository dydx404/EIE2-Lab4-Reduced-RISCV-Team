#include "Vregalu.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "../vbuddy.cpp"

Vregalu *top;
VerilatedVcdC *tfp;
int tickcount = 0;

// Basic clock
void tick() {
    top->clk = 0; top->eval(); tfp->dump(tickcount++);
    top->clk = 1; top->eval(); tfp->dump(tickcount++);
}

// Slow down human-visible refresh
void pause_frames(int frames) {
    for (int f = 0; f < frames; f++) {
        vbdCycle(f);
        tick();
    }
}

// Update all 3 visible 7-segment displays + LED bar
void show(const char* phase) {
    int alu = top->aluout & 0xF;     // one digit (0–F)
    int a0  = top->a0     & 0xF;
    int eq  = top->eq     & 0x1;

    // Three 7-seg digits:
    vbdHex(1, alu);      // rightmost: aluout
    vbdHex(2, a0);       // middle: a0
    vbdHex(3, eq);       // leftmost: eq flag

    // LED bar flashes when eq=1
    vbdBar(eq ? 0xFF : 0x00);

    // Terminal output
    printf("[%s] aluout=%3d | a0=%3d | eq=%d\n",
           phase, top->aluout, top->a0, top->eq);
}

int main(int argc, char **argv) {

    Verilated::commandArgs(argc, argv);
    top = new Vregalu;

    // Waveform
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("redcycle.vcd");

    // Vbuddy
    if (vbdOpen() != 1) return -1;
    vbdHeader("RED BLOCK 3-DIGIT DEMO");
    vbdSetMode(0);  // <<— HEX DISPLAY MODE (show 7-seg)

    printf("\n===== RED BLOCK 3-DIGIT DEMO =====\n");
    printf("HEX3 = EQ   | HEX2 = A0  | HEX1 = ALUOUT\n");
    printf("Values slowed for human observation.\n");
    printf("====================================\n\n");

    int cyc = 0;

    while (cyc < 20000) {

        // =============================
        // PHASE 1: SLOW COUNT UP
        // =============================
        printf("\n--- PHASE 1: COUNT UP ---\n");

        for (int i = 0; i <= 8; i++) {

            top->rs1 = 0;
            top->rd  = 10;
            top->imm = i;
            top->alusrc = 1;
            top->aluctrl = 0;   // ADD
            top->regwrite = 1;

            tick();
            show("COUNT-UP");

            // >> slow enough to watch values climb
            pause_frames(250);

            cyc++;
        }

        // =============================
        // PHASE 2: COUNT DOWN COMPARISON
        // =============================
        printf("\n--- PHASE 2: COUNT DOWN UNTIL EQ=1 ---\n");

        for (int i = 8; i >= 0; i--) {

            top->rs1 = 10;
            top->rs2 = 0;
            top->imm = i;

            top->alusrc  = 1; // subtract imm
            top->aluctrl = 1; // SUB
            top->regwrite = 0;

            tick();
            show("COUNT-DOWN");

            // slow enough to see decreasing
            pause_frames(250);

            cyc++;

            if (top->eq) break;
        }

        // =============================
        // PHASE 3: RESET a0
        // =============================
        printf("\n--- PHASE 3: RESET a0 ---\n");

        top->rs1 = 0;
        top->rd  = 10;
        top->imm = 0;
        top->alusrc = 1;
        top->aluctrl = 0;
        top->regwrite = 1;

        tick();
        show("RESET");

        pause_frames(300);
        cyc++;
    }

    vbdClose();
    tfp->close();
    return 0;
}
