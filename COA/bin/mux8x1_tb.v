`timescale 1ns/1ps
module mux8x1_tb;
    reg s2, s1, s0, enable;
    reg [7:0] I;
    wire Y;

    mux8x1 uut(.s2(s2), .s1(s1), .s0(s0), .enable(enable), .I(I), .Y(Y));

    initial begin
        s2 = 0; s1 = 0; s0 = 0; 
        enable = 1; 
        I = 8'b0101_0110;
    end

    initial begin
        $dumpfile("mux8x1.vcd");
        $dumpvars(0, mux8x1_tb);
        $monitor("t = %0t sel = %b%b%b I = %b -> Y = %b", $time, s2, s1, s0, I, Y);

        for (integer i = 0; i < 8; i++) begin
            {s2, s1, s0} = i;
            #10;
        end
        $finish;
    end

endmodule