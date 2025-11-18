module instruction (
    input  wire [31:0] pc,
    output wire [31:0] instr
);
    parameter MEM = 256;
    reg [31:0] rom [0:MEM-1];

    // Load the program into ROM
    initial begin
        $readmemh("imem.hex", rom);
    end

    // pc[1:0] are always 00 because PC increments by 4
    // Use pc[9:2] to select from 256-word ROM
    wire [7:0] word_index = pc[9:2];

    assign instr = rom[word_index];

endmodule
