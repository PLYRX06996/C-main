`timescale 1ns/1ps

module x_tb;
    reg  [1:0] S;
    wire [3:0] BUS;
    reg [8*1:0] selName;  // string-like register (8-bit ASCII per char)

    // Instantiate the DUT
    x uut (
        .S(S),
        .BUS(BUS)
    );

    // Local copies (for display only)
    localparam [3:0] A_val = 4'b0001;
    localparam [3:0] B_val = 4'b0010;
    localparam [3:0] C_val = 4'b0100;
    localparam [3:0] D_val = 4'b1000;

    initial begin
        $dumpfile("bus_system.vcd");
        $dumpvars(0, x_tb);

        $display("=== 4-register BUS system simulation ===");
        $display("Registers: A=%b B=%b C=%b D=%b", A_val, B_val, C_val, D_val);
        $display("S  : selects  |   BUS[3:0]");
        $display("---------------------------------");

        // Initial selection
        S = 2'b00; updateSelName; #10;
        S = 2'b01; updateSelName; #10;
        S = 2'b10; updateSelName; #10;
        S = 2'b11; updateSelName; #10;

        $finish;
    end

    task updateSelName;
        begin
            case (S)
                2'b00: selName = "A";
                2'b01: selName = "B";
                2'b10: selName = "C";
                2'b11: selName = "D";
            endcase
            #1 $display("%b : %s | %b", S, selName, BUS);
        end
    endtask

endmodule