#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vpc.h"
#include "../vbuddy.cpp"

#define MAX_SIM_CYC 100000

Vpc* top;
VerilatedVcdC* tfp;
int tickcount = 0;

// Perform exactly ONE PC update
void tick_once() {
    top->clk = 0; top->eval(); tfp->dump(tickcount++);
    top->clk = 1; top->eval(); tfp->dump(tickcount++);
    top->clk = 0; top->eval();
}

// Delay for human visualisation WITHOUT affecting the PC state
void delay_visual(int frames) {
    for (int i = 0; i < frames; i++) {
        vbdCycle(i);   // GUI update only
        // Do NOT tick the PC
    }
}

// Show PC on 4 HEX digits (lower 16 bits)
void show_pc_hex(uint32_t pc) {
    vbdHex(1, (pc >>  0) & 0xF);
    vbdHex(2, (pc >>  4) & 0xF);
    vbdHex(3, (pc >>  8) & 0xF);
    vbdHex(4, (pc >> 12) & 0xF);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    top = new Vpc;

    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("pc_vbuddy.vcd");

    if (vbdOpen() != 1) return -1;

    vbdHeader("PC DEMO");
    vbdSetMode(1);

    // Reset sequence
    top->rst = 1;
    tick_once();
    top->rst = 0;

    for (int simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++) {

        // --------------------------------------------------
        // INPUTS FROM VBUDDY
        // --------------------------------------------------

        // Button controls pcsrc (branch enable)
        top->pcsrc = vbdFlag();   

        // Knob 0..100 → effective jump -50..+50
        int knob = vbdValue();          // raw knob reading (0–100)
        int32_t eff_jump = knob - 50;   // remap to signed range
        top->immop = eff_jump;          // immediate as byte offset

        // --------------------------------------------------
        // ONE PC UPDATE
        // --------------------------------------------------
        tick_once();

        uint32_t pc_now = top->pccur;

        // --------------------------------------------------
        // VISUAL OUTPUTS
        // --------------------------------------------------

        show_pc_hex(pc_now);
        vbdBar(top->pcsrc ? 255 : 0);  // red LED bar when branching

        // --------------------------------------------------
        // CALCULATE NEXT PC
        // --------------------------------------------------

        uint32_t pc_next = top->pcsrc ? pc_now + eff_jump : pc_now + 4;

        const char* direction =
            eff_jump > 0 ? "FORWARD" :
            eff_jump < 0 ? "BACKWARD" :
                           "NO JUMP";

        // --------------------------------------------------
        // DEBUG INFORMATION
        // --------------------------------------------------

        printf("\n======================\n");
        printf("   PC DEBUG (Blue Block)\n");
        printf("----------------------\n");
        printf(" Current PC      : %u (0x%08X)\n", pc_now, pc_now);
        printf(" Knob value      : %d (0–100)\n", knob);
        printf(" immop (signed)  : %d bytes\n", eff_jump);
        printf(" Jump direction  : %s\n", direction);

        if (eff_jump >= 0)
            printf(" Effective jump  : +%u bytes\n", (unsigned)eff_jump);
        else
            printf(" Effective jump  : %u bytes backward\n", (unsigned)(-eff_jump));

        printf(" Next PC         : %u (0x%08X)\n", pc_next, pc_next);
        printf(" Branch taken?   : %s\n", top->pcsrc ? "YES" : "NO");
        printf("======================\n");

        // --------------------------------------------------
        // HUMAN-VISIBLE DELAY
        // --------------------------------------------------
        delay_visual(150);   // adjust 100–200 for clarity

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            break;
    }

    vbdClose();
    tfp->close();
    delete top;
    delete tfp;

    return 0;
}
