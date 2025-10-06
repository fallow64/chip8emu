# chip8emu

A simple CHIP-8 emulator written in C++ using SDL3 for graphics and input.

## How to Build and Run

1. Clone this repository, with submodules:
   ```bash
   git clone https://github.com/fallow64/chip8emu --recurse-submodules
   ```
2. Configure CMake:
   ```bash
   cmake -B build
   ```
3. Build the project:
   ```bash
   cmake --build build
   ```
4. Run the emulator with a CHIP-8 ROM:
   ```bash
   ./build/chip8emu 4 1 ./roms/
   ```

## Command Line Arguments

`chip8emu <scale> <delay> <rom_file>`

- `scale`: Integer scaling factor for the display (e.g., 4 for 4x scale).
- `delay`: Delay in milliseconds between CPU cycles (e.g., 1 for 1ms). Larger values slow down execution.
- `rom_file`: Path to the CHIP-8 ROM file to load. There are several sample ROMs in the `roms/` directory.
