`timescale 1ns/1ps  
module que1_tb;
    reg [4:1] B;
    wire [4:1] X;

    que1 uut(.B(B), .X(X));

    initial begin
        $dumpfile("que1.vcd");
        $dumpvars(0, que1_tb);
        $monitor("t = %0t B = %b -> X = %b", $time, B, X);

        for (integer i = 0; i <= 8; i++) begin
            B = i;
            #10;
        end
        $finish;
    end
endmodule