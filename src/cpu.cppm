module;

#include <cstdint>

export module CPU;
import Memory;

namespace chip8 {

/**
 * @brief Struct containing fields for a decoded uint16_t CHIP-8 instruction.
 *
 * Contains all byte and nibble combinations that may be needed in order to
 * flexibly support all CHIP-8 instructions.
 */
export struct Decoded_Inst {
  /// The upper 4 bits of the high byte of the instruction. contains the opcode.
  uint8_t opcode : 4;

  /// The lower 4 bits of the high byte of the instruction. Used to look up one
  /// of the 16 registers.
  uint8_t x : 4;

  /// The upper 4 bits of the low byte of the instruction. Also used to look up
  /// one of the 16 registers.
  uint8_t y : 4;

  /// The lowest 4 bits of the instruction.
  uint8_t nibble : 4;

  /// The lowest 12 bits of the instruction. A 12-bit immediate memory address.
  uint16_t nnn : 12;

  /// The lowest 8 bits of the instruction. An 8-bit immediate number.
  uint8_t byte;
};

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

  struct Decoded_Inst decode(const uint16_t &instruction);

private:
  // First 512 bytes historically reserved for fonts and CHIP-8 architecture
  uint16_t pc{0x0200};
  uint16_t index_reg{0x0000};
};

struct Decoded_Inst CPU::decode(const uint16_t &instruction) {
  struct Decoded_Inst di;
  di.opcode = instruction >> 12 & 0xF;
  di.x = instruction >> 8 & 0xF;
  di.y = instruction >> 4 & 0xF;
  di.nibble = instruction & 0xF;
  di.nnn = instruction & 0xFFF;
  di.byte = instruction & 0xFF;

  return di;
}

} // namespace chip8
