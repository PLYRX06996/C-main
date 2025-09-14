module DEC2x4 (
    input wire [1:0] A,
    input wire Enable,
    output wire [3:0] Y
);
    assign Y = (Enable) ? (4'b0001 << A) : 4'b0000;
endmodule
