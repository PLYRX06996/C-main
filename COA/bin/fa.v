module FA(A, B, Cin, Sum, Cout);
    input A, B, Cin;
    output Sum, Cout;
    wire w1, w2, w3;
    assign Sum = A ^ B ^ Cin;
    assign w1 = A & B;
    assign w2 = Cin & B;
    assign w3 = A & Cin;
    assign Cout = w1 | w2 | w3;
endmodule