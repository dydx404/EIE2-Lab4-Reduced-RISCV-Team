//regalu.sv
//combined top module for reg and alu

module regalu (
    input  logic        clk,
    //input  logic        rst,

    //control signals
    input  logic        regwrite,
    input  logic        alusrc,
    input  logic        aluctrl,

    //instruction register fields
    input  logic [4:0]  rs1,
    input  logic [4:0]  rs2,
    input  logic [4:0]  rd,

    //immediate
    input  logic [31:0] imm,

    //outputs
    output logic [31:0] aluout,
    output logic        eq,
    output logic [31:0] a0
);

    logic [31:0] RD1, RD2;
    logic [31:0] aluop2;

    //regfile instance
    regfile rf (
        .clk (clk),
        .we3 (regwrite),
        .ad1 (rs1),
        .ad2 (rs2),
        .ad3 (rd),
        .wd3 (aluout),
        .rd1 (RD1),
        .rd2 (RD2),
        .a0  (a0)
    );

    //mux for ALU second operand
    mux #(32) srcmux (
        .in0 (RD2),
        .in1 (imm),
        .sel (alusrc),
        .out (aluop2)
    );

    //ALU instance
    alu alu32 (
        .aluop1 (RD1),
        .aluop2 (aluop2),
        .aluctrl(aluctrl),
        .aluout (aluout),
        .eq     (eq)
    );

endmodule
