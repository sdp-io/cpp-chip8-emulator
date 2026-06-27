module;

#include <cstdint>

export module CPU;

namespace chip8 {

export class CPU {
public:
  CPU() = default;

private:
  // First 512 bytes historically reserved for fonts and CHIP-8 architecture
  uint16_t pc{0x0200};
  uint16_t index_reg{0x0000};
};

} // namespace chip8
