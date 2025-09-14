module T_FF ( 
    input wire clk,
    input wire rst,
    input wire T,
    output reg Q
);
    always @(posedge clk or posedge rst) begin
        if(rst)
        Q <= 0;
        else if(T)
        Q <= ~Q;
    end
    
endmodule