module MUX4x1 (
    input wire [3:0] I,
    input wire [1:0] S,
    output wire Y
);
    assign Y = (S == 2'b00) ? I[0] :
               (S == 2'b01) ? I[1] :
               (S == 2'b10) ? I[2] :
                              I[3];
endmodule