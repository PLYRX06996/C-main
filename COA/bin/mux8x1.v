module mux8x1 (
    input s2, s1, s0, enable,
    input [7:0] I,
    output Y
);
    wire [7:0] w;

    assign w[7] = s2 & s1 & s0 & I[7];
    assign w[6] = s2 & s1 & ~s0 & I[6];
    assign w[5] = s2 & ~s1 & s0 & I[5];
    assign w[4] = s2 & ~s1 & ~s0 & I[4];
    assign w[3] = ~s2 & s1 & s0 & I[3];
    assign w[2] = ~s2 & s1 & ~s0 & I[2];
    assign w[1] = ~s2 & ~s1 & s0 & I[1];
    assign w[0] = ~s2 & ~s1 & ~s0 & I[0];

    assign Y = enable & (|w);
    
endmodule