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
    localparam [1:0] DIR_UP    = 2'd0;
    localparam [1:0] DIR_DOWN  = 2'd1;
    localparam [1:0] DIR_LEFT  = 2'd2;
    localparam [1:0] DIR_RIGHT = 2'd3;

    reg [2:0] scan_row = 3'd0;
    reg [15:0] scan_div = 16'd0;

    reg [25:0] game_div = 26'd0;
    reg [24:0] blink_div = 25'd0;
    reg blink_on = 1'b1;
    reg [5:0] snake_len = 6'd3;
    reg [5:0] snake_idx [0:63];
    reg [63:0] snake_bits = 64'd0;
    reg [2:0] head_x = 3'd3;
    reg [2:0] head_y = 3'd4;
    reg [5:0] food_idx = 6'd21;
    reg [1:0] dir_cur = DIR_RIGHT;
    reg [1:0] dir_next = DIR_RIGHT;
    reg [5:0] score = 6'd0;
    reg game_over = 1'b0;
    reg [7:0] lfsr = 8'hA5;
    reg [15:0] entropy_ctr = 16'h1ACE;

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

    reg [2:0] next_x;
    reg [2:0] next_y;
    reg [5:0] next_idx;
    reg [5:0] tail_idx;
    reg [5:0] candidate_idx;
    reg [63:0] next_snake_bits;
    reg [63:0] occupied_after;
    reg [7:0] lfsr_next;
    reg grow;
    reg collision;
    reg wall_hit;
    reg found_food;
    integer i;

    function automatic [5:0] cell_index;
        input [2:0] x;
        input [2:0] y;
        begin
            cell_index = {y, x};
        end
    endfunction

    function automatic is_opposite_dir;
        input [1:0] a;
        input [1:0] b;
        begin
            is_opposite_dir =
                ((a == DIR_UP)    && (b == DIR_DOWN)) ||
                ((a == DIR_DOWN)  && (b == DIR_UP))   ||
                ((a == DIR_LEFT)  && (b == DIR_RIGHT)) ||
                ((a == DIR_RIGHT) && (b == DIR_LEFT));
        end
    endfunction

    initial begin
        for (i = 0; i < 64; i = i + 1) begin
            snake_idx[i] = 6'd0;
        end
        snake_idx[0] = cell_index(3'd3, 3'd4);
        snake_idx[1] = cell_index(3'd2, 3'd4);
        snake_idx[2] = cell_index(3'd1, 3'd4);
        snake_bits = (64'd1 << cell_index(3'd3, 3'd4)) |
                     (64'd1 << cell_index(3'd2, 3'd4)) |
                     (64'd1 << cell_index(3'd1, 3'd4));
        head_x = 3'd3;
        head_y = 3'd4;
        food_idx = cell_index(3'd5, 3'd2);
        dir_cur = DIR_RIGHT;
        dir_next = DIR_RIGHT;
        score = 6'd0;
        game_over = 1'b0;
        lfsr = 8'hA5;
        entropy_ctr = 16'h1ACE;
    end

    always @(posedge CLOCK_50) begin
        // Button synchronizers.
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

        // Direction changes on falling edges.
        if ((up_last == 1'b1) && (up_sync == 1'b0)) begin
            if (!is_opposite_dir(dir_next, DIR_UP)) begin
                dir_next <= DIR_UP;
            end
        end
        if ((down_last == 1'b1) && (down_sync == 1'b0)) begin
            if (!is_opposite_dir(dir_next, DIR_DOWN)) begin
                dir_next <= DIR_DOWN;
            end
        end
        if ((left_last == 1'b1) && (left_sync == 1'b0)) begin
            if (!is_opposite_dir(dir_next, DIR_LEFT)) begin
                dir_next <= DIR_LEFT;
            end
        end
        if ((right_last == 1'b1) && (right_sync == 1'b0)) begin
            if (!is_opposite_dir(dir_next, DIR_RIGHT)) begin
                dir_next <= DIR_RIGHT;
            end
        end

        // Center press restarts the game.
        if ((press_last == 1'b1) && (press_sync == 1'b0)) begin
            snake_idx[0] <= cell_index(3'd3, 3'd4);
            snake_idx[1] <= cell_index(3'd2, 3'd4);
            snake_idx[2] <= cell_index(3'd1, 3'd4);
            for (i = 3; i < 64; i = i + 1) begin
                snake_idx[i] <= 6'd0;
            end
            snake_bits <= (64'd1 << cell_index(3'd3, 3'd4)) |
                          (64'd1 << cell_index(3'd2, 3'd4)) |
                          (64'd1 << cell_index(3'd1, 3'd4));
            snake_len <= 6'd3;
            head_x <= 3'd3;
            head_y <= 3'd4;
            food_idx <= cell_index(3'd5, 3'd2);
            dir_cur <= DIR_RIGHT;
            dir_next <= DIR_RIGHT;
            score <= 6'd0;
            game_over <= 1'b0;
            lfsr <= ((entropy_ctr[7:0] ^ entropy_ctr[15:8] ^ 8'hA5) == 8'h00)
                ? 8'hA5
                : (entropy_ctr[7:0] ^ entropy_ctr[15:8] ^ 8'hA5);
            game_div <= 26'd0;
        end else begin
            // Slow game tick.
            game_div <= game_div + 26'd1;
            if (game_div == 26'd24_999_999) begin
                game_div <= 26'd0;

                if (!game_over) begin
                    next_x = head_x;
                    next_y = head_y;
                    wall_hit = 1'b0;

                    case (dir_next)
                        DIR_UP: begin
                            if (head_y == 3'd0) begin
                                wall_hit = 1'b1;
                            end else begin
                                next_y = head_y - 3'd1;
                            end
                        end
                        DIR_DOWN: begin
                            if (head_y == 3'd7) begin
                                wall_hit = 1'b1;
                            end else begin
                                next_y = head_y + 3'd1;
                            end
                        end
                        DIR_LEFT: begin
                            if (head_x == 3'd0) begin
                                wall_hit = 1'b1;
                            end else begin
                                next_x = head_x - 3'd1;
                            end
                        end
                        default: begin
                            if (head_x == 3'd7) begin
                                wall_hit = 1'b1;
                            end else begin
                                next_x = head_x + 3'd1;
                            end
                        end
                    endcase

                    next_idx = cell_index(next_x, next_y);
                    grow = (next_idx == food_idx);
                    tail_idx = snake_idx[snake_len - 1];
                    collision = wall_hit;

                    for (i = 0; i < 64; i = i + 1) begin
                        if (i < (grow ? snake_len : (snake_len - 6'd1))) begin
                            if (snake_idx[i] == next_idx) begin
                                collision = 1'b1;
                            end
                        end
                    end

                    if (collision) begin
                        game_over <= 1'b1;
                    end else begin
                        if (grow) begin
                            for (i = 63; i > 0; i = i - 1) begin
                                if (i <= snake_len) begin
                                    snake_idx[i] <= snake_idx[i - 1];
                                end
                            end
                            snake_len <= snake_len + 6'd1;
                        end else begin
                            for (i = 63; i > 0; i = i - 1) begin
                                if (i < snake_len) begin
                                    snake_idx[i] <= snake_idx[i - 1];
                                end
                            end
                        end

                        snake_idx[0] <= next_idx;
                        occupied_after = grow ?
                            (snake_bits | (64'd1 << next_idx)) :
                            ((snake_bits & ~(64'd1 << tail_idx)) | (64'd1 << next_idx));
                        snake_bits <= occupied_after;
                        head_x <= next_x;
                        head_y <= next_y;
                        dir_cur <= dir_next;
                        lfsr_next = {lfsr[6:0], lfsr[7] ^ lfsr[5]};
                        lfsr <= lfsr_next;

                        if (grow) begin
                            candidate_idx = lfsr_next[5:0];
                            found_food = 1'b0;
                            for (i = 0; i < 64; i = i + 1) begin
                                if (!found_food && !occupied_after[candidate_idx]) begin
                                    food_idx <= candidate_idx;
                                    found_food = 1'b1;
                                end else if (!found_food) begin
                                    candidate_idx = candidate_idx + 6'd1;
                                end
                            end
                            if (!found_food) begin
                                food_idx <= 6'd0;
                            end
                            score <= score + 6'd1;
                        end
                    end
                end
            end
        end

        if (game_over) begin
            blink_div <= blink_div + 25'd1;
            if (blink_div == 25'd24_999_999) begin
                blink_div <= 25'd0;
                blink_on <= ~blink_on;
            end
        end else begin
            blink_div <= 25'd0;
            blink_on <= 1'b1;
        end

        up_last <= up_sync;
        down_last <= down_sync;
        left_last <= left_sync;
        right_last <= right_sync;
        press_last <= press_sync;

        // Fast row scan for multiplexing.
        scan_div <= scan_div + 16'd1;
        if (scan_div == 16'd0) begin
            scan_row <= scan_row + 3'd1;
        end
    end

    wire [63:0] food_bits = 64'd1 << food_idx;
    wire [63:0] frame_bits = snake_bits | food_bits;
    wire [63:0] visible_bits = game_over ? (blink_on ? frame_bits : 64'd0) : frame_bits;
    wire [63:0] row_shift = visible_bits >> {scan_row, 3'b000};
    wire [7:0] cols_for_row = row_shift[7:0];

    // Inferred mapping from the previous board probing.
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

    genvar g;
    generate
        for (g = 0; g < 16; g = g + 1) begin : gpio_drive
            if (is_row_gpio(g[3:0])) begin : row_pin
                assign GPIO[g] = (g == row_gpio_for(scan_row)) ? 1'b0 : 1'bz;
            end else begin : col_pin
                assign GPIO[g] = cols_for_row[col_index_for_gpio(g[3:0])] ? 1'b1 : 1'b0;
            end
        end
    endgenerate

    // DVK600 onboard LEDs are active-low.
    // Keep the score visible even after game over.
    assign LED = {~score[0], ~score[1], ~score[2], ~score[3]};
endmodule
