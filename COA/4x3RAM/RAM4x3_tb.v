`timescale 1ns/1ps

module RAM4x3_tb;
    reg [1:0] Addr;
    reg [2:0] DataIn;
    reg Write, Clk;
    wire [2:0] DataOut;

    RAM4x3 uut (.Addr(Addr), .DataIn(DataIn), .Write(Write), .Clk(Clk), .DataOut(DataOut));

    initial begin
        Clk = 0;
        forever #5 Clk = ~Clk;
    end

    initial begin
        $dumpfile("RAM4x3.vcd");
        $dumpvars(0, RAM4x3_tb);

        $display("Time\tClk\tWrite\tAddr\tDataIn\tDataOut");
        $monitor("%0t\t%b\t%b\t%02b\t%03b\t%03b", $time, Clk, Write, Addr, DataIn, DataOut);

        Write = 1;
        Addr = 2'b00; DataIn = 3'b101; #10;
        Addr = 2'b01; DataIn = 3'b010; #10;
        Addr = 2'b10; DataIn = 3'b111; #10;
        Addr = 2'b11; DataIn = 3'b000; #10;

        Write = 0;
        Addr = 2'b00; #10;
        Addr = 2'b01; #10;
        Addr = 2'b10; #10;
        Addr = 2'b11; #10;

        $finish;
    end
endmodule
