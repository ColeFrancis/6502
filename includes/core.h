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

void decode_group(Core_t *core, uint8_t *ram);

void decode_inst_00(Core_t *core, uint8_t *ram, uint8_t inst);
void decode_inst_01(Core_t *core, uint8_t *ram, uint8_t inst);
void decode_inst_10(Core_t *core, uint8_t *ram, uint8_t inst);

void exec_inst_00(Core_t *core, uint8_t *ram, uint8_t inst, uint8_t *val);
void exec_inst_01(Core_t *core, uint8_t *ram, uint8_t inst, uint8_t *val);
void exec_inst_10(Core_t *core, uint8_t *ram, uint8_t inst, uint8_t *val);

void exec_inst_old(Core_t *core, uint8_t *ram);

void adc_inst(Core_t *core, uint8_t num);
void sbc_inst(Core_t *core, uint8_t num);
void cmp_inst(Core_t *core, uint8_t num1, uint8_t num2);

#endif

