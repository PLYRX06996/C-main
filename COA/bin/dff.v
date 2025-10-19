module dff (
    input d,
    input clk,
    output reg q,
    output wire qbar
);

    always @(posedge clk)begin
        q <= d;
    end
    assign qbar = ~q;
    
endmodule