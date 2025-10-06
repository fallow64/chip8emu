#pragma once

#include "platform.hpp"
#include <cstdint>
#include <random>

// The starting address for execution.
const unsigned int START_ADDRESS = 0x200;
// The starting address for the fontset in memory.
const unsigned int FONTSET_START_ADDRESS = 0x50;
// The size of the fontset.
const unsigned int FONTSET_SIZE = 80;
// The width of the display in pixels.
const unsigned int VIDEO_WIDTH = 64;
// The height of the display in pixels.
const unsigned int VIDEO_HEIGHT = 32;

class Chip8 {
  public:
    uint8_t keypad[16]{};
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};

    // Constructor takes a platform interface for I/O
    Chip8(IPlatform *platform);

    // Load a ROM into memory by filename
    void LoadROM(char const *filename);

    // Emulate one cycle
    void Cycle();

  private:
    IPlatform *platform;

    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint16_t opcode;

    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    typedef void (Chip8::*Chip8Func)();
    Chip8Func table[0xF + 1];
    Chip8Func table0[0xE + 1];
    Chip8Func table8[0xE + 1];
    Chip8Func tableE[0xE + 1];
    Chip8Func tableF[0x65 + 1];

    //
    // Subtable functions
    //

    void Table0();
    void Table8();
    void TableE();
    void TableF();

    //
    // Opcode implementations
    //

    // CLS
    // Clear the display
    void OP_00E0();
    // RET
    // Return from a subroutine
    void OP_00EE();
    // 1nnn - JP addr
    // Jump to location nnn
    void OP_1nnn();
    // 2nnn - CALL addr
    // Call subroutine at nnn
    void OP_2nnn();
    // 3xkk - SE Vx, byte
    // Skip next instruction if Vx = kk
    void OP_3xkk();
    // 4xkk - SNE Vx, byte
    // Skip next instruction if Vx != kk
    void OP_4xkk();
    // 5xy0 - SE Vx, Vy
    // Skip next instruction if Vx = Vy
    void OP_5xy0();
    // 6xkk - LD Vx, byte
    // Set Vx = kk
    void OP_6xkk();
    // 7xkk - ADD Vx, byte
    // Set Vx = Vx + kk
    void OP_7xkk();
    // 8xy0 - LD Vx, Vy
    // Set Vx = Vy
    void OP_8xy0();
    // 8xy1 - OR Vx, Vy
    // Set Vx = Vx OR Vy
    void OP_8xy1();
    // 8xy2 - AND Vx, Vy
    // Set Vx = Vx AND Vy
    void OP_8xy2();
    // 8xy3 - XOR Vx, Vy
    // Set Vx = Vx XOR Vy
    void OP_8xy3();
    // 8xy4 - ADD Vx, Vy
    // Set Vx = Vx + Vy, set VF = carry
    void OP_8xy4();
    // 8xy5 - SUB Vx, Vy
    // Set Vx = Vx - Vy, set VF = NOT borrow
    void OP_8xy5();
    // 8xy6 - SHR Vx, 1
    // If LSB of Vx is 1, set VF = 1, else 0. Then Vx = Vx >> 1
    void OP_8xy6();
    // 8xy7 - SUBN Vx, Vy
    // Set Vx = Vy - Vx, set VF = NOT borrow
    void OP_8xy7();
    // 8xyE - SHL Vx {, Vy}
    // If MSB of Vx is 1, set VF = 1, else 0. Then Vx = Vx << 1
    void OP_8xyE();
    // 9xy0 - SNE Vx, Vy
    // Skip next instruction if Vx != Vy
    void OP_9xy0();
    // Annn - LD I, addr
    // Set I = nnn
    void OP_Annn();
    // Bnnn - JP V0, addr
    // Jump to location nnn + V0
    void OP_Bnnn();
    // Cxkk - RND Vx, byte
    // Set Vx = random byte AND kk
    void OP_Cxkk();
    // Dxyn - DRW Vx, Vy, nibble
    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    void OP_Dxyn();
    // Ex9E - SKP Vx
    // Skip next instruction if key with the value of Vx is pressed
    void OP_Ex9E();
    // ExA1 - SKNP Vx
    // Skip next instruction if key with the value of Vx is not pressed
    void OP_ExA1();
    // Fx07 - LD Vx, DT
    // Set Vx = delay timer value
    void OP_Fx07();
    // Fx0A - LD Vx, K
    // Wait for a key press, store the value of the key in Vx
    void OP_Fx0A();
    // Fx15 - LD DT, Vx
    // Set delay timer = Vx
    void OP_Fx15();
    // Fx18 - LD ST, Vx
    // Set sound timer = Vx
    void OP_Fx18();
    // Fx1E - ADD I, Vx
    // Set I = I + Vx
    void OP_Fx1E();
    // Fx29 - LD F, Vx
    // Set I = location of sprite for digit Vx
    void OP_Fx29();
    // Fx33 - LD B, Vx
    // Store BCD representation of Vx in memory locations I, I+1, and I+2
    void OP_Fx33();
    // Fx55 - LD [I], Vx
    // Store registers V0 through Vx in memory starting at location I
    void OP_Fx55();
    // Fx65 - LD Vx, [I]
    // Read registers V0 through Vx from memory starting at location I
    void OP_Fx65();
    // Null operation
    void OP_NULL() {}
};

// Fontset for the Chip8 system
extern uint8_t fontset[FONTSET_SIZE];