`timescale 1ns/1ps
module dec4x16_tb;
    reg [3:0] A;
    wire [15:0] Y;

    dec4x16 uut(.A(A), .Y(Y));

    initial begin
        A = 0;
        end

    initial begin
        $dumpfile("dec4x16.vcd");
        $dumpvars(0, dec4x16_tb);
        $monitor("t = %0t A = %b -> Y = %b", $time, A, Y);
        for (integer i = 0; i < 16; i++) begin
            A = i;
            #5;
        end
        $finish;
    end
    
endmodule