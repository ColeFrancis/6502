# Overview

## Introduction
An instruction-accurate emulator of the NMOS 6502 processor, written in C.

## Motivation
I started and finished this project with the goal to learn more about the instruction set archititecture and the functionality of the 6502 microprocessor. I wanted to better understand how it works and how it is used. My motivation was not to create a cycle-accurate emulator, but rather an instruction-accurate one.

# Features

## Accuracy
As was said before, this emulator was made to be instruction-accurate. All of the NMOS 6502 instructions are fully
implemented and functional. Clock cycles are not tracked with this emulator, rather one instruction is emulated with
each update of the processor.

## Performance

## Additional features

Something about hardware interrupts
Something about loading binary file into ram and ram into binary file

# Build

## Requirements

## Compilation

## Dependencies

# Usage

## Running

## Loading Ram

## Interupts

# Example programs?

# Technical details

## Core/code structure

## Cycle accuracy

## Input/output

# Known Issues

## Bugs

## Limitations

# References

# License

# Contact

## Author information

## Feedback
Emulator for the NMOS 6502 written in C

I did not implement the program counter like the true 6502
	
	When the 6502 is ready for the next instruction 
	it increments the program counter before fetching 
	the instruction. Once it has the op code, it increments 
	the program counter by the length of the operand, if any. 
