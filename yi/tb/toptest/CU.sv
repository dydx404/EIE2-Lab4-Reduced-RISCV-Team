module CU (
    input  wire [31:0] instr,
    input  wire        eq,      

    output reg         RegWrite,
    output reg         ALUSrc,
    output reg  [1:0]  ImmSrc,
    output reg         ALUCtrl,   // 1-bit: 0=ADD, 1=SUB
    output reg         PCSrc
);

    wire [6:0] opcode = instr[6:0];
    wire [2:0] funct3 = instr[14:12];

    always @* begin

        // defaults
        RegWrite = 1'b0;
        ALUSrc   = 1'b0;
        ImmSrc   = 2'b00;
        ALUCtrl  = 1'b0;
        PCSrc    = 1'b0;

        case (opcode)

            // -------------------------
            // I-type: ADDI
            // -------------------------
            7'b0010011: begin
                RegWrite = 1'b1;
                ALUSrc   = 1'b1;      // use imm
                ImmSrc   = 2'b00;     // I-type imm
                ALUCtrl  = 1'b0;      // ADD
            end

            // -------------------------
            // B-type: BNE only
            // -------------------------
            7'b1100011: begin
                RegWrite = 1'b0;
                ALUSrc   = 1'b0;      // use registers
                ImmSrc   = 2'b01;     // B-type imm
                ALUCtrl  = 1'b1;      // SUB for comparison

                if (funct3 == 3'b001) // BNE
                    PCSrc = ~eq;      // branch if rs1 != rs2
                else
                    PCSrc = 1'b0;     // ignore other branches
            end

            default: begin
                // keep defaults
            end
        endcase
    end

endmodule
