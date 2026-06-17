# rbf_demo

This is a minimal Quartus example that compiles to:

- `output_files/rbf_demo.sof`
- `output_files/rbf_demo.rbf`

The design is a simple counter:

- `CLOCK_50` drives a 24-bit register counter
- `LED` shows the top bit of the counter, so it would blink if mapped to a real board LED

The board is not required to build the files, only to program and observe them.
