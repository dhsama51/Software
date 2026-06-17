import time
import tkinter as tk


GRID_SIZE = 8
LANE_COLUMNS = [0, 2, 4, 6]
LANE_NAMES = ["LEFT", "DOWN", "UP", "RIGHT"]
BEAT_MS = 500
BLINK_MS = 500
CELL = 52
GAP = 8
PADDING = 28


class RhythmGame:
    def __init__(self) -> None:
        self.root = tk.Tk()
        self.root.title("M5 Rhythm Game - SW implementation")
        self.root.resizable(False, False)

        width = PADDING * 2 + GRID_SIZE * CELL + (GRID_SIZE - 1) * GAP
        height = width + 118
        self.canvas = tk.Canvas(
            self.root,
            width=width,
            height=height,
            bg="#101318",
            highlightthickness=0,
        )
        self.canvas.pack()

        self.cells: list[list[int]] = []
        for y in range(GRID_SIZE):
            row = []
            for x in range(GRID_SIZE):
                x0 = PADDING + x * (CELL + GAP)
                y0 = PADDING + y * (CELL + GAP)
                row.append(
                    self.canvas.create_oval(
                        x0,
                        y0,
                        x0 + CELL,
                        y0 + CELL,
                        fill="#232933",
                        outline="#313947",
                        width=2,
                    )
                )
            self.cells.append(row)

        self.score_text = self.canvas.create_text(
            PADDING,
            width + 34,
            anchor="w",
            fill="#e8edf5",
            font=("Consolas", 16, "bold"),
            text="",
        )
        self.status_text = self.canvas.create_text(
            width - PADDING,
            width + 34,
            anchor="e",
            fill="#8ea0b7",
            font=("Consolas", 12),
            text="",
        )
        self.led_text = self.canvas.create_text(
            PADDING,
            width + 76,
            anchor="w",
            fill="#c5d4ea",
            font=("Consolas", 13),
            text="",
        )

        self.root.bind("<Left>", lambda _event: self.hit(0))
        self.root.bind("<Down>", lambda _event: self.hit(1))
        self.root.bind("<Up>", lambda _event: self.hit(2))
        self.root.bind("<Right>", lambda _event: self.hit(3))
        self.root.bind("<space>", lambda _event: self.restart())

        self.entropy_ctr = 0x53C1
        self.lanes = [0, 0, 0, 0]
        self.lfsr = 0xA5
        self.score = 0
        self.game_over = False
        self.blink_on = True
        self.last_beat = time.perf_counter()
        self.last_blink = time.perf_counter()

        self.restart()
        self.loop()
        self.root.mainloop()

    def restart(self) -> None:
        now_ns = time.perf_counter_ns()
        mixed = ((now_ns & 0xFF) ^ ((now_ns >> 8) & 0xFF) ^ 0x5A) & 0xFF
        self.lfsr = mixed if mixed != 0 else 0x5A
        self.entropy_ctr = (self.entropy_ctr + 1) & 0xFFFF
        self.lanes = [0, 0, 0, 0]
        self.score = 0
        self.game_over = False
        self.blink_on = True
        self.last_beat = time.perf_counter()
        self.last_blink = time.perf_counter()
        self.draw()

    def next_lfsr(self) -> int:
        feedback = ((self.lfsr >> 7) ^ (self.lfsr >> 5)) & 1
        self.lfsr = ((self.lfsr << 1) & 0xFE) | feedback
        if self.lfsr == 0:
            self.lfsr = 0x5A
        return self.lfsr

    @staticmethod
    def spawn_lane_from_lfsr(state: int) -> int | None:
        if ((state >> 2) & 0b111) == 0:
            return None
        return state & 0b11

    def hit(self, lane: int) -> None:
        if self.game_over:
            return
        if self.lanes[lane] & (1 << 7):
            self.lanes[lane] &= ~(1 << 7)
            self.score = (self.score + 1) & 0x3F
            self.draw()

    def beat(self) -> None:
        if self.game_over:
            return

        if any(lane & (1 << 7) for lane in self.lanes):
            self.game_over = True
            self.blink_on = True
            return

        self.lanes = [((lane << 1) & 0xFF) for lane in self.lanes]
        spawn_lane = self.spawn_lane_from_lfsr(self.next_lfsr())
        if spawn_lane is not None:
            self.lanes[spawn_lane] |= 1

    def matrix_bits(self) -> list[list[bool]]:
        matrix = [[False for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]
        for lane_idx, lane_bits in enumerate(self.lanes):
            col = LANE_COLUMNS[lane_idx]
            for row in range(GRID_SIZE):
                if lane_bits & (1 << row):
                    matrix[row][col] = True

        for col in LANE_COLUMNS:
            matrix[7][col] = True
        return matrix

    def draw(self) -> None:
        matrix = self.matrix_bits()
        visible = (not self.game_over) or self.blink_on

        for y in range(GRID_SIZE):
            for x in range(GRID_SIZE):
                active = visible and matrix[y][x]
                is_hit_line = y == 7 and x in LANE_COLUMNS
                if active and is_hit_line:
                    fill = "#ffd35a"
                    outline = "#fff0a8"
                elif active:
                    fill = "#30d5ff"
                    outline = "#9feeff"
                elif is_hit_line:
                    fill = "#3b341f"
                    outline = "#6f5a24"
                else:
                    fill = "#232933"
                    outline = "#313947"
                self.canvas.itemconfig(self.cells[y][x], fill=fill, outline=outline)

        visible_score = self.score & 0xF
        led_bits = "".join("1" if visible_score & (1 << i) else "0" for i in range(4))
        self.canvas.itemconfig(self.score_text, text=f"SCORE {visible_score:02d}")
        self.canvas.itemconfig(self.led_text, text=f"L4 L3 L2 L1 = {led_bits[0]}  {led_bits[1]}  {led_bits[2]}  {led_bits[3]}")
        self.canvas.itemconfig(
            self.status_text,
            text="GAME OVER - SPACE" if self.game_over else "ARROWS / SPACE",
        )

    def loop(self) -> None:
        now = time.perf_counter()
        self.entropy_ctr = (self.entropy_ctr + 1) & 0xFFFF

        if not self.game_over and (now - self.last_beat) * 1000 >= BEAT_MS:
            self.last_beat = now
            self.beat()
            self.draw()

        if self.game_over and (now - self.last_blink) * 1000 >= BLINK_MS:
            self.last_blink = now
            self.blink_on = not self.blink_on
            self.draw()

        self.root.after(16, self.loop)


if __name__ == "__main__":
    RhythmGame()
