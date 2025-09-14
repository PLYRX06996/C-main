`timescale 1ns/1ps

module DEC2x4_tb;
    reg [1:0] A;
    reg Enable;
    wire [3:0] Y;

    DEC2x4 uut (.A(A), .Enable(Enable), .Y(Y));

    initial begin
        $dumpfile("DEC2x4.vcd");
        $dumpvars(0, DEC2x4_tb);
        $monitor("t=%0t A=%b Enable=%b -> Y=%b", $time, A, Enable, Y);

        Enable = 0; A = 2'b00; #10; // Disabled
        Enable = 1; A = 2'b00; #10;   
        A = 2'b01; #10;
        A = 2'b10; #10; 
        A = 2'b11; #10;

        $finish;
    end
endmodule