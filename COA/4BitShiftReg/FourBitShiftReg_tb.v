`timescale 1ns/1ps
module FourBitShiftReg_tb;
    reg clk, rst, din;
    wire [3:0] Q;

    FourBitShiftReg UUT (.clk(clk), .rst(rst), .din(din), .Q(Q));

    always #5 clk = ~clk;

    initial begin
        $dumpfile("FourBitShiftReg.vcd");
        $dumpvars(0, FourBitShiftReg_tb);

        $display("Time\t rst din Q");
        $monitor("%0t\t %b   %b   %b", $time, rst, din, Q);

        clk = 0; rst = 1; din = 0;
        #10 rst = 0;

        // shift in sequence: 1,0,1,1
        din = 1; #10;
        din = 0; #10;
        din = 1; #10;
        din = 1; #10;

        #20 $finish;
    end
endmodule