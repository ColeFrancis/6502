#include <iostream>
#include <fstream>
#include "extendedCore.h"

using namespace std;

void ExtendedCore::loadRam (const char *fileName)
{
        ifstream inFile(fileName, ios::binary | ios::ate);

        if (!inFile) {
                cerr << "Error, unable to open " << fileName << endl;
                exit(1);
        }

        std::streamsize fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        std::streamsize readSize = (fileSize > MEM_SIZE) ? MEM_SIZE : fileSize;

        if (!inFile.read(reinterpret_cast<char*>(ram), readSize)) {
                cerr << "Error in loading memory" << endl;
                exit(1);
        }
}

void ExtendedCore::dumpRam (const char* fileName)
{
        ofstream outFile(fileName, ios::binary);
        if (!outFile) {
                cerr << "Error, unable to open " << fileName << endl;
                exit(1);
        }

        outFile.write(reinterpret_cast<const char*>(ram), MEM_SIZE);
}