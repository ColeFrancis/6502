#ifndef EXTENDED_CORE_H
#define EXTENDED_CORE_H

#include "core.h"

class ExtendedCore : public Core{
    public:
        ExtendedCore(uint8_t *p) : Core(p) {}

        void loadRam (const char *fileName);
        void dumpRam (const char *fileName);
};

#endif
