module FourBitAdder (
    input  [3:0] A, 
    input  [3:0] B, 
    input        Cin,  
    output [3:0] Sum,
    output       Cout
);

    wire C0, C1, C2;

    FullAdder FA0 (.A(A[0]), .B(B[0]), .Cin(Cin), .Sum(Sum[0]), .Cout(C0));
    FullAdder FA1 (.A(A[1]), .B(B[1]), .Cin(C0),  .Sum(Sum[1]), .Cout(C1));
    FullAdder FA2 (.A(A[2]), .B(B[2]), .Cin(C1),  .Sum(Sum[2]), .Cout(C2));
    FullAdder FA3 (.A(A[3]), .B(B[3]), .Cin(C2),  .Sum(Sum[3]), .Cout(Cout));

endmodule

module FullAdder (
    input A, B, Cin,
    output Sum, Cout
);
    assign {Cout, Sum} = A + B + Cin;
endmodule
