`timescale 1ns/1ps
module FA_tb;
    reg A, B, Cin;
    wire Sum, Cout;

    FA uut(.A(A), .B(B), .Cin(Cin), .Sum(Sum), .Cout(Cout));

    initial begin
        A = 0; B = 0; Cin = 0;
        end

    initial begin
        $dumpfile("fa.vcd");
        $dumpvars(0, FA_tb);
        $display("T\tA\tB\tCin\tSum\tCout");
        $monitor("t = %0t A = %b B = %b Cin = %b Sum = %b Cout = %b", $time, A, B, Cin, Sum, Cout);
        for(integer i = 1; i < 8; i++)
        begin
            {A, B, Cin} = i;
            #10;
            end
        end
    endmodule