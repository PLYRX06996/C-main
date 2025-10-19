module FourBitALU(
    input [3:0] A, B,
    input S1, S0, Cin,
    output [3:0] D,
    output Cout
);

    wire [3:0] Y;
    wire [4:0] result;

    assign Y = (S1 == 0 && S0 == 0) ? B :
                (S1 == 0 && S0 == 1) ? ~B :
                (S1 == 1)            ? 4'b0000 : 4'b0000;

    assign result = A + Y + Cin;

    assign D = result[3:0];
    assign Cout = result[4];

endmodule