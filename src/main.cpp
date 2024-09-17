#include <iostream>
#include <chrono>
#include "extendedCore.h"

using namespace std;

int main (int argc, char **argv) 
{
	long i;
	uint8_t ram[MEM_SIZE] = {0};

	ExtendedCore core(ram);

	core.setRES(1);
	core.setNMI(1);
	core.setIRQ(1);

	// Load a testprogram from commandline parameter
	if (argc < 2)
	{
		cout << "Warning: a binary file must be included for this test program" << endl;
		exit(1);
	}
	core.loadRam(argv[1]);

	// Test time to execute 1 billion instructions
	auto start = chrono::high_resolution_clock::now();
	for (i = 0; i < 1000000000; i++)
	{	
		core.decodeInst();
	}
	auto end = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    cout << "Took " << duration << " seconds to execute " << i << " instructions." << endl;
	
	return 1;	
}

