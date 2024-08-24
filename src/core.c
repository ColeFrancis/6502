#include <stdint.h>
#include <stdio.h> // <-- Temporary, for testing

#include "core.h"

#define NZ_FLAGS(x) core->sr = (core->sr & 0x7f) | ((x) & 0x80); \
                    (x) ? (core->sr &= 0xfd) : (core->sr |= 0x2);

void exec_inst(Core_t *core, uint8_t *ram)
{
	uint8_t inst;
        uint16_t addr;
        uint8_t addr_hi, addr_lo;
        register uint8_t num, c_in;
	
	inst = ram[core->pc];

	printf("Inst: %x\n", inst);
	printf("PC: %x\n", core->pc);
	printf("\n");
        
	core->pc++;

	switch (inst)
	{
		case 0x00: //BRK imp
			break;

		case 0x10: // BPL rel
			break;

                case 0x20: // JSR abs
                        break;

                case 0x30: // BMI rel
                        break;

                case 0x40: // RTI impl
                        break;

                case 0x50: // BVC rel
                        break;

                case 0x60: // RTS impl
                        break;

                case 0x70: // BVS rel
                        break;

                case 0x90: // BCC rel
                        break;

                case 0xa0: // LDY # --- Tested
			core->y = ram[core->pc];

                        NZ_FLAGS(core->y);

			core->pc++;
                        break;

                case 0xb0: // BCS rel
                        break;

                case 0xc0: // CPY #
                        break;

                case 0xd0: // BNE rel
                        break;

                case 0xe0: // CPX #
                        break;

                case 0xf0: // BEQ rel
                        break;

		case 0x01: // ORA X,ind
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        core->a |= ram[(addr_hi << 8) | addr_lo];

                        NZ_FLAGS(core->a);

                        core->pc++;
			break;

                case 0x11: // ORA ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        core->a |= ram[((addr_hi << 8) | addr_lo) + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x21: // AND X,ind
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        core->a &= ram[(addr_hi << 8) | addr_lo];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x31: // AND ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        core->a &= ram[((addr_hi << 8) | addr_lo) + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x41: // EOR X,ind
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        core->a ^= ram[(addr_hi << 8) | addr_lo];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x51: // EOR ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        core->a ^= ram[((addr_hi << 8) | addr_lo) + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x61: // ADC X,ind
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        num = ram[(addr_hi << 8) | addr_lo];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x71: // ADC ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        num = ram[((addr_hi << 8) | addr_lo) + core->y];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x81: // STA X,ind -- Tested
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        ram[(addr_hi << 8) | addr_lo] = core->a;

                        core->pc++;
                        break;

                case 0x91: // STA ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        ram[((addr_hi << 8) | addr_lo) + core->y] = core->a;

                        core->pc++;
                        break;

                case 0xa1: // LDA X,ind --- Tested
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        core->a = ram[(addr_hi << 8) | addr_lo];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xb1: // LDA ind,Y --- Tested
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        core->a = ram[((addr_hi << 8) | addr_lo) + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xc1: // CMP X,ind
                        break;

                case 0xd1: // CMP ind,Y
                        break;

                case 0xe1: // SBC X,ind
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        num = ram[(addr_hi << 8) | addr_lo];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

                case 0xf1: // SBC ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        num = ram[((addr_hi << 8) | addr_lo) + core->y];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

		case 0xa2: // LDX # --- Tested
			core->x = ram[core->pc];

                        NZ_FLAGS(core->x);

			core->pc++;
			break;

		case 0x24: // BIT xpg
			break;

                case 0x84: // STY zpg
                        ram[ram[core->pc]] = core->y;

                        core->pc++;
                        break;

                case 0x94: // STY zpg,X
                        ram[0xFF & (ram[core->pc] + core->x)] = core->y;

                        core->pc++;
                        break;

                case 0xa4: // LDY zpg --- Tested
                        core->y = ram[ram[core->pc]];

                        NZ_FLAGS(core->y);

                        core->pc++;
                        break;

                case 0xb4: // LDY zpg,X --- Tested
                        core->y = ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(core->y);

                        core->pc++;
                        break;

                case 0xc4: // CPY zpg
                        break;

                case 0xe4: // CPX zpg
                        break;

		case 0x05: // ORA zpg
                        core->a |= ram[ram[core->pc]];

                        NZ_FLAGS(core->a);

                        core->pc++;
			break;

                case 0x15: // ORA zpg,X
                        core->a |= ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x25: // AND zpg
                        core->a &= ram[ram[core->pc]];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x35: // AND zpg,X
                        core->a &= ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x45: // EOR zpg
                        core->a ^= ram[ram[core->pc]];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x55:// EOR zpg,X
                        core->a ^= ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x65: // ADC zpg
                        num = ram[ram[core->pc]];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x75: // ADC zpg,X
                        num = ram[0xFF & (ram[core->pc] + core->x)];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x85: // STA zpg
                        ram[ram[core->pc]] = core->a;

                        core->pc++;
                        break;

                case 0x95: // STA zpg,X --- Tested
                        ram[0xFF & (ram[core->pc] + core->x)] = core->a;

                        core->pc++;
                        break;

                case 0xa5: // LDA zpg --- Tested
                        core->a = ram[ram[core->pc]];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xb5: // LDA zpg,X --- Tested
                        core->a = ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xc5: // CMP zpg
                        break;

                case 0xd5: // CMP zpg,X
                        break;

                case 0xe5: // SBC zpg
                        num = ram[ram[core->pc]];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

                case 0xf5: // SBC zpg,X
                        num = ram[0xFF & (ram[core->pc] + core->x)];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

		case 0x06: // ASL zpg -- Tested
                        num = ram[ram[core->pc]];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;

                        NZ_FLAGS(num);

                        ram[ram[core->pc]] = num;

                        core->pc++;
			break;

                case 0x16: // ASL zpg,X
                        num = ram[0xFF & (ram[core->pc] + core->x)];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;

                        NZ_FLAGS(num);

                        ram[0xFF & (ram[core->pc] + core->x)] = num;

                        core->pc++;
                        break;

                case 0x26: // ROL zpg -- Tested
                        c_in = core->sr & 0x01;

                        num = ram[ram[core->pc]];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;
                        num |= c_in; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[ram[core->pc]] = num;

                        core->pc++;
                        break;

                case 0x36: // ROL zpg,X
                        c_in = core->sr & 0x01;

                        num = ram[0xFF & (ram[core->pc] + core->x)];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;
                        num |= c_in; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[0xFF & (ram[core->pc] + core->x)] = num;

                        core->pc++;
                        break;

                case 0x46: // LSR zpg
                        num = ram[ram[core->pc]];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;

                        NZ_FLAGS(num);

                        ram[ram[core->pc]] = num;

                        core->pc++;
                        break;

                case 0x56: // LSR zpg,X
                        num = ram[0xFF & (ram[core->pc] + core->x)];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;

                        NZ_FLAGS(num);

                        ram[0xFF & (ram[core->pc] + core->x)] = num;

                        core->pc++;
                        break;

                case 0x66: // ROR zpg -- Tested
                        c_in = core->sr & 0x01;

                        num = ram[ram[core->pc]];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;
                        num |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[ram[core->pc]] = num;

                        core->pc++;
                        break;

                case 0x76: // ROR zpg,X
                        c_in = core->sr & 0x01;

                        num = ram[0xFF & (ram[core->pc] + core->x)];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;
                        num |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[0xFF & (ram[core->pc] + core->x)] = num;

                        core->pc++;
                        break;

                case 0x86: // STX zpg
                        ram[ram[core->pc]] = core->x;

                        core->pc++;
                        break;

                case 0x96: // STX zpg,Y
                        ram[0xFF & (ram[core->pc] + core->y)] = core->x;

                        core->pc++;
                        break;

                case 0xa6: // LDX zpg --- Tested
                        core->x = ram[ram[core->pc]];

                        NZ_FLAGS(core->x);

                        core->pc++;
                        break;

                case 0xb6: // LDX zpg,Y --- Tested
                        core->x = ram[0xFF & (ram[core->pc] + core->y)];

                        NZ_FLAGS(core->x);

                        core->pc++;
                        break;

                case 0xc6: // DEC zpg
                        num = --ram[ram[core->pc]];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

                case 0xd6: // DEC zpg,X
                        num = --ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

                case 0xe6: // INC zpg
                        num = ++ram[ram[core->pc]];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

                case 0xf6: // INC zpg,X
                        num = ++ram[0xFF & (ram[core->pc] + core->x)];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

		case 0x08: // PHP imp --- Tested
                        ram[0x100 | core->sp] = core->sr;

                        core->sp--;
			break;

                case 0x18: // CLC impl --- Tested
                        core->sr &= 0xfe;
                        break;

                case 0x28: // PLP impl --- Tested
                        core->sp++;

                        core->sr = ram[0x100 | core->sp];
                        break;

                case 0x38: // SEC impl
                        core->sr |= 0x1;
                        break;

                case 0x48: // PHA impl --- Tested
                        ram[0x100 | core->sp] = core->a;

                        core->sp--;
                        break;

                case 0x58: // CLI impl -- Tested
                        core->sr &= 0xfb;
                        break;

                case 0x68: // PLA impl
                        core->sp++;

                        core->a = ram[0x100 | core->sp];

                        NZ_FLAGS(core->a);

                        break;

                case 0x78: // SEI immpl --- Tested
                        core->sr |= 0x04;
                        break;

                case 0x88: // DEY impl
                        core->y--;

                        NZ_FLAGS(core->y);

                        break;

                case 0x98: // TYA impl
                        core->a = core->y;

                        NZ_FLAGS(core->a);

                        break;

                case 0xa8: // TAY impl
                        core->y = core->a;

                        NZ_FLAGS(core->y);

                        break;

                case 0xb8: // CLV impl --- Tested
                        core->sr &= 0xbf;
                        break;

                case 0xc8: // INY  impl
                        core->y++;

                        NZ_FLAGS(core->y);

                        break;

                case 0xd8: // CLD impl --- Tested
                        core->sr &= 0xf7;
                        break;

                case 0xe8: // INX impl
                        core->x++;

                        NZ_FLAGS(core->x);

                        break;

                case 0xf8: // SED impl --- Tested
                        core->sr |= 0x08;
                        break;

		case 0x09: // ORA # --- Tested
			core->a |= ram[core->pc];

                        NZ_FLAGS(core->a);

			core->pc++;
			break;

                case 0x19: // ORA abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a |= ram[addr + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x29: // AND # --- Tested
			core->a &= ram[core->pc];

                        NZ_FLAGS(core->a);

			core->pc++;
                        break;

                case 0x39: // AND abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a &= ram[addr + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x49: // EOR # --- Tested
			core->a ^= ram[core->pc];

                        NZ_FLAGS(core->a);

			core->pc++;
                        break;

                case 0x59: // EOR abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a ^= ram[addr + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x69: // ADC # --- Tested
			num = ram[core->pc];

                        adc_inst(core, num);

			core->pc++;
                        break;

                case 0x79: // ADC abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->y];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x99: // STA abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        ram[addr + core->y] = core->a;

                        core->pc++;
                        break;

                case 0xa9: // LDA # --- Tested
			core->a = ram[core->pc];

                        NZ_FLAGS(core->a);

			core->pc++;
                        break;

                case 0xb9: // LDA abs,Y --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a = ram[addr + core->y];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xc9: // CMP #
                        break;

                case 0xd9: // CMP abs,Y
                        break;

                case 0xe9: // SBC #
			num = ram[core->pc];

                        sbc_inst(core, num);

			core->pc++;
                        break;

                case 0xf9: // SBC abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->y];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

		case 0x0a: // ASL A --- Tested
                        core->sr = (core->sr & 0xfe) | (core->a >> 7);

                        core->a <<= 1;

                        NZ_FLAGS(core->a);
			break;

                case 0x2a: // ROL A
                        c_in = core->sr & 0x01;

                        core->sr = (core->sr & 0xfe) | (core->a >> 7);

                        core->a <<= 1;
                        core->a |= c_in; // Rotate in carry

                        NZ_FLAGS(core->a);
                        break;

                case 0x4a: // LSR A
                        core->sr = (core->sr & 0xfe) | (core->a & 0x01);

                        core->a >>= 1;

                        NZ_FLAGS(core->a);
                        break;

                case 0x6a: // ROR A --- Tested
                        c_in = core->sr & 0x01;

                        core->sr = (core->sr & 0xfe) | (core->a & 0x01);

                        core->a >>= 1;
                        core->a |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(core->a);
                        break;

                case 0x8a: // TXA impl
                        core->a = core->x;

                        NZ_FLAGS(core->a);
                        break;

                case 0x9a: // TXS impl --- Tested
                        core->sp = core->x;

                        NZ_FLAGS(core->sp);

                        break;

                case 0xaa: // TAX impl --- Tested
                        core->x = core->a;

                        NZ_FLAGS(core->x);

                        break;

                case 0xba: // TSX impl
                        core->x = core->sp;

                        NZ_FLAGS(core->x);

                        break;

                case 0xca: // DEX impl --- Tested
                        core->x--;

                        NZ_FLAGS(core->x);

                        break;

                case 0xea: // NOP impl
                        break;

		case 0x2c: // BIT abs
			break;

                case 0x4c: // JMP abs
                        break;

                case 0x6c: // JMP ind
                        break;

                case 0x8c: // STY abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        ram[addr] = core->y;

                        core->pc++;
                        break;

                case 0xac: // LDY abs --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->y = ram[addr];

                        NZ_FLAGS(core->y);

                        core->pc++;
                        break;

                case 0xbc: // LDY abs,X --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->y = ram[addr + core->x];

                        NZ_FLAGS(core->y);

                        core->pc++;
                        break;

                case 0xcc: // CPY abs
                        break;

                case 0xec: // CPX abs
                        break;

		case 0x0d: // ORA abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a |= ram[addr];

                        NZ_FLAGS(core->a);

                        core->pc++;
			break;

                case 0x1d: // ORA abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a |= ram[addr + core->x];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x2d: // AND abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a &= ram[addr];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x3d: // AND abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a &= ram[addr + core->x];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x4d: // EOR abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a ^= ram[addr];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x5d: // EOR abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a ^= ram[addr + core->x];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0x6d: // ADC abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x7d: // ADC abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->x];

                        adc_inst(core, num);

                        core->pc++;
                        break;

                case 0x8d: // STA abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        ram[addr] = core->a;

                        core->pc++;
                        break;

                case 0x9d: // STA abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        ram[addr + core->x] = core->a;

                        core->pc++;
                        break;

                case 0xad: // LDA abs --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a = ram[addr];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xbd: // LDA abs,X --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->a = ram[addr + core->x];

                        NZ_FLAGS(core->a);

                        core->pc++;
                        break;

                case 0xcd: // CMP abs
                        break;

                case 0xdd: // CMP abs,X
                        break;

                case 0xed:// SBC abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

                case 0xfd: // SBC abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->x];

                        sbc_inst(core, num);

                        core->pc++;
                        break;

		case 0x0e: // ASL abs --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;

                        NZ_FLAGS(num);

                        ram[addr] = num;

                        core->pc++;
			break;

                case 0x1e: // ASL abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->x];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;

                        NZ_FLAGS(num);

                        ram[addr + core->x] = num;

                        core->pc++;
                        break;

                case 0x2e: // ROL abs
                        c_in = core->sr & 0x01;

                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;
                        num |= c_in; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[addr] = num;

                        core->pc++;
                        break;

                case 0x3e: // ROL abs,X
                        c_in = core->sr & 0x01;

                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->x];
                        core->sr = (core->sr & 0xfe) | (num >> 7);

                        num <<= 1;
                        num |= c_in; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[addr + core->x] = num;

                        core->pc++;
                        break;

                case 0x4e: // LSR abs --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;

                        NZ_FLAGS(num);

                        ram[addr] = num;

                        core->pc++;
                        break;

                case 0x5e: // LSR abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->x];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;

                        NZ_FLAGS(num);

                        ram[addr + core->x] = num;

                        core->pc++;
                        break;

                case 0x6e: // ROR abs
                        c_in = core->sr & 0x01;

                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;
                        num |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[addr] = num;

                        core->pc++;
                        break;

                case 0x7e: // ROR abs,X
                        c_in = core->sr & 0x01;

                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ram[addr + core->x];
                        core->sr = (core->sr & 0xfe) | (num & 0x01);

                        num >>= 1;
                        num |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(num);

                        ram[addr + core->x] = num;

                        core->pc++;
                        break;

                case 0x8e: // STX abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        ram[addr] = core->x;

                        core->pc++;
                        break;

                case 0xae: // LDX abs --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->x = ram[addr];

                        NZ_FLAGS(core->x);

                        core->pc++;
                        break;

                case 0xbe: // LDX abs,Y --- Tested
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        core->x = ram[addr + core->y];

                        NZ_FLAGS(core->x);

                        core->pc++;
                        break;

                case 0xce: // DEC abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = --ram[addr];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

                case 0xde: // DEC abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = --ram[addr + core->x];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

                case 0xee: // INC abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ++ram[addr];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;

                case 0xfe: // INC abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        num = ++ram[addr + core->x];

                        NZ_FLAGS(num);

                        core->pc++;
                        break;
	}

	printf("Acc: %x\n", core->a);
	printf("X: %x\n", core->x);
        printf("Y: %x\n", core->y);

        printf("NV-BDIZC: ");
        printf("%x", (core->sr >> 7) & 0x1);
        printf("%x", (core->sr >> 6) & 0x1);
        printf("-");
        printf("%x", (core->sr >> 4) & 0x1);
        printf("%x", (core->sr >> 3) & 0x1);
        printf("%x", (core->sr >> 2) & 0x1);
        printf("%x", (core->sr >> 1) & 0x1);
        printf("%x", (core->sr >> 0) & 0x1);
        printf("\n");

        printf("Stack Pointer: %x\n", core->sp);
        
        printf("\n");

        for (uint8_t i = 0xFF; i > 0xF9; i--)
        {
                printf("Stack 0x%x: %x\n", i, ram[0x100 | i]);
        }

	printf("\n");
}

