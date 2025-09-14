module decode2x4(input reg A, input reg B, output wire [3:0] I);
    assign I[3] = A & B;
    assign I[2] = ~A & B;
    assign I[1] = A & ~B;
    assign I[0] = ~A & ~B;
    endmodule
