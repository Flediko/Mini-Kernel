<h1 align="center">
  🧠 MiniOS
</h1>

<p align="center">
  ⚔️ A Minimal Operating System • 🛠️ Built in C & Assembly  
</p>

<p align="center">
  Low-level systems programming • Kernel fundamentals • OS internals
</p>

---

## 🧬 Project Overview
**MiniOS** is a minimal operating system implementation written in **C and x86 Assembly**.  
This project demonstrates fundamental OS concepts including the boot process, kernel
initialization, memory management, and basic system utilities.

---

## 🗺️ Project Structure

- `boot/` - Bootloader and early initialization code
- `include/` - Header files and system definitions
- `init/` - System initialization code
- `kernel/` - Core kernel functionality
- `lib/` - System libraries and utilities
- `mm/` - Memory management implementation
- `tools/` - System tools and shell implementation




---

## ⚙️ Features
- Custom bootloader
- Kernel initialization
- Memory management
- Console & keyboard support
- Simple shell interface
- Basic system utilities

---

## 🛠️ Tech Stack
<p align="center">
  <img src="https://img.shields.io/badge/C-blue?style=for-the-badge&logo=c" />
  <img src="https://img.shields.io/badge/x86_Assembly-black?style=for-the-badge&logo=assemblyscript" />
  <img src="https://img.shields.io/badge/GCC-orange?style=for-the-badge&logo=gnu" />
  <img src="https://img.shields.io/badge/NASM-darkred?style=for-the-badge" />
  <img src="https://img.shields.io/badge/QEMU-purple?style=for-the-badge" />
</p>

---

## 🎮 Build & Run

### 🔑 Prerequisites
- GCC (32-bit support)
- NASM
- Make
- QEMU

### ▶️ Steps
```bash
git clone https://github.com/yourusername/minios.git
cd minios
make
make run
```

### 🧠 System Details
- Language: C (GNU99), x86 Assembly
- Architecture: 32-bit
- Boot Mode: Legacy BIOS
- Build System: Makefile

### 🧪 Makefile Commands
- ```make``` — Build the OS
- ```make clean``` — Clean build files
- ```make run``` — Run OS in QEMU
- ```make backup``` — Create project backup

### 🚀 Future Enhancements

- Multitasking support
- File system implementation
- Process scheduling
- Memory protection
- Basic networking

### Features

- Basic kernel implementation
- Memory management
- Simple shell interface
- Console and keyboard support
- System initialization
- Basic system utilities

## License

This project is licensed under the GPL-3.0 License - see the [gpl-3.0.txt](gpl-3.0.txt) file for details.