void adc_inst(Core_t *core, uint8_t num)
{
        uint8_t test_v, c_in;
        uint8_t a_hi, num_hi;

        c_in = core->sr & 0x1;

        if (core->sr & 0x08) // BDC Mode
        {
                a_hi = core->a >> 4; // Isolate high nibble
                num_hi = num >> 4; // Isolate high nibble

                core->a &= 0x0f; // Isolate low nibble
                num &= 0x0f; // Isolate low nibble

                core->a += num + c_in;

                if (core->a > 0x09) // a..f must be hanged to 11..15
                        core->a += 0x06;


                a_hi += num_hi + (core->a >> 4);

                if (a_hi > 0x09) // a..f must be changed to 11..15
                        a_hi += 0x06;


                core->sr = (core->sr & 0xfe) | (a_hi >> 4); // Carry out
                core->a = (core->a & 0x0f) | (a_hi << 4);
        }
        else
        {
                if (core->a > 0xff - num) // Carry out
                        core->sr |= 0x1;
                else    
                        core->sr &= 0xfe;

                test_v = (core->a & 0x80) ^ (num & 0x80); // Overflow flag 1

                core->a += num + c_in;

                if (!test_v)
                        core->sr |= ((num & 0x80) ^ (core->a & 0x80)) >> 1; // Overflow flag 2
        }

        NZ_FLAGS(core->a);
}

