# CHIP-8 Emulator

Interpreter of CHIP-8 language and emulator for COSMAC VIP and Telmac 1800 games

https://github.com/user-attachments/assets/dfbf374a-87c7-4267-8ba2-c4728a8ad5f3

https://github.com/user-attachments/assets/e2cc38e7-1116-485f-82a9-8c5a06be10b9

## Requirements

```bash
sudo apt update && sudo apt install libsdl2-dev libsdl2-mixer-dev
```

## Build

```bash
cmake -S . -B build && cmake --build build
```

## Run

```bash
cd bin/ && ./chip8-emulator [ROM_FILE_PATH]
```

## References
- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- https://en.wikipedia.org/wiki/CHIP-8
