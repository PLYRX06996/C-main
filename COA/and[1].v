// `timescale 1ns/1ps

//----------------and gate

// module and_gate(
//     input wire a,
//     input wire b,
//     output wire y
// );
//     assign y = a & b;
// endmodule

// module tb_and;
//     reg a;
//     reg b;
//     wire y;

//     and_gate rx(.a(a), .b(b), .y(y));
//     integer i;

//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_and);

//         for (i = 0; i<4; i = i + 1) begin
//             {a, b} = i;   
//             #10;    
//             $display("A=%b , B=%b , Y=%b", a, b, y);
//         end
//         $finish;
//     end
// endmodule



// ---------HALF ADDER-----------//

// module half_adder(input  a, input b, output sum, output carry);
//     assign carry=a&b;
//     assign sum=a^b;
// endmodule

// module tb_ha;
//     reg a;
//     reg b;
//     wire sum;
//     wire carry;
//     half_adder rx(.a(a),.b(b),.sum(sum),.carry(carry));
//     integer i;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_ha);
//         for(i=0;i<4;i++)begin
//         {a,b}=i;
//         #5
//         $display("A=%b , B=%b , SUM=%b , CARRY=%b",a,b,sum,carry);
//         end
//         $finish;
//     end
// endmodule


//----------Full Adder

// module full_adder(input a,input b, input cin, output s, output c);
//     assign s=a^b^cin;
//     assign c=(a&b)|((a^b)&cin);
// endmodule

// module tb_fa;
//     reg a,b,cin;
//     wire c,s;
//     full_adder rx(.a(a), .b(b), .cin(cin), .s(s),.c(c));
//     integer m,n,p;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_fa);
//         for(m=0;m<2;m=m+1)    begin
//             for(n=0;n<2;n=n+1)    begin
//                 for(p=0;p<2;p=p+1)    begin
//                     a=m;b=n;cin=p;
//                     #5
//                     $display("A=%b , B=%b , CIN=%b , SUM=%b , CARRY=%b",a,b,cin,s,c);
//                     end end end
//                     $finish;
//                 end
//             endmodule


//------------full adder using half adder



// module half_adder(input  a, input b, output sum, output carry);
//     assign carry=a&b;
//     assign sum=a^b;
// endmodule


// module full_adder(input a,input b,input cin,output s,output c);
//     wire t1,t2,t3;
//     half_adder r(a,b,t1,t2);
//     half_adder f(t1,cin,s,t3);
//     assign c=t2|t3;
// endmodule

// module tb_fa_using_ha;
//     reg a,b,cin;
//     wire sum,cout;
//     full_adder rx(.a(a),.b(b),.cin(cin),.s(sum),.c(cout));
//     integer m,n,p;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_fa_using_ha);
//         for(m=0;m<2;m++)    begin
//             for(n=0;n<2;n++)    begin
//                 for(p=0;p<2;p++)    begin
//                     a=m;b=n;cin=p;
//                     #5
//                     $display("A=%b , B=%b , CIN=%b , SUM=%b , CARRY=%b",a,b,cin,sum,cout);
//                 end
//             end
//         end
//         $finish;
//     end
// endmodule





//-------------4 BIT BINARY ADDER

// module full_adder(
//     input  a, b, cin,
//     output sum, carry
// );
//     assign sum   = a ^ b ^ cin;
//     assign carry = (a & b) | ((a ^ b) & cin);
// endmodule


// module binary_adder_4bit(
//     input  a0, a1, a2, a3,
//     input  b0, b1, b2, b3,
//     input  cin,
//     output s0, s1, s2, s3,
//     output cout
// );
//     wire c1, c2, c3; // Internal carries

//     full_adder fa0(.a(a0), .b(b0), .cin(cin), .sum(s0), .carry(c1));
//     full_adder fa1(.a(a1), .b(b1), .cin(c1),  .sum(s1), .carry(c2));
//     full_adder fa2(.a(a2), .b(b2), .cin(c2),  .sum(s2), .carry(c3));
//     full_adder fa3(.a(a3), .b(b3), .cin(c3),  .sum(s3), .carry(cout));
// endmodule


// module tb_binary_adder_4bit;
//     reg a0,a1,a2,a3;
//     reg b0,b1,b2,b3;
//     reg cin;
//     wire s0,s1,s2,s3,cout;

