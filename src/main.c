#include <stdlib.h>

#include "core.h"

int main (int argc, char **argv) 
{
	Core_t* core = malloc(sizeof(Core_t));
	uint8_t ram[65536];

	ram[0] = 0x38; // SEC
	ram[1] = 0xa5; // LDA zpg
	ram[2] = 0x0f;
	ram[3] = 0x6a; // ROR A

	ram[0xf] = 0x10;

	// Implement Z flag for bdc? (acts how it would in binary mode)
	// PHP pushes processor status register with break flag set?
	 
	for (int i = 0; i < 3; i++)
	{	
		exec_inst(core, ram);
	}

	return 1;	
}

