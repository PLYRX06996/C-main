`timescale 1ns/1ps

module HA_tb;
    reg a, b;           // inputs to HA
    wire s, c;          // outputs from HA
    integer i;
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

    for (i = 0; i < 4; i++) begin
            {a, b} = i;
            #10;
        end

    $finish;
end
endmodule