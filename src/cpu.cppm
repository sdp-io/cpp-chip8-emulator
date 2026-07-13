module;

#include <algorithm>
#include <cstdint>
#include <format>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

export module CPU;
import Memory;
import Display;

namespace chip8 {

int random_int(const int &min, const int &max);

constexpr size_t Register_Size{16};

/**
 * @brief Struct containing fields for a decoded uint16_t CHIP-8 instruction.
 *
 * Contains all byte and nibble combinations that may be needed in order to
 * flexibly support all CHIP-8 instructions.
 */
export struct Decoded_Inst {
  /// All 16 bits (the instruction.)
  uint16_t inst;

  /// The upper 4 bits of the high byte of the instruction. Contains the
  /// opcode.
  uint8_t opcode : 4;

  /// The lower 4 bits of the high byte of the instruction. Used to look up
  /// one of the 16 registers.
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
  // Represents actuation status for keys 0-F. True if actuated, else false.
  std::array<bool, Keypad_Size> keypad{};

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

  struct Decoded_Inst decode(const uint16_t &instruction) {
    struct Decoded_Inst di;
    di.inst = instruction;
    di.opcode = instruction >> 12 & 0xF;
    di.x = instruction >> 8 & 0xF;
    di.y = instruction >> 4 & 0xF;
    di.nibble = instruction & 0xF;
    di.nnn = instruction & 0xFFF;
    di.byte = instruction & 0xFF;

    return di;
  }

  void decrement_timers() {
    if (delay_timer > 0) {
      delay_timer--;
    }

    if (sound_timer > 0) {
      sound_timer--;
    }
  }

  void execute(struct Decoded_Inst &di, Memory &memory, Display &display);

  uint8_t get_register(const size_t address) { return registers[address]; }

private:
  // First 512 bytes historically reserved for fonts and CHIP-8 architecture
  uint16_t pc{0x0200};
  uint16_t index_reg{0x0000};
  uint8_t delay_timer{0};
  uint8_t sound_timer{0};
  std::vector<uint16_t> stack_reg{};
  std::array<uint8_t, Register_Size> registers{};
  uint8_t &flag_reg{registers[Register_Size - 1]};

  void jump(const unsigned int location) { pc = location; }

