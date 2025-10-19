module alu_1bit (
    input A, B, Cin,
    input [1:0] S,
    output reg F,
    output Cout
);
    wire and_out, or_out, xor_out, sum;

    assign and_out = A & B;
    assign or_out = A | B;
    assign xor_out = A ^ B;
    assign {Cout, sum} = A + B + Cin;

    always @(*) begin
        case(S)
        2'b00: F = and_out;
        2'b01: F = or_out;
        2'b10: F = xor_out;
        2'b11: F = sum;
        default: F = 1'b0;
        endcase
    end
endmodule