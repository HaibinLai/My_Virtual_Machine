# Implementing Virtual Machine Using C Language

This is a virtual machine written in C, supporting the RISC-V instruction set.

![img.png](image/Asm.png)

## How does a VM works?
A VM creates one standard CPU architecture which is simulated on various hardware devices. One advantage of a compiler is that it has no runtime overhead while a VM does. Even though compilers do a pretty good job, writing a new one that targets multiple platforms is very difficult, so VMs are still helpful here. 
In practice, VMs and compilers are mixed at various levels.

A VM is a program that acts like a computer. It simulates a CPU along with a few other hardware components, allowing it to perform arithmetic, read and write to memory, and interact with I/O devices, just like a physical computer. Most importantly, it can understand a machine language which you can use to program it.

## Features

- **Instruction Set**: Supports RISC-V
- **Build Tool**: Compiled using Makefile

## Installation

Make sure you have [Make](https://www.gnu.org/software/make/) and the necessary compilers installed.

```bash
git clone https://your-repo-url.git
cd your-repo-directory
make

````
## Usage

After compilation, you can run the virtual machine with the following command:

```bash
./your-vm-executable
```

```asm
.ORIG x3000                        ; this is the address in memory where the program will be loaded
LEA R0, HELLO_STR                  ; load the address of the HELLO_STR string into R0
PUTs                               ; output the string pointed to by R0 to the console
HALT                               ; halt the program
HELLO_STR .STRINGZ "Hello World!"  ; store this string here in the program
.END                               ; mark the end of the file
```

## Contributing

Contributions of any kind are welcome! Please feel free to submit issues or pull requests.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
```

Feel free to modify any part of it to better fit your project!