void sbc_inst(Core_t *core, uint8_t num)
{
        uint8_t test_v, c_in;
        uint8_t a_hi, num_hi;

        c_in = core->sr & 0x1;

        if (core->sr & 0x08) // BDC mode
        {
                a_hi = core->a >> 4; // Isolate high nibble
                num_hi = num >> 4; // Isolate high nibble

                core->a &= 0x0f; // Isolate low nibble
                num &= 0x0f; // Isolate low nibble

                core->a -= (num + c_in);
                
                if (core->a > 0x09) // Underflow test
                        core->a -= 0x06; 

                a_hi -= (num_hi + (core->a >> 7));

                if (a_hi > 0x09) // Underflow test
                        a_hi -= 0x06;

                core->sr = (core->sr & 0xfe) | (a_hi >> 7); // Carry out
                core->a = (core->a & 0x0f) | (a_hi << 4);
        }
        else
        {
                if (core->a < num) // Carry out
                        core->sr |= 0x1;
                else    
                        core->sr &= 0xfe;
                
                test_v = (core->a & 0x80) ^ (num & 0x80); // Overflow flag 1
                
                core->a -= (num + c_in);

                if (!test_v)
                        core->sr |= ((num & 0x80) ^ (core->a & 0x80)) >> 1; // Overflow flag 2
        }

        NZ_FLAGS(core->a);
}
