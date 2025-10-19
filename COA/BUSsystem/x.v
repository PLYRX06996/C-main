module x(
    input  wire [1:0] S,
    output wire [3:0] BUS
);

    wire [3:0] A = 4'b0001;
    wire [3:0] B = 4'b0010;
    wire [3:0] C = 4'b0100;
    wire [3:0] D = 4'b1000;

   assign BUS = (S == 2'b00) ? A : (S == 2'b01) ? B : (S == 2'b10) ? C : D ;

endmodule