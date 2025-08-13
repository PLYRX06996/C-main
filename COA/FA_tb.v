`timescale 1ns/1ps

module FA_tb;
    reg a, b, ci;       // inputs
    wire s, co;         // outputs

    // Instantiate the FA module
    FA uut (
        .a(a),
        .b(b),
        .ci(ci),
        .s(s),
        .co(co)
    );

    initial begin
        // Dump waveform data
        $dumpfile("fa_wave.vcd");
        $dumpvars(0, FA_tb);
        
        // Monitor changes
        $monitor("t=%0t a=%b b=%b ci=%b -> s=%b co=%b", 
                $time, a, b, ci, s, co);
        
        // Test all 8 possible input combinations
        a=0; b=0; ci=0; #10;
        a=0; b=0; ci=1; #10;
        a=0; b=1; ci=0; #10;
        a=0; b=1; ci=1; #10;
        a=1; b=0; ci=0; #10;
        a=1; b=0; ci=1; #10;
        a=1; b=1; ci=0; #10;
        a=1; b=1; ci=1; #10;
        
        $finish;
    end
endmodule