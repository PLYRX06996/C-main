`timescale 1ns/1ps
module memory_cell_tb;
    reg D, Read, Write, Clk;
    wire Out;
    wire Q;

    memory_cell uut (.D(D), .Read(Read), .Write(Write), .Clk(Clk), .Q(Q), .Out(Out));

    initial Clk = 0;
    always #5 Clk = ~Clk;
    initial begin
        $dumpfile("memory_cell.vcd");
        $dumpvars(0, memory_cell_tb);
        $monitor("t=%0t | D=%b Read=%b Write=%b Q=%b Out=%b", $time, D, Read, Write, Q, Out);

        // Initialize
        D=0; Read=0; Write=0; #10;

        D=1; Write=1; #10;
        Write=0; #10;

        Read=1; #10;
        Read=0; #10;

        D=0; Write=1; #10;
        Write=0; #10;

        Read=1; #10;
        Read=0; #10;

        $finish;
    end
endmodule