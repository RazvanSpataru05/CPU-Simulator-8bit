## CPU Emulator - 8-bit
A cycle accurate 8-bit CPU emulator with a 16-bit address space, built in C++ with an interactive ImGui + SFML interface
Designed to simulate a low-level processor including instruction execution, memory access, stack management.

## Features
# Processor
- Custom 8-bit ISA with ~50 instructions across 7 categories
- 4 general-purpose registers, Program Counter, Instruction Register and Stack Pointer
- Flag registers
- Accurate fetch, decode and execute cycle

# Memory
- 16-bit address space (65.536 bytes)
- Paginated view featuring 256 pages x 256 addresses each
- Active cell highlighted for PC position
- Read/Write access highlighted cells per instruction cycle
- Free navigation mode

# Execution Control
- Step-by-step execution
- Auto mode: continous execution at configurable speed
- Execution speed slider: 0.1s-2.0s per instruction
- Reset to initial state at any point

# Dissasembler
- Reads machine code from a .txt file
- Translates hexadecimal code back into human-readable assembly mnemonics

# CPU State Viewer
- Real-time display of all registers and flags after each instruction

## Built With
- C++17
- ImGui
- SFML
