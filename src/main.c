#include <stdlib.h>

#include "core.h"

int main (int argc, char **argv) 
{
	Core_t* core = malloc(sizeof(Core_t));
	uint8_t ram[65536];
	
	ram[0] = 0xa9; // LDA #
	ram[1] = 0x01;
	ram[2] = 0x49; // AND #
	ram[3] = 0x03;

	// Implement Z flag for bdc? (acts how it would in binary mode)
	// PHP pushes processor status register with break flag set?
	 
	for (int i = 0; i < 4; i++)
	{	
		exec_inst(core, ram);
	}

	return 1;	
}

