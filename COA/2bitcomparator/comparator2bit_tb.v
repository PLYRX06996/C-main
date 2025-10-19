`timescale 1ns/1ps
module comparator_2bit_tb;

    reg [1:0] A, B;
    wire A_gt_B, A_eq_B, A_lt_B;

    comparator_2bit uut(.A(A), .B(B), .A_gt_B(A_gt_B), .A_eq_B(A_eq_B), .A_lt_B(A_lt_B));

    initial begin
        $dumpfile("comparator_2bit.vcd");
        $dumpvars(0, comparator_2bit_tb);
        $monitor("t=%0t A=%b B=%b => > %b, = %b, < %b",
                 $time, A, B, A_gt_B, A_eq_B, A_lt_B);

        for (integer i = 0; i < 4; i=i+1)
            for (integer j = 0; j < 4; j=j+1) begin
                A = i; B = j; #10;
            end

        $finish;
    end
endmodule