//     binary_adder_4bit uut(
//         .a0(a0), .a1(a1), .a2(a2), .a3(a3),
//         .b0(b0), .b1(b1), .b2(b2), .b3(b3),
//         .cin(cin),
//         .s0(s0), .s1(s1), .s2(s2), .s3(s3),
//         .cout(cout)
//     );

//     integer i;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_binary_adder_4bit);

//         // Test all input combinations
//         for (i = 0; i < 512; i = i + 1) begin
//             {cin,a3,a2,a1,a0,b3,b2,b1,b0} = i;
//             #5;
//             $display("A=%b%b%b%b B=%b%b%b%b Cin=%b | Sum=%b%b%b%b Cout=%b",
//                       a3,a2,a1,a0, b3,b2,b1,b0, cin, s3,s2,s1,s0, cout);
//         end
//         $finish;
//     end
// endmodule



//-----------D flip flop

// module d_ff(input q,input clk, output reg d);
//     always @(posedge clk)   begin
//         d<=q;
//     end
// endmodule

// module tb_d_ff;
//     reg q,clk;
//     wire d;
//     d_ff rx(.q(q),.clk(clk),.d(d));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_d_ff);
//         clk=0;
//         forever #8 clk= ~clk;
//     end
//     initial begin
//         q=0;
//         #5 q=1;
//         #7 q=0;
//         #9 q=1;
//         #15 q=0;
//         #20 $finish;
//     end
// endmodule


//------------2x4 Decoder

// module decoder(input a, input b,output y0,output y1,output y2,output y3);
//     assign y0= (~a) & (~b);
//     assign y1=(~a) & b;
//     assign y2=a&(~b);
//     assign y3=a&b;
// endmodule

// module tb_dc;
//     reg a,b;
//     wire y0,y1,y2,y3;
//     decoder rx(.a(a),.b(b),.y0(y0),.y1(y1),.y2(y2),.y3(y3));
//     integer i;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_dc);
//         $display(" A  |  B  |  Y0  |  Y1  |  Y2  |  Y3  |");
//         for(i=0;i<4;i++) begin
//             {a,b}=i;
//             #5
//             $display("  %b  |  %b  |  %b  |  %b  |  %b  |  %b  |",a,b,y0,y1,y2,y3);
//         end
//         $finish;
//     end
// endmodule


//----------4X1 mux

// module mux(input s1,input s0,input y0,input y1,input y2,input y3,output y);
//     assign y= (y0&(~s1)&(~s0)) | (y1&(~s1)&(s0)) | (y2&(s1)&(~s0)) | (y3&(s1)&(s0));
// endmodule

// module tb_mux;
//     reg s0,s1,y0,y1,y2,y3;
//     wire y;
//     mux rx(.s0(s0),.s1(s1),.y0(y0),.y1(y1),.y2(y2),.y3(y3),.y(y));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_mux);
//         $display("S1 S0 | I0 I1 I2 I3 | Y");
//         $monitor("%b %b | %b %b %b %b | %b", s1, s0, y0, y1,
// y2, y3, y);
//         y0=1;y1=0;y2=0;y3=0;s1=0;s0=0;
//         #10;
//         y0=0;y1=1;y2=0;y3=0;s1=0;s0=1;
//         #10;
//         y0=0;y1=0;y2=1;y3=0;s1=1;s0=0;
//         #10;
//         y0=0;y1=0;y2=0;y3=1;s1=1;s0=1;
//         #10;
//         $finish;
//     end
// endmodule


//---------T flip flop

// module t_ff(input t,input clk,output reg q);
//     always @(posedge clk) begin
//         if(t) begin   q<=~q; end
//         else begin   q<=q; end
//     end
// endmodule

// module tb_t_ff;
//     reg t,clk;
//     wire q;
//     t_ff rx(.t(t),.clk(clk),.q(q));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_t_ff);
//         clk=0;
//         forever #5 clk=~clk;
//     end
//     initial begin
//         t=0;
//         #6; t=1;
//         #8; t=0;
//         #16; t=1;
//         #16; t=0;
//         #20; t=1;
//         $finish;
//     end
// endmodule


//----------4X3 rom

