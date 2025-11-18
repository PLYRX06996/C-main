`timescale 1ns/1ps
module FourBitALU_tb;
    reg [3:0] A, B;
    reg S1, S0, Cin;
    wire [3:0] D;
    wire Cout;

    FourBitALU uut(.A(A), .B(B), .S1(S1), .S0(S0), .Cin(Cin), .D(D), .Cout(Cout));

    initial begin
        $dumpfile("FourBitALU.vcd");
        $dumpvars(0, FourBitALU_tb);
        $monitor("t=%0t A=%b B=%b S1=%b S0=%b Cin=%b -> D=%b Cout=%b", 
                  $time, A, B, S1, S0, Cin, D, Cout);

        A = 4'b0101; B = 4'b0011;
        //Cases: 
        S1=0; S0=0; Cin=0; #10; // Add
        S1=0; S0=0; Cin=1; #10; // Add with carry
        S1=0; S0=1; Cin=0; #10; // Subtract with borrow
        S1=0; S0=1; Cin=1; #10; // Subtract
        S1=1; S0=0; Cin=0; #10; // Transfer A
        S1=1; S0=0; Cin=1; #10; // Increment A
        S1=1; S0=1; Cin=0; #10; // Decrement A
        S1=1; S0=1; Cin=1; #10; // Transfer A again

        $finish;
    end
endmodule