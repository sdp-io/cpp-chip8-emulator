module;

#include <cstdint>

export module CPU;
import Memory;

namespace chip8 {

export class CPU {
public:
  CPU() = default;

  uint16_t fetch(const chip8::Memory &memory) {
    uint16_t instruction{memory[pc]};

    // Shift first byte of instruction in memory into upper bits
    instruction = instruction << 8;

    // Bitwise OR second byte of instruction in memory into lower bits
    instruction = instruction | memory[pc + 1];

    // Increment PC by two as two bytes were read
    pc += 2;

    return instruction;
  }

private:
  // First 512 bytes historically reserved for fonts and CHIP-8 architecture
  uint16_t pc{0x0200};
  uint16_t index_reg{0x0000};
};

} // namespace chip8
