#include <stdlib.h>

#include "core.h"

int main (int argc, char **argv) 
{
	Core_t* core = malloc(sizeof(Core_t));
	uint8_t ram[MAX_MEM_SIZE];

	core->RES_pin = 1;

	core->sp = 0xff;

	// Test ALL opcodes

	ram[0xfffa] = 0x00; // NMI jump vector
	ram[0xfffb] = 0x00;
	ram[0xfffc] = 0x00; // RES jump vector
	ram[0xfffd] = 0x00;
	ram[0xfffe] = 0x00; // IRQ jump vector
	ram[0xffff] = 0x00;
	
	for (long i = 0; i < 100; i++)
	{	
		decode_inst(core, ram);
	}

	return 1;	
}

