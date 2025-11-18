// ----------------------------------------------------------
// CU + SignExtend wrapper
// Clean version (no instruction memory, no PC input)
// ----------------------------------------------------------

module cu_signextend_top(
    input  logic [31:0] instr,     // instruction from instruction memory
    input  logic        eq,        // ALU equality flag

    output logic        RegWrite,
    output logic [0:0]  ALUCtrl,   // 1-bit reduced ALU ctrl
    output logic        ALUSrc,
    output logic        PCSrc,
    output logic [31:0] ImmOp      // extended immediate
);

    logic [1:0] ImmSrc;            // internal: CU decides how to extend

    // ----------------------
    // Control Unit
    // ----------------------
    CU cu(
        .instr(instr),
        .eq(eq),
        .RegWrite(RegWrite),
        .ALUSrc(ALUSrc),
        .ImmSrc(ImmSrc),
        .ALUCtrl(ALUCtrl),
        .PCSrc(PCSrc)
    );

    // ----------------------
    // Sign Extend block
    // ----------------------
    signextend se(
        .instr(instr),
        .ImmSrc(ImmSrc),
        .ImmOp(ImmOp)
    );

endmodule
