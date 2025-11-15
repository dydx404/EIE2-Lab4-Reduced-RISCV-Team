// alu.sv
module alu(
    input  logic [31:0] aluop1,
    input  logic [31:0] aluop2,
    input  logic        aluctrl,
    output logic [31:0] aluout,    
    output logic        eq
);
    assign aluout = aluctrl?(aluop1-aluop2):(aluop1+aluop2);

    assign eq = (aluout == 32'b0);

endmodule
