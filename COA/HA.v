module HA (input a, input b, output s, output c);
    xor gate_xor(s, a, b);
    and gate_and(c, a, b);
endmodule