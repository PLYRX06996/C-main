module FA_HA (
    input a,
    input b,
    input ci,
    output s,
    output co
);
    wire s1, c1, c2;

    HA ha1 (
        .a(a),
        .b(b),
        .s(s1),
        .c(c1)
    );

    HA ha2 (
        .a(s1),
        .b(ci),
        .s(s),
        .c(c2)
    );

    assign co = c1 | c2;
endmodule