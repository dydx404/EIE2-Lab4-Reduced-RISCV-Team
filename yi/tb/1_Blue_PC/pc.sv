module pc #(
    parameter DATA_WIDTH = 32
) (
    input   logic                   clk,
    input   logic                   rst,
    input   logic                   pcsrc,
    input   logic [DATA_WIDTH-1:0]  immop,
    output  logic [DATA_WIDTH-1:0]  pccur
);
    logic [DATA_WIDTH-1:0] pcnext;
    logic [DATA_WIDTH-1:0] pcinc;
    logic [DATA_WIDTH-1:0] pcbranch;
    
    // PC + 4
    assign pcinc = pccur + 32'd4;
    assign pcbranch = pccur + immop;
    // MUX for PC source
    mux #(
        .DATA_WIDTH(DATA_WIDTH)
    ) pc_mux (
        .in0(pcinc),
        .in1(pcbranch),
        .sel(pcsrc),
        .out(pcnext)
    );
    
    // PC Register
    pcreg #(
        .DATA_WIDTH(DATA_WIDTH)
    ) pc_register (
        .clk(clk),
        .rst(rst),
        .d(pcnext),
        .q(pccur)
);
endmodule
