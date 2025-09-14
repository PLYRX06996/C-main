`timescale 1ns/1ps
module FourBitAdder_tb;

    reg  [3:0] A, B;
    reg  Cin;
    wire [3:0] Sum;
    wire Cout;

    FourBitAdder UUT (.A(A), .B(B), .Cin(Cin), .Sum(Sum), .Cout(Cout));

    initial begin
        $dumpfile("FourBitAdder.vcd");
        $dumpvars(0, FourBitAdder_tb);
        $monitor("t=%0t A=%b B=%b Cin=%b  |  Sum=%b Cout=%b", $time, A, B, Cin, Sum, Cout);
        
        A=4'b0101; B=4'b0011; Cin=0; #10; 
        A=4'b0101; B=4'b0011; Cin=1; #10; 
        A=4'b1111; B=4'b0001; Cin=0; #10;
        A=4'b1010; B=4'b0101; Cin=0; #10;

        $finish;
    end
endmodule
