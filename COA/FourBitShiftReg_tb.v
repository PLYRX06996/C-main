`timescale 1ns/
module FourBitShiftReg_tb;
    reg clk, rst, Din;
    wire [3:0] Q;

    shift_reg uut(.clk(clk), .rst(rst), .Din(Din), .Q(Q));

    always #5 clk = ~clk;

    initial begin
        $dumpfile("FourBitShiftReg.vcd");
        $dumpvars(0, FourBitShiftReg_tb);

        $display()
    end
    endmodule