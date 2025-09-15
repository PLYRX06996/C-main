module dec2x4 (
    input [1:0] A,
    output [3:0] Y
);
    assign Y = 4'b0001 << A;    
endmodule