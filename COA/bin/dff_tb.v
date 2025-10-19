`timescale 1ns/1ps
module dff_tb;
    reg d, clk;
    wire q, qbar;

    dff uut(.d(d), .clk(clk), .q(q), .qbar(qbar));

    initial clk = 0;
    always #5 clk = ~clk;

    initial begin
        $dumpfile("dff.vcd");
        $dumpvars(0, dff_tb);
        $monitor("t = %0t clk = %b d = %b -> q = %b qbar = %b", $time, clk, d, q, qbar);

        d = 0;

        for(integer i = 0; i < 8; i++)begin
            #10; d = ~d;
        end
        #10;
        $finish;
    end

endmodule