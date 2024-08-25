#include <stdlib.h>

#include "core.h"

int main (int argc, char **argv) 
{
	Core_t* core = malloc(sizeof(Core_t));
	uint8_t ram[65536];

	core->a = 0xc0;

	ram[0] = 0x6c;
	ram[1] = 0x0a;
	ram[2] = 0x01;

	ram[0x010a] = 0xa9;
	ram[0x010b] = 0x0f;

	// Implement Z flag for bdc? (acts how it would in binary mode)
	// PHP pushes processor status register with break flag set?
	 
	for (int i = 0; i < 2; i++)
	{	
		//exec_inst_old(core, ram);
		decode_group(core, ram);
	}

	return 1;	
}

