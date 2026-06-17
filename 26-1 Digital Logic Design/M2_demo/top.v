module top (
    input  wire CLOCK_50,
    input  wire JOY_PRESS,
    inout  wire [15:0] GPIO,
    output wire [3:0] LED
);
    // Joystick center button is active-low.
    // Each press advances one of 8 visible patterns.
    reg [2:0] pattern_id = 3'd0;

    // Button synchronizer and debounce.
    reg joy_sync0 = 1'b1;
    reg joy_sync1 = 1'b1;
    reg joy_stable = 1'b1;
    reg joy_last = 1'b1;
    reg [19:0] joy_wait = 20'd0;

    // Fast row scan for multiplexing.
    reg [2:0] scan_row = 3'd0;
    reg [15:0] scan_div = 16'd0;

    // Slow animation counters for the moving test patterns.
    reg [2:0] anim_row = 3'd0;
    reg [2:0] anim_col = 3'd7;
    reg [25:0] anim_div = 26'd0;

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
                    pattern_id <= pattern_id + 3'd1;
                end
            end
        end else begin
            joy_wait <= 20'd0;
            joy_last <= joy_stable;
        end

        // Row multiplexing.
        scan_div <= scan_div + 16'd1;
        if (scan_div == 16'd0) begin
            scan_row <= scan_row + 3'd1;
        end

        // Slow animation for the two sweeping patterns.
        anim_div <= anim_div + 26'd1;
        if (anim_div == 26'd29_999_999) begin
            anim_row <= anim_row + 3'd1;
            anim_col <= anim_col - 3'd1;
            anim_div <= 26'd0;
        end
    end

    // Inferred mapping from the previous M2 observations.
    // Rows are common-cathode candidates, columns are common-anode candidates.
    function automatic [3:0] row_gpio_for;
        input [2:0] row_idx;
        begin
            case (row_idx)
                3'd0: row_gpio_for = 4'd9;   // R1
                3'd1: row_gpio_for = 4'd3;   // R2
                3'd2: row_gpio_for = 4'd13;  // R3
                3'd3: row_gpio_for = 4'd1;   // R4
                3'd4: row_gpio_for = 4'd6;   // R5
                3'd5: row_gpio_for = 4'd15;  // R6
                3'd6: row_gpio_for = 4'd10;  // R7
                default: row_gpio_for = 4'd0; // R8
            endcase
        end
    endfunction

    function automatic [3:0] col_gpio_for;
        input [2:0] col_idx;
        begin
            case (col_idx)
                3'd0: col_gpio_for = 4'd14;  // C1
                3'd1: col_gpio_for = 4'd12;  // C2
                3'd2: col_gpio_for = 4'd4;   // C3
                3'd3: col_gpio_for = 4'd11;  // C4
                3'd4: col_gpio_for = 4'd2;   // C5
                3'd5: col_gpio_for = 4'd7;   // C6
                3'd6: col_gpio_for = 4'd5;   // C7
                default: col_gpio_for = 4'd8; // C8
            endcase
        end
    endfunction

    function automatic [2:0] col_index_for_gpio;
        input [3:0] gpio_idx;
        begin
            case (gpio_idx)
                4'd14: col_index_for_gpio = 3'd0; // C1
                4'd12: col_index_for_gpio = 3'd1; // C2
                4'd4:  col_index_for_gpio = 3'd2; // C3
                4'd11: col_index_for_gpio = 3'd3; // C4
                4'd2:  col_index_for_gpio = 3'd4; // C5
                4'd7:  col_index_for_gpio = 3'd5; // C6
                4'd5:  col_index_for_gpio = 3'd6; // C7
                default: col_index_for_gpio = 3'd7; // C8
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

    function automatic [7:0] pattern_cols;
        input [2:0] pid;
        input [2:0] row_idx;
        input [2:0] col_idx;
        begin
            case (pid)
                3'd0: begin
                    // Four corners.
                    if ((row_idx == 3'd0) || (row_idx == 3'd7)) begin
                        pattern_cols = 8'b1000_0001;
                    end else begin
                        pattern_cols = 8'b0000_0000;
                    end
                end
                3'd1: begin
                    // All on.
                    pattern_cols = 8'b1111_1111;
                end
                3'd2: begin
                    // Checkerboard, alternating every cell.
                    pattern_cols = row_idx[0] ? 8'b0101_0101 : 8'b1010_1010;
                end
                3'd3: begin
                    // Border only.
                    if ((row_idx == 3'd0) || (row_idx == 3'd7)) begin
                        pattern_cols = 8'b1111_1111;
                    end else begin
                        pattern_cols = 8'b1000_0001;
                    end
                end
                3'd4: begin
                    // Anti-diagonal: (8,1), (7,2), ..., (1,8).
                    pattern_cols = 8'b1000_0000 >> row_idx;
                end
                3'd5: begin
                    // Row sweep: (1,*), (2,*), ..., (8,*).
                    pattern_cols = (row_idx == anim_row) ? 8'b1111_1111 : 8'b0000_0000;
                end
                3'd6: begin
                    // Column sweep: (*,8), (*,7), ..., (*,1).
                    pattern_cols = 8'b0000_0001 << anim_col;
                end
                3'd7: begin
                    // All on again.
                    pattern_cols = 8'b1111_1111;
                end
                default: begin
                    pattern_cols = 8'b0000_0000;
                end
            endcase
        end
    endfunction

    wire [3:0] active_row_gpio = row_gpio_for(scan_row);
    wire [7:0] cols_for_row = pattern_cols(pattern_id, scan_row, anim_col);

    genvar i;
    generate
        for (i = 0; i < 16; i = i + 1) begin : gpio_drive
            if (is_row_gpio(i[3:0])) begin : row_pin
                // Only the currently scanned row is pulled LOW.
                assign GPIO[i] = (i == active_row_gpio) ? 1'b0 : 1'bz;
            end else begin : col_pin
                // Columns are actively driven HIGH/LOW.
                assign GPIO[i] = cols_for_row[col_index_for_gpio(i[3:0])] ? 1'b1 : 1'b0;
            end
        end
    endgenerate

    // DVK600 onboard LEDs are active-low.
    // Show pattern_id as L4, L3, L2, keep L1 off.
    assign LED = {~pattern_id[0], ~pattern_id[1], ~pattern_id[2], 1'b1};
endmodule
