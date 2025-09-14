`timescale 1ns/1ps
module Dff_tb;

    reg clk, d;
    wire q, qbar;

    Dff uut (.clk(clk), .d(d), .q(q), .qbar(qbar));

    initial clk = 0;
    always #5 clk = ~clk;

    initial begin
        $dumpfile("Dff.vcd");
        $dumpvars(0, Dff_tb);
        $monitor("t=%0t D=%b Q=%b Qbar=%b clk=%b", $time, d, q, qbar, clk);
        d = 0; #10;
        d = 1; #10;
        d = 0; #10;
        d = 1; #10;

        $finish;
    end
endmodule