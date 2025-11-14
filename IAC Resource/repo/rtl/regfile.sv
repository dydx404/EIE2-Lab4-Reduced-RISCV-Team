// regfile.sv
// 32 x 32-bit RISC-V register file
// - 2 async read ports (rd1, rd2)
// - 1 sync write port (we3, ad3, wd3)
// - x0 is hardwired to 0
// - a0 (x10) is exposed as a separate output

module regfile(
    input  logic        clk,
    input  logic        we3,          // write enable (RegWrite)
    input  logic [4:0]  ad1,          // rs1 address
    input  logic [4:0]  ad2,          // rs2 address
    input  logic [4:0]  ad3,          // rd address (write dest)
    input  logic [31:0] wd3,          // write data
    output logic [31:0] rd1,          // read data 1 (rs1)
    output logic [31:0] rd2,          // read data 2 (rs2)
    output logic [31:0] a0            // register x10 (for top-level output)
);

    // 32 general-purpose registers, x0..x31
    logic [31:0] regs [0:31];

    // asynchronous reads
    assign rd1 = (ad1 == 5'd0) ? 32'b0 : regs[ad1];    // x0 always 0
    assign rd2 = (ad2 == 5'd0) ? 32'b0 : regs[ad2];

    // expose a0 (x10) directly
    assign a0  = regs[5'd10];  // safe because we never write x0 anyway

    // synchronous write on rising edge
    always_ff @(posedge clk) begin
        if (we3 && (ad3 != 5'd0)) begin
            regs[ad3] <= wd3;          // never write to x0
        end
    end

endmodule