// module rom(input s0,input s1,output reg[2:0] o);
//     always @(*) begin
//         if(s0==0 && s1==0)begin
//             o = 3'b101;
//         end
//         if(s0==1 && s1==0)begin
//             o = 3'b100;
//         end
//         if(s0==0 && s1==1)begin
//             o = 3'b011;
//         end
//         if(s0==1 && s1==1)begin
//             o = 3'b010;
//         end
//     end
// endmodule
// module rom_tb();
//     reg s0,s1;
//     wire[2:0] o;
//     rom rx(.s0(s0),.s1(s1),.o(o));
//     integer i;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,rom_tb);
//         for(i=0;i<4;i++)begin
//             {s1,s0} = i;
//             #5
//             $display("S1 = %b | S0 = %b | O = %b",s1,s0,o);
//         end
//         $finish;
//     end
// endmodule


//----------4 bit shift register

// module siso(input SI,input clk,output SO);
//     reg [3:0]q;
//     always @(posedge clk) begin
//         q<={q[2:0],SI};
//     end
//     assign SO=q[3];
// endmodule

// module tb_siso();
//     reg clk,SI;
//     wire SO;
//     siso rx(.SI(SI),.clk(clk),.SO(SO));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_siso);
//         clk=0;
//         forever #5 clk=~clk;
//     end
//     initial begin
//         $monitor("Time=%0t SI=%b SO=%b", $time, SI, SO);
//         SI=0;
//         #7 SI=1;
//         #10 SI = 0;
//         #10 SI = 1;
//         #10 SI = 1;
//         #10 SI=0;
//         #10 SI=1;
//         #70 $finish;
//     end
// endmodule


//----------- sipo 4 bit

// module sipo(input SI,input clk,output [3:0]PO);
//     reg [3:0]q;
//     always @(posedge clk) begin
//         q<={q[2:0],SI};
//     end
//     assign PO[3:0]=q[3:0];
// endmodule

// module tb_sipo();
//     reg SI,clk;
//     wire [3:0]PO;
//     sipo rx(.SI(SI),.clk(clk),.PO(PO));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_sipo);
//         clk=0;
//         forever #5 clk=~clk;
//     end
//     initial begin
//         SI = 1; #10;
//         SI = 0; #10;
//         SI = 1; #10;
//         SI = 1; #10;
//         $finish;
//     end
//     initial begin
//         $monitor("Time=%0t SI=%b PO=%b", $time, SI, PO);
//     end
// endmodule


//---------- PISO 

// module piso(input [3:0]PI,input load,input clk,output SO);
//     reg [3:0]q;
//     always @(posedge clk) begin
//         if(load)    q<=PI;
//         else    q<={1'b0,q[3:1]};
//     end
//     assign SO=q[0];
// endmodule

// module tb_piso();
//     reg clk,load;
//     reg [3:0]PI;
//     output SO;
//     piso rx(.PI(PI),.load(load),.clk(clk),.SO(SO));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_piso);
//         clk=0;
//         forever #5 clk=~clk;
//     end
//     initial begin
//         PI = 4'b1011; load = 1; #10;  // Load parallel data
//         load = 0; #10;                // Start shifting
//         #40 $finish;
//     end
//     initial begin
//         $monitor("Time=%0t LOAD=%b PI=%b SO=%b", $time, load, PI, SO);
//     end
// endmodule


//------------PIPO

// module pipo(input [3:0]PI,output [3:0]PO,input clk,input load);
//     reg [3:0] d;
//     always @(posedge clk) begin
//         if(load)    d=PI;
//     end
//     assign PO=d;
// endmodule

// module tb_pipo();
//     reg load,clk;
//     reg [3:0] PI;
//     wire [3:0] PO;
//     pipo rx(.PI(PI),.PO(PO),.clk(clk),.load(load));
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0,tb_pipo);
//         clk=0;
//         forever #5 clk=~clk;
//     end
//     initial begin
//         PI = 4'b1011; load = 1; #10;
//         load = 0; #10;
//         PI=4'b1010; load=1;#10;
//         load=0;          
//         #40 $finish;
//     end
//     initial begin   $monitor("Time=%0t LOAD=%b PI=%b PO=%b", $time, load, PI, PO);
//     end
// endmodule

//------------MEMORY CELL

// module memory_cell (
//     input wire clk,
//     input wire W_en,
//     input wire R_en,
//     input wire D_in,
//     output wire D_out
// );
//     reg Q;
//     wire D_mux = (W_en) ? D_in : Q;
//     always @(posedge clk) begin
//         Q <= D_mux;
//     end
//     assign D_out = Q & R_en;
// endmodule


