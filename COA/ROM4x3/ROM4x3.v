module ROM4x3 (
    input [1:0] addr,
    output D1, D2, D0
);

    wire [3:0] w;
    assign w[0] = (~addr[1]) & (~addr[0]);
    assign w[1] = (~addr[1]) & (addr[0]);
    assign w[2] = (addr[1]) & (~addr[0]);
    assign w[3] = (addr[1]) & (addr[0]);

    assign D2 = w[0] | w[1] | w[3];
    assign D1 = w[1] | w[2];
    assign D0 = w[0] | w[2];
    
endmodule