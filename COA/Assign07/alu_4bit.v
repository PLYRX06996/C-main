module alu_4bit (
    input [3:0] A, B,
    input Cin,
    input [1:0] S,
    output reg [3:0] F,
    output reg Cout
);

    always @(*) begin
        case(S)
            2'b00: F = A & B;          // AND
            2'b01: F = A | B;          // OR
            2'b10: F = A ^ B;          // XOR
            2'b11: {Cout, F} = A + B + Cin; // ADD
            default: F = 4'b0000;
        endcase
    end
endmodule