// module tb_memory_cell;
//     reg clk = 0;
//     reg W_en;
//     reg R_en;
//     reg D_in;
//     wire D_out;
//     memory_cell uut (
//         .clk(clk),
//         .W_en(W_en),
//         .R_en(R_en),
//         .D_in(D_in),
//         .D_out(D_out)
//     );
//     always #5 clk = ~clk;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_memory_cell);

//         $monitor("%t | W=%b R=%b D_in=%b D_out=%b", 
//                   $time, W_en, R_en, D_in, D_out);

//         W_en = 0; R_en = 0; D_in = 0;
//         #10;

//         W_en = 1; D_in = 1;
//         #10;
//         W_en = 0;

//         R_en = 1;
//         #10;
//         R_en = 0;

//         W_en = 1; D_in = 0;
//         #10;
//         W_en = 0;

//         R_en = 1;
//         #10;
//         R_en = 0;

//         #20;

//         $finish;
//     end
// endmodule


//------------------4X3 RAM 

// module ram(
//     input clk,
//     input w_en,
//     input r_en,
//     input [1:0] addr,
//     input [2:0] d_in,
//     output [2:0] d_out
// );

//     reg Q;
//     reg [2:0] mem [0:3];
//     always @(posedge clk) begin
//         if(w_en)    mem[addr]<=d_in;
//         if(r_en)    Q<=mem[addr];
//     end
//     assign d_out=(r_en)?Q:3'b000;
// endmodule

// module tb_ram;
//     reg clk;
//     reg w_en;
//     reg r_en;
//     reg [1:0] addr;
//     reg [2:0] d_in;
//     wire [2:0] d_out;

//     ram uut (
//         .clk(clk),
//         .w_en(w_en),
//         .r_en(r_en),
//         .addr(addr),
//         .d_in(d_in),
//         .d_out(d_out)
//     );

//     always #5 clk = ~clk;

//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_ram);
//         clk = 0;
//         w_en = 0;
//         r_en = 0;
//         addr = 2'b00;
//         d_in = 3'b000;

//         // Write data to address 0
//         #10 w_en = 1; addr = 2'b00; d_in = 3'b101;
//         #10 w_en = 0;

//         // Write data to address 1
//         #10 w_en = 1; addr = 2'b01; d_in = 3'b110;
//         #10 w_en = 0;

//         // Read data from address 0
//         #10 r_en = 1; addr = 2'b00;
//         #10 r_en = 0;

//         // Read data from address 1
//         #10 r_en = 1; addr = 2'b01;
//         #10 r_en = 0;

//         #20 $finish;
//     end
// endmodule


//-----------------1 bit ALU

// module alu_1bit(
//     input a,
//     input b,
//     input cin,
//     input [1:0] sel,
//     output reg result,
//     output reg cout
// );
//     always @(*) begin
//         case(sel)
//             2'b00: begin // AND
//                 result = a & b;
//                 cout = 1'b0;
//             end
//             2'b01: begin // OR
//                 result = a | b;
//                 cout = 1'b0;
//             end
//             2'b10: begin
//                 result=a^b^cin;
//                 cout=(a&b)|((a^b)&cin);
//             end
//             2'b11: begin // subtraction
//                 result = a ^ b ^ 1'b1;
//                 cout = (~a & b) | ((~(a ^ b)) & 1'b1);
//             end
//             default: begin
//                 result = 1'b0;
//                 cout = 1'b0;
//             end
//         endcase
//     end
// endmodule   

// module tb_alu_1bit;
//     reg a;
//     reg b;
//     reg cin;
//     reg [1:0] sel;
//     wire result;
//     wire cout;

//     alu_1bit uut (
//         .a(a),
//         .b(b),
//         .cin(cin),
//         .sel(sel),
//         .result(result),
//         .cout(cout)
//     );

//     integer i;
//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_alu_1bit);
//         $display("A B CIN SEL | RESULT COUT");
//         for (i = 0; i < 32; i = i + 1) begin
//             {a, b, cin, sel} = i;
//             #10;
//             $display("%b %b  %b  %b  |   %b     %b", a, b, cin, sel, result, cout);
//         end
//         $finish;
//     end
// endmodule

//-----------------------4bit ALU using 1 bit ALU

