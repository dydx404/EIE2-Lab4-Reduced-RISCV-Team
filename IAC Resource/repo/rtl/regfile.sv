// regfile.sv
// 32 x 32-bit RISC-V register file
// - 2 async read ports (rd1, rd2)
// - 1 sync write port (we3, ad3, wd3)
// - x0 is hardwired to 0
// - a0 (x10) is exposed as a separate output

module regfile(
    input  logic        clk,
    input  logic        we3,
    input  logic [4:0]  ad1,
    input  logic [4:0]  ad2,
    input  logic [4:0]  ad3,
    input  logic [31:0] wd3,
    output logic [31:0] rd1,
    output logic [31:0] rd2,
    output logic [31:0] a0
);

    logic [31:0] regs [0:31];

    // asynchronous reads
    assign rd1 = (ad1 == 5'd0) ? 32'b0 : regs[ad1];
    assign rd2 = (ad2 == 5'd0) ? 32'b0 : regs[ad2];

    assign a0  = regs[5'd10];

    always_ff @(posedge clk) begin
        if (we3 && (ad3 != 5'd0)) begin
            regs[ad3] <= wd3;
        end
    end

endmodule
