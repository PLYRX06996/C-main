`timescale  1ns/1ps
module ROM4x3_tb;
    reg[1:0] addr;
    wire D2, D1, D0;

    ROM4x3 uut (.addr(addr), .D2(D2), .D1(D1), .D0(D0));

    initial begin
        $dumpfile("ROM4x3.vcd");
        $dumpvars(0, ROM4x3_tb);

        $display("Time  Addr-> D2 D1 D0");
        $monitor("%0t   %b-> %b %b %b", $time, addr, D2, D1, D0);
        for (integer i = 0; i < 4; i++) begin
            addr = i;
            #10;
        end
        $finish;
        end
endmodule