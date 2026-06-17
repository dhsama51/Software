module top (
    input wire CLOCK_50,
    output wire LED
);
    reg [23:0] counter = 24'd0;

    always @(posedge CLOCK_50) begin
        counter <= counter + 24'd1;
    end

    assign LED = counter[23];
endmodule
