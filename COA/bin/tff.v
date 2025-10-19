module tff (
    input clk,
    input T,
    output reg q
);

    always @(posedge clk)begin
        if(T)
            q <= ~q;
            else
            q <= q;
    end
endmodule