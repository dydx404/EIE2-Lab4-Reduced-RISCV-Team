module signextend(
    input  wire [31:0] instr,
    input  wire [1:0]  ImmSrc,
    output reg  [31:0] Immout
);

    // I-type immediate (ADDI)
    wire [31:0] i_Imm = {{20{instr[31]}}, instr[31:20]};

    // B-type immediate (BEQ)
    // Note: RISC-V branch immediate is shifted left by 1 (imm[0] = 0)
    wire [12:0] b_inter = {
        instr[31],       // imm[12]
        instr[7],        // imm[11]
        instr[30:25],    // imm[10:5]
        instr[11:8],     // imm[4:1]
        1'b0             // imm[0]
    };

    wire [31:0] b_Imm = {{19{b_inter[12]}}, b_inter};

    always @(*) begin
        case (ImmSrc)
            2'b00: Immout = i_Imm;  // I-type
            2'b01: Immout = b_Imm;  // B-type
            default: Immout = i_Imm;
        endcase
    end

endmodule
