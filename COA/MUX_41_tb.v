`timescale 1ns/1ps

module MUX4x1_tb;
    reg [3:0] I;
    reg [1:0] S;
    wire Y;

    MUX4x1 uut (.I(I), .S(S), .Y(Y));

    initial begin
        $dumpfile("MUX4x1.vcd");
        $dumpvars(0, MUX4x1_tb);
        $monitor("t=%0t I=%b S=%b -> Y=%b", $time, I, S, Y);

        I = 4'b1010; //inputs

        S = 2'b00; #10;
        S = 2'b01; #10;
        S = 2'b10; #10;
        S = 2'b11; #10;

        $finish;
    end
endmodule