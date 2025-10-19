`timescale 1ns/1ps
module tff_tb;
    reg clk, T;
    wire q, qbar;

    tff uut(.clk(clk), .T(T), .q(q));

    initial clk = 0;
    always #5 clk = ~clk;

    assign qbar = ~q;
    initial begin
        $dumpfile("tff.vcd");
        $dumpvars(0, tff_tb);
        $monitor("t = %0t clk = %b T = %b -> q = %b qbar = %b", $time, clk, T, q, qbar);
        T = 0;
        for(integer i = 0; i < 8; i++)begin
            #10;
            T = ~T;
        end
        $finish;
    end
endmodule