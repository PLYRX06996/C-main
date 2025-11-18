module memory_cell(
    input D, Read, Write, Clk, Rst,
    output reg Q,
    output Out
);
    always @(posedge Clk or posedge Rst) begin
        if (Rst)
            Q <= 0;
        else if (Write)
            Q <= D;
    end

    assign Out = (Read) ? Q : 1'bz;
endmodule
