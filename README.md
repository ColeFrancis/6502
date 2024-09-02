# Overview

## Introduction
An instruction-accurate emulator of the NMOS 6502 processor, written in C, with an included test program to evaluate the performance.

## Motivation
I started and finished this project with the goal to learn more about the instruction set archititecture and the functionality of the 6502 microprocessor. I wanted to better understand how it works and how it is used. My motivation was not to create a cycle-accurate emulator, but rather an instruction-accurate one.

# Features

## Accuracy
As was said before, this emulator was made to be instruction-accurate. All of the NMOS 6502 instructions are fully
implemented and functional. Clock cycles are not tracked with this emulator, rather one instruction is emulated with
each update of the processor.

## Performance
The core ran the test program ran at 8.3e6 instructions/sec or about 33MHz on an i7 3.2 GHz with 16GB of ram. The same program ran 5.3e6 instructions/sec or about 21 MHz on an i5 1.0 GHz with 8GB Windows machine running WSL.

## Additional features

Something about hardware interrupts
Something about loading binary file into ram and ram into binary file

# Build

## Requirements
Any system with a C compiler and the standard library will be able to run this emulator, though Linux is needed to run make and the gettimeofday function in the test program.
A C compiler such as gcc running c99 or newer with the standard library. Older versions have not been tested.

## Compilation
To compile, simply run make.

## Dependencies
There are no dependencies.

# Usage

## Running
The program can be compiled and ran as is if you would like to run the test program. The core can be used, however, in any other program or system emulator. To run the core, simply set the RST_pin to a value aboce zero, then execute the decode_inst() function for each instruction.

## Loading Ram
2 file processing functions are included in the file_handler header. load_ram() is used to load the contents of a binary file into the ram to be run through the core. dump_ram() can then be used to save the resulting contents of the ram into the same or a new binary file.

## Interupts
This core features the same interupts as the real 6502, namely the RST pin, the NMI pin, and the IRQ pin. To trigger an interupt or a reset, set the corresponding pun variable to 0 and run an instruction using the decode_inst() function. 

The NMI is edge-sensitive, triggering on the falling edge, or just the first cycle after the NMI_pin is set to zero. The IRQ is level-sensitive, triggering when the IRQ_pin and the interupt flag are set to zero. Resets are triggered by pulling the RST_pin low, then back to high.

# Technical details

## Core/code structure
The code structure of this emulator is a struct to organize the flags and registers, and switch statments to decode instructions. This allowed the code to be more usable, as well as simpler.

## Cycle accuracy
As stated above, this emulator is instruction accurate, not cycle accurate. The purpose of this project was simply to learn the ISA and 

## Input/output
Just as with the original 6502, this emulator fetures memory-mapped io, with the possibility of hardware interrupts.

# Known Issues

## Bugs
There are no currently known bugs. If any are found, please lset me know. My information can be found in the contact section.

## Limitations
The primary limitation of the emulator is not being cycle-accurate. Because of this, it would be very difficult to expand and use to emulate full systems, such as the NES or Comodor 64.

# References

# License
This code is covered under the standard MIT license.

# Contact

## Author information
You can contact me directly over Github, or through my linkedin: Cole Francis.

## Feedback
Please feel free to send me any feedback on the usability, readability, or maintainability of the code!