// module alu1bit(
//     input wire A,
//     input wire B,
//     input wire Cin,
//     input wire [1:0] op,
//     output wire R,
//     output wire Cout
// );

//     wire and_r = A & B;
//     wire or_r  = A | B;
//     wire xor_r = A ^ B;
//     wire sum   = A ^ B ^ Cin;
//     wire c_out = (A & B) | (A & Cin) | (B & Cin);

//     assign R = (op == 2'b00) ? and_r :
//                (op == 2'b01) ? or_r  :
//                (op == 2'b10) ? sum   :
//                xor_r;

//     assign Cout = (op == 2'b10) ? c_out : 1'b0;

// endmodule

// module alu4bit(
//     input wire [3:0] A,
//     input wire [3:0] B,
//     input wire [1:0] op,
//     output wire [3:0] R,
//     output wire Cout
// );

//     wire c1, c2, c3;

//     alu1bit u0(A[0], B[0], 1'b0, op, R[0], c1);
//     alu1bit u1(A[1], B[1], c1 , op, R[1], c2);
//     alu1bit u2(A[2], B[2], c2 , op, R[2], c3);
//     alu1bit u3(A[3], B[3], c3 , op, R[3], Cout);

// endmodule


// module tb_alu4bit;

//     reg clk = 0;
//     reg [3:0] A;
//     reg [3:0] B;
//     reg [1:0] op;
//     wire [3:0] R;
//     wire Cout;

//     alu4bit uut(
//         .A(A),
//         .B(B),
//         .op(op),
//         .R(R),
//         .Cout(Cout)
//     );

//     always #5 clk = ~clk;

//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_alu4bit);

//         $monitor("%t | A=%b B=%b op=%b R=%b Cout=%b",
//                   $time, A, B, op, R, Cout);

//         A = 4'b1100;  B = 4'b1010; op = 2'b00;
//         #20;

//         A = 4'b1100;  B = 4'b1010; op = 2'b01;
//         #20;

//         A = 4'b1111;  B = 4'b0001; op = 2'b10;
//         #20;

//         A = 4'b1111;  B = 4'b0101; op = 2'b11;
//         #20;

//         $finish;
//     end

// endmodule


//----------------Direct Mapping

// module direct_cache(
//     input wire clk,
//     input wire read,
//     input wire [8:0] address,
//     input wire [7:0] mem_data,
//     output wire [7:0] data_out,
//     output wire hit
// );

//     wire [3:0] tag = address[8:5];
//     wire [2:0] index = address[4:2];
//     wire [1:0] word = address[1:0];

//     reg [3:0] tag_array [0:7];
//     reg valid_array [0:7];
//     reg [7:0] data_array [0:7][0:3];

//     reg [7:0] data_reg;
//     reg hit_reg;

//     always @(posedge clk) begin
//         if (read) begin
//             if (valid_array[index] && tag_array[index] == tag) begin
//                 data_reg <= data_array[index][word];
//                 hit_reg <= 1;
//             end else begin
//                 data_array[index][word] <= mem_data;
//                 tag_array[index] <= tag;
//                 valid_array[index] <= 1;
//                 data_reg <= mem_data;
//                 hit_reg <= 0;
//             end
//         end
//     end

//     assign data_out = data_reg;
//     assign hit = hit_reg;

// endmodule


// module tb_direct_cache;

//     reg clk = 0;
//     reg read;
//     reg [8:0] address;
//     reg [7:0] mem_data;
//     wire [7:0] data_out;
//     wire hit;

//     direct_cache uut(
//         .clk(clk),
//         .read(read),
//         .address(address),
//         .mem_data(mem_data),
//         .data_out(data_out),
//         .hit(hit)
//     );

//     always #5 clk = ~clk;

//     initial begin
//         $dumpfile("wave.vcd");
//         $dumpvars(0, tb_direct_cache);

//         $monitor("%t | ADR=%b DATA=%b HIT=%b OUT=%b",
//                  $time, address, mem_data, hit, data_out);

//         read = 1;

//         address = 9'b000100010; mem_data = 8'hAA;
//         #10;

//         address = 9'b000100010; mem_data = 8'hAA;
//         #10;

//         address = 9  'b001000010; mem_data = 8'hBB;
//         #10;

//         address = 9'b001000010; mem_data = 8'hBB;
//         #10;

//         $finish;
//     end

// endmodule





        