module CU (
    input  wire [31:0] instr,
    input  wire        eq,      
    output reg         RegWrite,
    output reg         ALUSrc,
    output reg  [1:0]  ImmSrc,
    output reg  [2:0]  ALUCtrl,  
    output reg         PCSrc
);

    wire [6:0] opcode = instr[6:0];

    always @* begin

        //
        RegWrite = 1'b0;
        ALUSrc   = 1'b0;
        ImmSrc   = 2'b00;
        ALUCtrl  = 3'b000;
        PCSrc    = 1'b0;
        //

        case (opcode)
            7'b0010011: begin //addi
                RegWrite = 1'b1;
                ALUSrc   = 1'b1;
                ImmSrc   = 2'b00;
                //since we're assuming we're just doing addi for now
                ALUCtrl = 3'b000;
            end

            7'b1100011: begin //bne
                RegWrite = 1'b0;
                ALUSrc   = 1'b0;
                ImmSrc   = 2'b01;
                //assuming only bne for now
                ALUCtrl  = 3'b001;
                PCSrc = ~eq;
            end

            default: begin
            end
        endcase
    end

endmodule