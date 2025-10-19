`timescale 1ns/1ps
module alu_4bit_struct_tb;
reg [3:0] A, B;
reg [1:0] S;
reg Cin;
wire [3:0] F;
wire Cout;

alu_4bit_struct uut (.A(A), .B(B), .Cin(Cin), .S(S), .F(F), .Cout(Cout));

initial begin
    $dumpfile("alu_4bit_struct.vcd");
    $dumpvars(0, alu_4bit_struct_tb);
    S = 2'b11;
    $display("S = %b (ADD)\n", S);
    Cin = 0;
    $monitor("t=%0t | A=%b B=%b Cin=%b S=%b | F=%b Cout=%b", $time, A, B, Cin, S, F, Cout);
        
        A=4'b0001; B=4'b0011; Cin=0; #10;
        A=4'b0101; B=4'b1010; Cin=0; #10;
        A=4'b1111; B=4'b0001; Cin=0; #10;
        A=4'b1000; B=4'b0111; Cin=0; #10;
        A=4'b1111; B=4'b1111; Cin=1; #10;
        $finish;

end
endmodule