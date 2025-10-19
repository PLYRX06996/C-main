`timescale 1ns/1ps
module alu_4bit_tb;
    reg [3:0] A, B;
    reg Cin;
    reg [1:0] S;
    wire [3:0] F;
    wire Cout;

    alu_4bit uut(.A(A), .B(B), .Cin(Cin), .S(S), .F(F), .Cout(Cout));

    initial begin
        $dumpfile("alu_4bit.vcd");
        $dumpvars(0, alu_4bit_tb);
        
        S = 2'b11;
        $display("S = %b (ADD)\n", S);
        $monitor("t=%0t | A=%b B=%b Cin=%b | F=%b Cout=%b", $time, A, B, Cin, F, Cout);

        A = 4'b0000; B = 4'b0000; Cin = 0; #10;
        A = 4'b1010; B = 4'b0101; Cin = 0; #10;
        A = 4'b1111; B = 4'b1010; Cin = 0; #10;
        A = 4'b1100; B = 4'b1111; Cin = 0; #10;
        A = 4'b0110; B = 4'b1001; Cin = 0; #10;
        A = 4'b1111; B = 4'b1111; Cin = 1; #10;

        $finish;
    end

endmodule