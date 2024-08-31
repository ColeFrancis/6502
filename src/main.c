#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "file_handler.h"

int main (int argc, char **argv) 
{
	uint8_t ram[MEM_SIZE];

	Core_t* core = calloc(1, sizeof(Core_t));
	if (core == NULL)
	{
		printf("Error: unable to allocate memory for 6502");
		return 1;
	}

	load_ram (argv[1], ram);

	core->RES_pin = 1;
	core->NMI_pin = 1;
	core->IRQ_pin = 1;
	
	for (long i = 0; i < 100; i++)
	{	
		decode_inst(core, ram);
	}

	dump_ram(argv[2], ram);

	return 1;	
}

