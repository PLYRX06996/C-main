module RAM4x3 (
    input [1:0] Addr,
    input [2:0] DataIn,
    input Write,
    input Clk,
    output reg [2:0] DataOut
);
    reg [2:0] mem [3:0];

    always @(posedge Clk) begin
        if (Write)
            mem[Addr] <= DataIn;
    end

    always @(*) begin
        if (!Write)
            DataOut = mem[Addr];
        else
            DataOut = 3'bzzz;
    end
endmodule