#include <fstream>
#include <iostream>
import CPU;
import Memory;
import Display;

int main() {
  chip8::Display display{};
  chip8::CPU cpu{};
  chip8::Memory memory{};

  std::cout << "Hello world!\n";
  std::string rom_path{"../data/roms/IBMLogo.ch8"};

  std::fstream rom{rom_path, rom.binary | rom.in};

  if (!rom.is_open()) {
    std::cout << "Error getting ROM!\n";
    return 1;
  } else {
    // Load ROM here
    memory.load_rom(rom);
  }

  return 0;
}
