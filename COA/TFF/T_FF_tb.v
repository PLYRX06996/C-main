`timescale 1ns/1ps

module T_FF_tb;
    reg clk, rst, T;
    wire Q;

    T_FF uut (.clk(clk), .rst(rst), .T(T), .Q(Q));

    always #5 clk = ~clk;

    initial begin
        $dumpfile("T_FF.vcd");
        $dumpvars(0, T_FF_tb);
        $monitor("t=%0t clk=%b rst=%b T=%b -> Q=%b", $time, clk, rst, T, Q);

        //Initialization
        clk = 0;
        rst = 1;
        T = 0;
        #10;
        rst =0;
    
        //Toggle behav
        T = 1;
        #40;
        T = 0;
        #20;
        T = 1;
        #30;

        $finish;
    end    
endmodule