`include "alu_1bit.v"

module alu_4bit_struct (
    input [3:0] A, B,
    input [1:0] S,
    input Cin,
    output [3:0] F,
    output Cout
);
    wire [2:0] carry;

    alu_1bit alu0 (.A(A[0]), .B(B[0]), .Cin(Cin), .S(S), .F(F[0]), .Cout(carry[0]));
    alu_1bit alu1 (.A(A[1]), .B(B[1]), .Cin(carry[0]), .S(S), .F(F[1]), .Cout(carry[1]));
    alu_1bit alu2 (.A(A[2]), .B(B[2]), .Cin(carry[1]), .S(S), .F(F[2]), .Cout(carry[2]));
    alu_1bit alu3 (.A(A[3]), .B(B[3]), .Cin(carry[2]), .S(S), .F(F[3]), .Cout(Cout));
    
endmodule