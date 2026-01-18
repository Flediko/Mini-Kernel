# MiniOS

A minimal operating system implementation written in C and assembly. This project demonstrates the fundamental concepts of operating system development, including boot process, kernel initialization, memory management, and basic system utilities.

## Project Structure

- `boot/` - Bootloader and early initialization code
- `include/` - Header files and system definitions
- `init/` - System initialization code
- `kernel/` - Core kernel functionality
- `lib/` - System libraries and utilities
- `mm/` - Memory management implementation
- `tools/` - System tools and shell implementation

## Building the Project

### Prerequisites

- GCC (with 32-bit support)
- NASM (Netwide Assembler)
- Make
- QEMU (for running the OS)

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/yourusername/minios.git
cd minios
```

2. Build the project:
```bash
make
```

3. Run the OS in QEMU:
```bash
make run
```

## Features

- Basic kernel implementation
- Memory management
- Simple shell interface
- Console and keyboard support
- System initialization
- Basic system utilities

## Development

The project is built using:
- C (GNU99 standard)
- x86 Assembly (NASM)
- 32-bit architecture

### Build System

The project uses a Makefile-based build system with the following main targets:
- `make` - Build the entire project
- `make clean` - Clean build artifacts
- `make run` - Run the OS in QEMU
- `make backup` - Create a backup of the project

## License

This project is licensed under the GPL-3.0 License - see the [gpl-3.0.txt](gpl-3.0.txt) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.