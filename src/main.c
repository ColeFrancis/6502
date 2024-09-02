#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "file_handler.h"

#include <sys/time.h>

struct timeval t0, t1;

int main (int argc, char **argv) 
{
	uint8_t ram[MEM_SIZE];

	Core_t* core = calloc(1, sizeof(Core_t));
	if (core == NULL)
	{
		printf("Error: unable to allocate memory for 6502");
		return 1;
	}

	//load_ram (argv[1], ram);

	core->RES_pin = 1;
	core->NMI_pin = 1;
	core->IRQ_pin = 1;
	
	ram[0] = 0xa5;
	ram[1] = 0xf0;

	ram[2] = 0x69;
	ram[3] = 0x02;

	ram[4] = 0x20;
	ram[5] = 0x00;
	ram[6] = 0x02;

	ram[7] = 0xa8;

	ram[8] = 0x68;

	ram[0x09] = 0x6c;
	ram[0x0a] = 0x00;
	ram[0x0b] = 0x03;

	ram[0xf0] = 0xff;

	ram[0x0200] = 0xe8;
	ram[0x0201] = 0x48;
	ram[0x0202] = 0x18;
	ram[0x0203] = 0x60;

	ram[0x0300] = 0x00;
	ram[0x0400] = 0x00;

	ram[0xfffc] = 0x00;
	ram[0xfffd] = 0x00;

	gettimeofday(&t0, NULL);
	long i;
	for (i = 0; i < 1000000000; i++)
	{	
		decode_inst(core, ram);
	}
	gettimeofday(&t1, NULL);

	printf("Took %.2g seconds to execute %ld instructions", t1.tv_sec - t0.tv_sec + 1E-6 * (t1.tv_usec - t0.tv_usec), i);

	//dump_ram(argv[2], ram);

	return 1;	
}

