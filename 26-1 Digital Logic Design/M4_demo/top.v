module top (
    input  wire CLOCK_50,
    input  wire JOY_UP,
    input  wire JOY_DOWN,
    input  wire JOY_LEFT,
    input  wire JOY_RIGHT,
    input  wire JOY_PRESS,
    inout  wire [15:0] GPIO,
    output wire [3:0] LED
);
    // Lane order: LEFT, DOWN, UP, RIGHT.
    reg [7:0] lane_left = 8'd0;
    reg [7:0] lane_down = 8'd0;
    reg [7:0] lane_up = 8'd0;
    reg [7:0] lane_right = 8'd0;

    reg [24:0] beat_div = 25'd0;
    reg [24:0] blink_div = 25'd0;
    reg blink_on = 1'b1;
    reg [5:0] score = 6'd0;
    reg game_over = 1'b0;
    reg [7:0] lfsr = 8'hA5;
    reg [15:0] entropy_ctr = 16'h53C1;

    // Input synchronizers.
    reg up_meta = 1'b1;
    reg up_sync = 1'b1;
    reg up_last = 1'b1;
    reg down_meta = 1'b1;
    reg down_sync = 1'b1;
    reg down_last = 1'b1;
    reg left_meta = 1'b1;
    reg left_sync = 1'b1;
    reg left_last = 1'b1;
    reg right_meta = 1'b1;
    reg right_sync = 1'b1;
    reg right_last = 1'b1;
    reg press_meta = 1'b1;
    reg press_sync = 1'b1;
    reg press_last = 1'b1;

    reg [7:0] hit_row = 8'd0;
    reg [3:0] spawn_mask = 4'd0;
    reg lane_hit = 1'b0;
    reg miss = 1'b0;
    reg [7:0] lfsr_next = 8'd0;

    reg [2:0] scan_row = 3'd0;
    reg [15:0] scan_div = 16'd0;

    integer i;

    function automatic [3:0] song_mask_from_lfsr;
        input [7:0] state;
        begin
            // One note at a time. Roughly 1/8 of the beats are rests.
            if (state[4:2] == 3'b000) begin
                song_mask_from_lfsr = 4'b0000;
            end else begin
                case (state[1:0])
                    2'd0: song_mask_from_lfsr = 4'b0001;
                    2'd1: song_mask_from_lfsr = 4'b0010;
                    2'd2: song_mask_from_lfsr = 4'b0100;
                    default: song_mask_from_lfsr = 4'b1000;
                endcase
            end
        end
    endfunction

    function automatic [7:0] row_bits_for;
        input [2:0] row_idx;
        input [7:0] left_lane;
        input [7:0] down_lane;
        input [7:0] up_lane;
        input [7:0] right_lane;
        begin
            row_bits_for = 8'b0000_0000;
            row_bits_for[0] = left_lane[row_idx];
            row_bits_for[2] = down_lane[row_idx];
            row_bits_for[4] = up_lane[row_idx];
            row_bits_for[6] = right_lane[row_idx];

            if (row_idx == 3'd7) begin
                row_bits_for[0] = 1'b1;
                row_bits_for[2] = 1'b1;
                row_bits_for[4] = 1'b1;
                row_bits_for[6] = 1'b1;
            end
        end
    endfunction

    function automatic [3:0] row_gpio_for;
        input [2:0] row_idx;
        begin
            case (row_idx)
                3'd0: row_gpio_for = 4'd9;
                3'd1: row_gpio_for = 4'd3;
                3'd2: row_gpio_for = 4'd13;
                3'd3: row_gpio_for = 4'd1;
                3'd4: row_gpio_for = 4'd6;
                3'd5: row_gpio_for = 4'd15;
                3'd6: row_gpio_for = 4'd10;
                default: row_gpio_for = 4'd0;
            endcase
        end
    endfunction

    function automatic [2:0] col_index_for_gpio;
        input [3:0] gpio_idx;
        begin
            case (gpio_idx)
                4'd14: col_index_for_gpio = 3'd0;
                4'd12: col_index_for_gpio = 3'd1;
                4'd4:  col_index_for_gpio = 3'd2;
                4'd11: col_index_for_gpio = 3'd3;
                4'd2:  col_index_for_gpio = 3'd4;
                4'd7:  col_index_for_gpio = 3'd5;
                4'd5:  col_index_for_gpio = 3'd6;
                default: col_index_for_gpio = 3'd7;
            endcase
        end
    endfunction

    function automatic is_row_gpio;
        input [3:0] gpio_idx;
        begin
            case (gpio_idx)
                4'd0,
                4'd1,
                4'd3,
                4'd6,
                4'd9,
                4'd10,
                4'd13,
                4'd15: is_row_gpio = 1'b1;
                default: is_row_gpio = 1'b0;
            endcase
        end
    endfunction

    always @(posedge CLOCK_50) begin
        // Synchronize inputs.
        entropy_ctr <= entropy_ctr + 16'd1;
        up_meta <= JOY_UP;
        up_sync <= up_meta;
        down_meta <= JOY_DOWN;
        down_sync <= down_meta;
        left_meta <= JOY_LEFT;
        left_sync <= left_meta;
        right_meta <= JOY_RIGHT;
        right_sync <= right_meta;
        press_meta <= JOY_PRESS;
        press_sync <= press_meta;

        // Restart on center press.
        if ((press_last == 1'b1) && (press_sync == 1'b0)) begin
            lane_left <= 8'h00;
            lane_down <= 8'h00;
            lane_up <= 8'h00;
            lane_right <= 8'h00;
            beat_div <= 25'd0;
            blink_div <= 25'd0;
            blink_on <= 1'b1;
            score <= 6'd0;
            game_over <= 1'b0;
            lfsr <= ((entropy_ctr[7:0] ^ entropy_ctr[15:8] ^ 8'h5A) == 8'h00)
                ? 8'h5A
                : (entropy_ctr[7:0] ^ entropy_ctr[15:8] ^ 8'h5A);
        end else begin
            // Direction hits: clear the bottom note if it is aligned.
            if ((left_last == 1'b1) && (left_sync == 1'b0)) begin
                if (lane_left[7]) begin
                    lane_left[7] <= 1'b0;
                    score <= score + 6'd1;
                end
            end
            if ((down_last == 1'b1) && (down_sync == 1'b0)) begin
                if (lane_down[7]) begin
                    lane_down[7] <= 1'b0;
                    score <= score + 6'd1;
                end
            end
            if ((up_last == 1'b1) && (up_sync == 1'b0)) begin
                if (lane_up[7]) begin
                    lane_up[7] <= 1'b0;
                    score <= score + 6'd1;
                end
            end
            if ((right_last == 1'b1) && (right_sync == 1'b0)) begin
                if (lane_right[7]) begin
                    lane_right[7] <= 1'b0;
                    score <= score + 6'd1;
                end
            end

            // Main beat timer.
            if (!game_over) begin
                beat_div <= beat_div + 25'd1;
                if (beat_div == 25'd24_999_999) begin
                    beat_div <= 25'd0;

                    miss = lane_left[7] | lane_down[7] | lane_up[7] | lane_right[7];
                    if (miss) begin
                        game_over <= 1'b1;
                    end else begin
                        lane_left <= {lane_left[6:0], 1'b0};
                        lane_down <= {lane_down[6:0], 1'b0};
                        lane_up <= {lane_up[6:0], 1'b0};
                        lane_right <= {lane_right[6:0], 1'b0};

                        lfsr_next = {lfsr[6:0], lfsr[7] ^ lfsr[5]};
                        lfsr <= lfsr_next;
                        spawn_mask = song_mask_from_lfsr(lfsr_next);
                        if (spawn_mask[0]) lane_left[0] <= 1'b1;
                        if (spawn_mask[1]) lane_down[0] <= 1'b1;
                        if (spawn_mask[2]) lane_up[0] <= 1'b1;
                        if (spawn_mask[3]) lane_right[0] <= 1'b1;
                    end
                end
            end else begin
                blink_div <= blink_div + 25'd1;
                if (blink_div == 25'd24_999_999) begin
                    blink_div <= 25'd0;
                    blink_on <= ~blink_on;
                end
            end
        end

        press_last <= press_sync;
        left_last <= left_sync;
        down_last <= down_sync;
        up_last <= up_sync;
        right_last <= right_sync;

        // Row scan.
        scan_div <= scan_div + 16'd1;
        if (scan_div == 16'd0) begin
            scan_row <= scan_row + 3'd1;
        end
    end

    wire [7:0] render_row = game_over && !blink_on
                            ? 8'b0000_0000
                            : row_bits_for(scan_row, lane_left, lane_down, lane_up, lane_right);

    genvar g;
    generate
        for (g = 0; g < 16; g = g + 1) begin : gpio_drive
            if (is_row_gpio(g[3:0])) begin : row_pin
                assign GPIO[g] = (g == row_gpio_for(scan_row)) ? 1'b0 : 1'bz;
            end else begin : col_pin
                assign GPIO[g] = render_row[col_index_for_gpio(g[3:0])] ? 1'b1 : 1'b0;
            end
        end
    endgenerate

    // Keep score visible on the onboard LEDs.
    // L1 is the 1's digit and L4 is the 8's digit on this board.
    assign LED = {~score[3], ~score[2], ~score[1], ~score[0]};
endmodule
