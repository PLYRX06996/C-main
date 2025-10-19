`timescale 1ns/1ps
module alu_1bit_tb;
    reg A, B, Cin;
    reg [1:0] S;
    wire F, Cout;

    alu_1bit uut(.A(A), .B(B), .Cin(Cin), .S(S), .F(F), .Cout(Cout));

    initial begin
        $dumpfile("alu_1bit.vcd");
        $dumpvars(0, alu_1bit_tb);
        S = 2'b11;
        $display("S = %b (ADD)\n", S);
        $monitor("t=%0t | A=%b B=%b Cin=%b | F=%b Cout=%b", $time, A, B, Cin, F, Cout);

        A=0; B=0; Cin=0; #10;
        A=0; B=1; Cin=0; #10;
        A=1; B=0; Cin=1; #10;
        A=1; B=1; Cin=0; #10;
        A=1; B=1; Cin=1; #10;
        $finish;
    end
endmodule