module cu_signextend_top(
    input  wire [31:0]      pc,
    input  wire             eq,      
    output wire             RegWrite,
    output wire             ALUCtrl, 
    output wire             ALUSrc,
    output wire             PCSrc,
    output wire             MemtoReg,
    output wire             MemRead,
    output wire  [31:0]     ImmOp,
    output wire  [31:0]     instr
    
);

    wire [1:0]  ImmSrc;

    instruction instrmem(pc, instr);
    CU ctrl(instr, eq, RegWrite, ALUSrc, ImmSrc, ALUCtrl, PCSrc, MemtoReg, MemRead);
    signextend extend(instr, ImmSrc, ImmOp);

endmodule