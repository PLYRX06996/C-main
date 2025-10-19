module x(
    input clk,
    input reset,
    output q1, q2, q3
);

    reg q1, q2, q3_reg;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            q1 <= 0;
            q2 <= 0;
            q3_reg <= 0;
        end else begin
            q1 <= ~q1;

            if (q1)
                q2 <= ~q2;

            if (q1 & q2)
                q3_reg <= ~q3_reg;
        end
    end

    assign q3 = q3_reg;

endmodule