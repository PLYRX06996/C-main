`timescale 1ns/1ps

module DirectMap_tb;
    reg        clk;
    reg        rst;
    reg        read;
    reg        we;
    reg  [10:0] addr;
    reg  [7:0]  din;
    wire [7:0]  dout;
    wire        hit;

    DirectMap dut (
        .addr(addr),
        .din(din),
        .clk(clk),
        .we(we),
        .dout(dout),
        .hit(hit)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    function [10:0] make_addr;
        input [3:0] tag;
        input [3:0] index;
        begin
            make_addr = (tag << 7) | index;
        end
    endfunction

    task show;
        input [10:0] a;
        input [7:0] w;
        input r;
        input wr;
        begin
            #1;
            $display("%0t | addr=%0d tag=%b idx=%b | we=%b read=%b | hit=%b dout=%02h written=%02h",
                     $time, a, a[10:7], a[6:0], wr, r, hit, dout, w);
        end
    endtask

    initial begin
        $dumpfile("DirectMap.vcd");
        $dumpvars(0, DirectMap_tb);

        $display("\nDirectMap testbench");
        $display("Registers: 16 lines, tag=4 bits, index=7 bits(used partly), data=8 bits");
        $display("Time | addr tag idx | we read | hit dout written");
        $display("------------------------------------------------");

        rst = 1; read = 0; we = 0; addr = 0; din = 0;
        #12;
        rst = 0;
        #8;

       we = 1; read = 0;
        addr = make_addr(4'b0001, 4'd0); din = 8'hA1; show(addr,din,read,we); #10;
        addr = make_addr(4'b0010, 4'd1); din = 8'hB2; show(addr,din,read,we); #10;
        addr = make_addr(4'b0011, 4'd2); din = 8'hC3; show(addr,din,read,we); #10;
        addr = make_addr(4'b0100, 4'd3); din = 8'hD4; show(addr,din,read,we); #10;
        we = 0;

        read = 1; din = 8'h00;

        addr = make_addr(4'b0001, 4'd0); show(addr,din,read,we); #10;
        addr = make_addr(4'b0010, 4'd1); show(addr,din,read,we); #10;

        addr = make_addr(4'b1111, 4'd4); show(addr,din,read,we); #10;

        we = 1; read = 0;
        addr = make_addr(4'b1001, 4'd0); din = 8'h9A; show(addr,din,read,we); #10;
        we = 0; read = 1;
        addr = make_addr(4'b0001, 4'd0); show(addr,din,read,we); #10;

        addr = make_addr(4'b1001, 4'd0); show(addr,din,read,we); #10;

        $finish;
    end

endmodule
