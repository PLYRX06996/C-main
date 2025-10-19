`timescale 1ns/1ps
module x_tb;
    reg clk, reset;
    wire q1, q2, q3;

    x uut(.clk(clk), .reset(reset), .q1(q1), .q2(q2), .q3(q3));

    initial clk = 0;
    always #10 clk = ~clk;

    initial begin
        $dumpfile("x.vcd");
        $dumpvars(0, x_tb);
        $monitor("t=%0t clk=%b reset=%b q1 = %b q2=%b q3 = %b ", $time, clk, reset, q1, q2, q3);

        reset = 1; #10; reset = 0;
        #200;
        $finish;
    end
endmodule