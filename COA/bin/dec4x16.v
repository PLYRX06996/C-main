module dec2x4 (
    input [1:0] A,
    output [3:0] Y
);
    assign Y = 4'b0001 << A;    
endmodule

module dec4x16 (
    input [3:0] A,
    output [15:0] Y
);
    wire [3:0] deciding;
    wire [3:0] y3, y2, y1, y0;

    dec2x4 decideDEC(.A(A[3:2]), .Y(deciding));

    dec2x4 dec3(.A(A[1:0]), .Y(y3));
    dec2x4 dec2(.A(A[1:0]), .Y(y2));
    dec2x4 dec1(.A(A[1:0]), .Y(y1));
    dec2x4 dec0(.A(A[1:0]), .Y(y0));

    assign Y[15:12] = decideDEC[3] ? y3 : 4'b0000;
    assign Y[11:8] = decideDEC[2] ? y2 : 4'b0000;
    assign Y[7:4] = decideDEC[1] ? y1 : 4'b0000;
    assign Y[3:0] = decideDEC[0] ? y3 : 4'b0000;
    
endmodule