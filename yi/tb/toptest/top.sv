module top(
    input  logic clk,
    input  logic rst,
    output logic [31:0] a0

);


    logic [31:0] pc;
    logic [31:0] instr;
    logic [31:0] ImmOp;
    logic [31:0] aluout;

    logic eq;
    logic RegWrite;
    logic ALUSrc;
    logic PCSrc;
    logic ALUCtrl;         // 1-bit
    logic [1:0] ImmSrc;    // from CU

    // 1. Program Counter
    pc pc_inst(
        .clk(clk),
        .rst(rst),
        .pcsrc(PCSrc),
        .immop(ImmOp),
        .pccur(pc)
    );

    // 2. Instruction Memory
    instruction imem(
        .pc(pc),
        .instr(instr)
    );

    // 3. Control Unit
    CU cu(
        .instr(instr),
        .eq(eq),
        .RegWrite(RegWrite),
        .ALUSrc(ALUSrc),
        .ImmSrc(ImmSrc),
        .ALUCtrl(ALUCtrl),
        .PCSrc(PCSrc)
    );

    // 4. Sign Extend
    signextend se(
        .instr(instr),
        .ImmSrc(ImmSrc),
        .Immout(ImmOp)
    );

    // 5. Register File + ALU bundle
    regalu alu_reg(
        .clk(clk),
        .regwrite(RegWrite),
        .alusrc(ALUSrc),
        .aluctrl(ALUCtrl),
        .rs1(instr[19:15]),
        .rs2(instr[24:20]),
        .rd(instr[11:7]),
        .imm(ImmOp),
        .aluout(aluout),
        .eq(eq),
        .a0(a0)
    );

endmodule
