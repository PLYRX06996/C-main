`timescale 1ns/1ps

module HA_tb;
    reg a, b;           // inputs to HA
    wire s, c;          // outputs from HA

    // Instantiate the HA module
    HA uut (
        .a(a),
        .b(b),
        .s(s),
        .c(c)
    );

    initial begin
    $dumpfile("wave.vcd");
    $dumpvars(0, HA_tb);  // Dump all signals in the testbench
    $monitor("t=%0t a=%b b=%b -> s=%b c=%b", $time, a, b, s, c);

    a = 0; b = 0; #10;

    a = 0; b = 1; #10;

    a = 1; b = 0; #10;

    a = 1; b = 1; #10;

    $finish;
end
endmodule