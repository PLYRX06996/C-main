`timescale 1ns/1ps
module fourbitadder_tb;
    reg [3:0] A, B; 
    reg Cin;
    wire [3:0] Sum;
    wire Cout;
    fourbitadder uut(.A(A), .B(B), .Cin(Cin), .Sum(Sum), .Cout(Cout));
    initial begin
        A = 0; B = 0; Cin = 0;
        end

    initial begin

    $dumpfile("fourbitadd.vcd");
    $dumpvars(0, fourbitadder_tb);
    $monitor("t=%0t A=%b B=%b Cin=%b  |  Sum=%b Cout=%b", $time, A, B, Cin, Sum, Cout);

        for(integer i = 0; i < 16; i++) begin
            for(integer j = 0; j < 16; j++)begin
                for(integer k = 0; k < 2; k++)begin
                    A = i; B = j; Cin = k;
                    #5;
                    end
            end
        end

        end
endmodule