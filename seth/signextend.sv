module signextend(
    input  wire [31:0] instr,
    input  wire [1:0]  ImmSrc,
    output reg  [31:0] ImmOp
);

    wire [31:0] i_Imm = {{20{instr[31]}}, instr[31:20]}; 
    wire [12:0] b_inter = {instr[31], instr[7], instr[30:25], instr[11:8], 1'b0 }; //RISC-V branch offsets are 2 bytes (?)
    wire [31:0] b_Imm = {{19{b_inter[12]}}, b_inter}; 

    always @* begin
        case(ImmSrc)
            2'b00: ImmOp = i_Imm;
            2'b01: ImmOp = b_Imm;
            default: ImmOp = i_Imm;
        endcase
    end

endmodule