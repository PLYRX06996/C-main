module DirectMap(
    input  [10:0] addr,
    input  [7:0]  din,
    input         clk,
    input         we,
    output reg [7:0] dout,
    output reg       hit
);
    reg  [3:0] tag  [0:15];
    reg  [7:0] data [0:15];
    reg        valid[0:15];

    wire [3:0] index;
    wire [2:0] t;

    assign index = addr[3:0];
    assign t     = addr[10:7];

    always @(posedge clk) begin
        if (we) begin
            tag[index]   <= t;
            data[index]  <= din;
            valid[index] <= 1;
        end 
        else begin
            if (valid[index] && tag[index] == t) begin
                dout <= data[index];
                hit  <= 1;
            end 
            else begin
                dout <= 8'b0;
                hit  <= 0;
            end
        end
    end
endmodule