module instruction (
    input  wire [31:0] pc,
    output wire [31:0] instr
);
    parameter MEM = 256;
    reg [31:0] rom [0:MEM-1];

    initial begin
        //we can initialise ROM values for testing here
    end

    wire [31:0] word_index = pc[31:2];
    assign instr = rom[word_index];

endmodule