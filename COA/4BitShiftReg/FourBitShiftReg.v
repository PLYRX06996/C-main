module dff (input D, input clk, input rst, output reg Q);
    always @(posedge clk or posedge rst) begin
        if (rst) Q <= 0;
        else Q <= D;
    end
endmodule

module FourBitShiftReg (
    input clk, rst, din,
    output [3:0] Q
);
    wire q0, q1, q2, q3;

    dff d0 (.D(din), .clk(clk), .rst(rst), .Q(q0));
    dff d1 (.D(q0), .clk(clk), .rst(rst), .Q(q1));
    dff d2 (.D(q1), .clk(clk), .rst(rst), .Q(q2));
    dff d3 (.D(q2), .clk(clk), .rst(rst), .Q(q3));

    assign Q = {q3, q2, q1, q0};
endmodule