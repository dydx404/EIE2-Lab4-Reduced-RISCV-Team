#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>

int main(int argc, char **argv, char **env) {

    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    // init top module
    Vtop* top = new Vtop;

    // init VCD tracing
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("cpu.vcd");

    // initialize inputs
    top->clk = 1;
    top->rst = 1;

    // run simulation
    for (i = 0; i < 300; i++) {

        // toggle clock twice per cycle
        for (clk = 0; clk < 2; clk++) {

            tfp->dump(2 * i + clk);  // VCD timestamp

            // toggle clock
            top->clk = !top->clk;
            top->eval();

            // detect rising edge → instruction executed
            if (top->clk == 1) {
                std::cout << "cycle " << i
                          << "  a0=" << top->a0
                          << std::endl;
            }
        }

        // reset asserted for first 2 cycles only
        top->rst = (i < 2);

        if (Verilated::gotFinish()) break;
    }

    tfp->close();
    delete top;
    return 0;
}
