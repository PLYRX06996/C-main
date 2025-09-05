module Dff (
    input  wire clk,
    input  wire d,
    output reg  q,
    output wire qbar
);

    always @(posedge clk) begin
        q <= d;
    end

    assign qbar = ~q;

endmodule
