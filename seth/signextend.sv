module signextend(
    input  wire [31:0] instr,
    input  wire [1:0]  ImmSrc,
    output reg  [31:0] immout
);

    wire [31:0] i_Imm = {{20{instr[31]}}, instr[31:20]}; 
    wire [12:0] b_inter = {instr[31], instr[7], instr[30:25], instr[11:8]};
    wire [31:0] b_imm = {{19{b_inter[12]}}, b_inter, 1'b0}; 

    always @* begin
        case(ImmSrc)
            2'b00: immout = i_Imm;
            2'b01: immout = b_Imm;
            default: immout = i_imm;
        endcase
    end

endmodule