#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdint.h>
#include "core.h"

void load_ram (char *file_name, uint8_t *ram);
void dump_ram (char* file_name, uint8_t *ram);

#endif
