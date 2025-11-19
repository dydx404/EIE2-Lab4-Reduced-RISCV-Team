module CU (
    input  wire [31:0]  instr,
    input  wire         eq,      
    output reg          RegWrite,
    output reg          ALUSrc,
    output reg  [1:0]   ImmSrc,
    output reg          ALUCtrl,  
    output reg          PCSrc,
    output reg          MemtoReg,
    output reg          MemRead //decided whether register input value comes from ALU or from Memory
);

    wire [6:0] opcode = instr[6:0];

    always @* begin

        //
        RegWrite = 1'b0;
        ALUSrc   = 1'b0;
        ImmSrc   = 2'b00;
        ALUCtrl  = 3'b000;
        PCSrc    = 1'b0;
        MemtoReg = 0;
        MemRead = 0;
        //

        case (opcode)
            7'b0010011: begin //addi
                RegWrite = 1'b1;
                ALUSrc   = 1'b1;
                ImmSrc   = 2'b00;
                //since we're assuming we're just doing addi for now
                ALUCtrl = 1'b0;
                PCSrc = 1'b0;
                MemRead = 0;
                MemtoReg = 0;
            end

            7'b1100011: begin //bne
                RegWrite = 1'b0;
                ALUSrc   = 1'b0;
                ImmSrc   = 2'b01;
                //assuming only bne for now
                ALUCtrl  = 1'b1;
                PCSrc = ~eq;
                MemRead = 0;
                MemtoReg = 0;
            end

             7'b0000011: begin //lw
                RegWrite = 1'b1;
                ALUSrc   = 1'b1;
                ImmSrc   = 2'b00;
                //since we're assuming we're just doing addi for now
                ALUCtrl = 1'b0;
                PCSrc = 1'b0;
                MemRead = 1;
                MemtoReg = 1;
                
            end

            default: begin
            end
        endcase
    end

endmodule