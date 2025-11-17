module cu_signextend_top(
    input  wire [31:0] pc,
    input  wire        eq,      
    output reg         RegWrite,
    output reg  [2:0]  ALUCtrl, 
    output reg         ALUSrc,
    output reg         PCSrc,
    output reg  [31:0] ImmOp,
    output wire [31:0] instr
);

    wire [1:0]  ImmSrc;

    instruction instrmem(pc, instr);
    CU ctrl(instr, eq, RegWrite, ALUSrc, ImmSrc, ALUCtrl, PCSrc);
    signextend extend(instr, ImmSrc, ImmOp);

endmodule