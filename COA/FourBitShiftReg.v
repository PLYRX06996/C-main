module Dff (input D, input clk, input rst, output reg Q);
    always @(posedge clk or posedge rst) begin
        if(rst) Q <= 0;
        else Q <= D;
    end    
endmodule

module FourBitShiftReg (
    input clk, rst, Din;
    output [3:0] Q
);

wire Q0, Q1, Q2, Q3;

Dff D0(.D(Din), .clk(clk), .rst(rst), Q(Q0));
Dff D1(.D(Q0), .clk(clk), .rst(rst), Q(Q1));
Dff D2(.D(Q1), .clk(clk), .rst(rst), Q(Q2));
Dff D3(.D(Q2), .clk(clk), .rst(rst), Q(Q3));

assign Q = {Q3, Q2, Q1, Q0};    
endmodule