  void execute_DXYN(struct Decoded_Inst &di, Memory &memory, Display &display);
};

void CPU::execute(struct Decoded_Inst &di, Memory &memory, Display &display) {
  uint8_t val_x{registers[di.x]};
  uint8_t val_y{registers[di.y]};
  uint8_t bitmask{};

  int random_num{random_int(0, 16)};

  switch (di.opcode) {
  case 0x0:
    switch (di.nibble) {
    case 0x0:
      display.clear_display();
      break;
    case 0xE:
      pc = stack_reg.back();
      stack_reg.pop_back();
      break;
    }
    break; // Parent case
  case 0x1:
    CPU::jump(di.nnn);
    break;
  case 0x2:
    stack_reg.push_back(pc);
    CPU::jump(di.nnn);
    break;
  case 0x3:
    if (val_x == di.byte) {
      pc += 2;
    }
    break;
  case 0x4:
    if (val_x != di.byte) {
      pc += 2;
    }
    break;
  case 0x5:
    if (val_x == val_y) {
      pc += 2;
    }
    break;
  case 0x6:
    registers[di.x] = di.byte;
    break;
  case 0x7:
    registers[di.x] += di.byte;
    break;
  case 0x8:
    // Switches for logical and arithmetic instructions
    switch (di.nibble) {
    case 0x0:
      registers[di.x] = registers[di.y];
      break;
    case 0x1:
      registers[di.x] = val_x | val_y;
      break;
    case 0x2:
      registers[di.x] = val_x & val_y;
      break;
    case 0x3:
      registers[di.x] = val_x ^ val_y;
      break;
    case 0x4:
      registers[di.x] = val_x + val_y;
      if ((val_x + val_y) > 255) { // Set VF on overflow
        flag_reg = 1;
      } else {
        flag_reg = 0;
      }
      break;
    case 0x5:
      registers[di.x] = val_x - val_y;
      if (val_y > val_x) { // Set VF (carry flag) on underflow
        flag_reg = 0;
      } else {
        flag_reg = 1;
      }
      break;
    case 0x6:
      // TODO: Add support CHIP-48 and SUPER-CHIP instruction variations
      registers[di.x] >>= 1;
      if ((val_x & 1) == 1) { // Set VF if bit carried out was a 1
        flag_reg = 1;
      } else {
        flag_reg = 0;
      }
      break;
    case 0x7:
      registers[di.x] = val_y - val_x;
      if (val_x > val_y) { // Set VF (carry flag) on underflow
        flag_reg = 0;
      } else {
        flag_reg = 1;
      }
      break;
    case 0xE:
      // TODO: Add support CHIP-48 and SUPER-CHIP instruction variations
      registers[di.x] <<= 1;
      if ((val_x & 128) == 128) { // Set VF if bit carried out was a 1
        flag_reg = 1;
      } else {
        flag_reg = 0;
      }
      break;
    }
    break; // Parent case
  case 0x9:
    if (val_x != val_y) {
      pc += 2;
    }
    break;
  case 0xA:
    index_reg = di.nnn;
    break;
  case 0xB:
    // TODO: Add support CHIP-48 and SUPER-CHIP instruction variations
    jump(di.nnn + registers[0]);
    break;
  case 0xC:
    registers[di.x] = di.byte & random_num;
    break;
  case 0xD:
    execute_DXYN(di, memory, display);
    break;
  case 0xE:
    switch (di.nibble) {
    case 0x1:
      if (!keypad[val_x]) {
        pc += 2;
      }
      break;
    case 0xE:
      if (keypad[val_x]) {
        pc += 2;
      }
      break;
    }
    break; // Parent case
  case 0xF:
    switch (di.byte) {
    case 0x07:
      registers[di.x] = delay_timer;
      break;
    case 0x0A: {
      auto keypad_it{std::ranges::find(keypad, true)}; // Find activated key
      if (keypad_it != keypad.end()) {
        // Index for activated key equal to key's hex value
        auto index{std::distance(keypad.begin(), keypad_it)};
        registers[di.x] = index;
      } else {
        pc -= 2; // No keys pressed, reset PC from previous step
      }
      break;
    }
    case 0x15:
      delay_timer = val_x;
      break;
    case 0x18:
      sound_timer = val_x;
      break;
    case 0x1E:
      // TODO: Set VF flag on overflow
      index_reg += val_x;
      break;
    case 0x29:
      index_reg = val_x * 5; // Index for each font row is 0-F * 5
      break;
    case 0x33:
      memory[index_reg + 0] = val_x / 100;
      memory[index_reg + 1] = (val_x / 10) % 10;
      memory[index_reg + 2] = val_x % 10;
      break;
    case 0x55:
      for (int i{0}; i <= di.x; i++) {
        memory[index_reg + i] = registers[i];
      }
      break;
    case 0x65:
      for (int i{0}; i <= di.x; i++) {
        registers[i] = memory[index_reg + i];
      }
      break;
    }
    break; // Parent case
  default:
    std::cout << "Unknown instruction!\n";
  }
}

void CPU::execute_DXYN(struct Decoded_Inst &di, Memory &memory,
                       Display &display) {
  // Bitwise AND to wraparound any out-of-bounds coordinates
  int x_coord{registers[di.x] & 63};
  int y_coord{registers[di.y] & 31};
  uint8_t sprite_height{di.nibble}; // Get N lines of sprite data

  flag_reg = 0;
  for (uint8_t i{0}; i < sprite_height; i++) {
    uint8_t sprite_data{memory.get_sprite_data(index_reg + i)};
    uint8_t has_collision{display.draw_line(sprite_data, x_coord, y_coord)};

    y_coord++;
    if (has_collision) {
      flag_reg = 1;
    }
  }
}

int random_int(const int &min, const int &max) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(min, max);

  int random_num{distrib(gen)};

  return random_num;
}

} // namespace chip8
