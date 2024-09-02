#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#define MEM_SIZE 65536

typedef struct {
	uint8_t a;

	uint8_t x;
	uint8_t y;
	
	uint8_t sp;
	uint16_t pc;

	uint8_t sr;

	uint8_t prev_res;
	uint8_t prev_nmi;

	// For realistic emulation, these 3 pins are the only 3 which should be
	//		changed outside of these functions
	// To start the core, set all to > 0. To trigger any of the interups, 
	//		pull the corresponding pin to 0, then run decode_inst() to emulate 
	//		a single instruction
	uint8_t NMI_pin;
	uint8_t IRQ_pin;
	uint8_t RES_pin;
} Core_t;

void decode_inst(Core_t *core, uint8_t *ram);

void decode_grp_00(Core_t *core, uint8_t *ram, uint8_t inst);
void decode_grp_01(Core_t *core, uint8_t *ram, uint8_t inst);
void decode_grp_10(Core_t *core, uint8_t *ram, uint8_t inst);

void exec_inst_00(Core_t *core, uint8_t inst, uint8_t *val);
void exec_inst_01(Core_t *core, uint8_t inst, uint8_t *val);
void exec_inst_10(Core_t *core, uint8_t inst, uint8_t *val);
void exec_inst_br(Core_t *core, uint8_t *ram, uint8_t inst); // Branch instructions
void exec_inst_sb(Core_t *core, uint8_t inst); // Set bit instructions
void exec_sp_00(Core_t *core, uint8_t *ram, uint8_t inst); // Execute special 00 instructions
void exec_sp_10(Core_t *core, uint8_t *ram, uint8_t inst); // Execute special 10 instructions

void adc_inst(Core_t *core, uint8_t num);
void sbc_inst(Core_t *core, uint8_t num);
void cmp_inst(Core_t *core, uint8_t num1, uint8_t num2);

void core_rst(Core_t *core, uint8_t *ram);
void core_nmi(Core_t *core, uint8_t *ram);
void core_irq(Core_t *core, uint8_t *ram);

#endif