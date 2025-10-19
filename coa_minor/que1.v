module que1 (
    input [4:1] B,
    output [4:1] X
);

    wire [7:1] w;

    assign w[7] = B[3] & B[2];
    assign w[6] = B[3] & B[1];
    assign w[5] = B[3] & ~B[2] & ~B[1];
    assign w[4] = B[3] & B[1];
    assign w[3] = ~B[3] & B[2];
    assign w[2] = ~B[2] & ~B[1];
    assign w[1] = B[2] & B[1];

    assign X[4] = B[4] | w[7] | w[6];
    assign X[3] = w[5] | w[4] | w[3];
    assign X[2] = w[2] | w[1];
    assign X[1] = ~B[1];
    
endmodule