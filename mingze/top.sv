module top #(
    DATA_WIDTH = 32
) (
    input   logic clk,
    input   logic rst,
    output  logic [DATA_WIDTH-1:0] a0    
);
    logic [DATA_WIDTH-1:0] pc;
    logic eq;
    logic RegWrite;
    logic [2:0] ALUCtrl;
    logic ALUSrc;
    logic PCSrc;
    logic [DATA_WIDTH-1:0] ImmOp;
    logic [DATA_WIDTH-1:0] instr;

    pc #(
        .DATA_WIDTH(DATA_WIDTH)
    )top_pc(
        .clk(clk),
        .rst(rst),
        .pcsrc(PCSrc),
        .immop(ImmOp),
        .pccur(pc)
    );

    cu_signextended_top top_cu(
        .pc(pc),
        .eq(eq),
        .RegWrite(RegWrite),
        .ALUCtrl(ALUCtrl),
        .ALUSrc(ALUSrc),
        .PCSrc(PCSrc),
        .ImmOp(ImmOp),
        .instr(instr)
    );

    regalu top_alu(
        .clk(clk),
        .regwrite(RegWrite),
        .alusrc(ALUSrc),
        .aluctrl(ALUCtrl),
        .rs1(instr[19:15]),
        .rs2(instr[24:20]),
        .rd(instr[11:7]),
        .imm(ImmOp),
        .eq(eq),
        .a0(a0)
    );

endmodule
