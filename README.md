# CHIP-8 Emulator

A CHIP-8 interpreter and CPU emulator written in modern C++20. Built as an educational exploration into hardware emulation, low-level programming, and C++20.

Runs classic CHIP-8 ROMs including Pong as well as passing the majority of the improved CHIP-8 test suite.

## Features

- Full fetch/decode/execute pipeline with proper instruction timings
- 35+ CHIP-8 opcodes decoded via bitwise masking and nibble extractions
- 16 8-bit registers (V0-VF), a Program Counter, Index Register, and Stack
- Resizable 64x32 monochrome display with XOR sprite rendering and VF collision detection
- 60Hz delay and sound timers with 11 CPU cycles per frame (roughly 1.42ms per cycle)
- Functional audio via SDL3 audio stream with generated square waves
- Full 16-key hex keypad input support via SDL3 scancode events
- RAII resource management, with SDL3 Window, Renderer, Texture, and AudioStream wrapped in `std::unique_ptr` with custom stateless deleters for zero overhead
- C++20 modules (`.cppm`) utilized throughout instead of usual header files

## Architecture

Five independent C++20 named modules, each with a single responsibility:

| Module | File | Responsibility |
|---|---|---|
| `CPU` | `cpu.cppm` | Fetch/decode/execute, all 35+ opcodes, register states, timers |
| `Memory` | `memory.cppm` | 4KB RAM array, font loading, ROM loading, invariant-checked `operator[]` |
| `Display` | `display.cppm` | 64×32 pixel framebuffer, XOR sprite rendering, SDL3 texture stream |
| `Audio` | `audio.cppm` | Square wave generation, SDL3 audio stream |
| `main` | `main.cpp` | Emulation loop, `chrono`-based timing, argument parsing |stateless struct deleter

The `Decoded_Inst` struct utilizes C++ bit fields to extract all instruction fields: opcode, x, y, nibble, nnn, and byte, from the raw 16-bit opcode in a single decode step.

## Building

### Prerequisites

- CMake 3.28+
- GCC 14+ or Clang 17+ (C++20 modules support required)
- SDL3 (included as a submodule)

### Clone

```bash
git clone --recurse-submodules https://github.com/sdp-io/cpp-chip8-emulator.git
cd cpp-chip8-emulator
```

### Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Usage

ROMs are loaded from `data/roms/` relative to the project root. Pass the ROM filename as an argument:

```bash
./build/chip8 pong.ch8
```

The emulator expects `.ch8` files. Public domain ROMs are available at [Zophar's Domain](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html).

## Keypad

The original CHIP-8 hex keypad maps to the keyboard as follows:

```
CHIP-8 Key    Keyboard
──────────    ────────
1  2  3  C    1  2  3  4
4  5  6  D    Q  W  E  R
7  8  9  E    A  S  D  F
A  0  B  F    Z  X  C  V
```

## Known Limitations

- ROM path is currently hardcoded relative to `../../data/roms` requiring running the emulator from the build directory
- CHIP-48 and SUPER-CHIP variants are not currently implemented (noted in TODO comments), ensure compatibility before loading a given ROM

## License

MIT
