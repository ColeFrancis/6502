#include <stdlib.h>

#include "core.h"

int main (int argc, char **argv) 
{
	Core_t* core = malloc(sizeof(Core_t));
	uint8_t ram[MAX_MEM_SIZE];

	core->RES_pin = 1;
	core->NMI_pin = 1;
	core->IRQ_pin = 1;
	core->sp = 0xff;

	core->y = 0x02;

	ram[0] = 0xb6;
	ram[1] = 0x80;
	ram[3] = 0xa9;
	ram[3] = 0xa9;
	ram[4] = 0x33;

	ram[0x80] = 0xbb;
	
	ram[0x81] = 0x33;
	ram[0x82] = 0xaa;



	// LDA #: 0xa9
	// NOP: 0xea
	// SED: 0xf8

	// Test ALL opcodes and interrupts

	ram[0xfffa] = 0x00; // NMI vector
	ram[0xfffb] = 0x00;
	ram[0xfffc] = 0x00; // RES vector
	ram[0xfffd] = 0x00;
	ram[0xfffe] = 0x00; // IRQ vector
	ram[0xffff] = 0x00;
	
	for (long i = 0; i < 2; i++)
	{	
		decode_inst(core, ram);
	}

	return 1;	
}

