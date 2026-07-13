#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
import CPU;
import Memory;
import Display;

int main() {
  using namespace std::chrono_literals;

  // 60Hz timers = 1 decrementation every 16.67ms
  constexpr auto frame_time{16.67ms};

  // 11 CPU Cycles can run roughly every 16.67ms (1.42ms per cycle)
  constexpr auto cycles_per_frame{11};

  chip8::Display display{};
  chip8::CPU cpu{};
  chip8::Memory memory{};

  std::cout << "Hello world!\n";
  std::string rom_path{"../../data/roms/Pong.ch8"};

  std::fstream rom{rom_path, rom.binary | rom.in};

  if (!rom.is_open()) {
    std::cout << "Error getting ROM!\n";
    return 1;
  } else {
    // Load ROM here
    memory.load_rom(rom);
  }

  // Main CHIP-8 program loop
  while (display.poll_events(cpu.keypad)) {
    auto loop_start{std::chrono::steady_clock::now()};

    cpu.decrement_timers();

    for (int i{0}; i < cycles_per_frame; i++) {
      // Fetch
      uint16_t instruction{cpu.fetch(memory)};

      // Decode
      struct chip8::Decoded_Inst di{cpu.decode(instruction)};

      // Execute
      cpu.execute(di, memory, display);
    }

    auto loop_delta{std::chrono::steady_clock::now() - loop_start};
    auto remaining_frame_time{frame_time - loop_delta};

    std::this_thread::sleep_for(remaining_frame_time);
  }
  return 0;
}
