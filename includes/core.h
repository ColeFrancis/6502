#ifndef CORE_H
#define CORE_H

#include <stdint.h>

typedef struct {
	uint8_t a;

	uint8_t x;
	uint8_t y;
	
	uint8_t sp;
	uint16_t pc;

	uint8_t sr;
} Core_t;

void exec_inst(Core_t *core, uint8_t *ram);

void adc_inst(Core_t *core, uint8_t num);
void sbc_inst(Core_t *core, uint8_t num);

#endif

