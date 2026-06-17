// M2 experiment 2: find row candidates.
// Fixed anchor: GPIO[8] is held HIGH, which was observed as C8.
// Each JOY_PRESS advances probe_idx and drives one GPIO LOW.
// If the LED matrix shows (8, y), the probed GPIO is row Ry.
module top (
    input  wire CLOCK_50,
    input  wire JOY_PRESS,
    inout  wire [15:0] GPIO,
    output wire [3:0] LED
);
    localparam [3:0] ANCHOR_COL_GPIO = 4'd8;

    reg [3:0] probe_idx = 4'd0;

    reg joy_sync0 = 1'b1;
    reg joy_sync1 = 1'b1;
    reg joy_stable = 1'b1;
    reg joy_last = 1'b1;
    reg [19:0] joy_wait = 20'd0;

    always @(posedge CLOCK_50) begin
        joy_sync0 <= JOY_PRESS;
        joy_sync1 <= joy_sync0;

        if (joy_sync1 != joy_stable) begin
            joy_wait <= joy_wait + 20'd1;
            if (joy_wait == 20'd999_999) begin
                joy_last <= joy_stable;
                joy_stable <= joy_sync1;
                joy_wait <= 20'd0;

                if ((joy_last == 1'b1) && (joy_sync1 == 1'b0)) begin
                    probe_idx <= probe_idx + 4'd1;
                end
            end
        end else begin
            joy_wait <= 20'd0;
            joy_last <= joy_stable;
        end
    end

    genvar i;
    generate
        for (i = 0; i < 16; i = i + 1) begin : gpio_drive
            if (i == ANCHOR_COL_GPIO) begin : anchor_pin
                assign GPIO[i] = 1'b1;
            end else begin : probe_pin
                assign GPIO[i] = (probe_idx == i[3:0]) ? 1'b0 : 1'bz;
            end
        end
    endgenerate

    // Active-low onboard LEDs. Display probe_idx as 0~15.
    assign LED = {~probe_idx[0], ~probe_idx[1], ~probe_idx[2], ~probe_idx[3]};
endmodule
