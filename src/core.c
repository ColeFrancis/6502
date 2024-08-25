#include <stdint.h>
#include <stdio.h> // <-- Temporary, for testing

#include "core.h"

#define DEBUG

#define NZ_FLAGS(x) core->sr = (core->sr & 0x7f) | ((x) & 0x80); \
                    (x) ? (core->sr &= 0xfd) : (core->sr |= 0x2);

void decode_group(Core_t *core, uint8_t *ram)
{
        uint8_t inst;
	
	inst = ram[core->pc];

        #ifdef DEBUG
                printf("Inst: %x\n", inst);
                printf("PC: %x\n", core->pc);
                printf("\n");
        #endif
        
	core->pc++;

        // Inst format: aaabbbcc 
        //      aaa: inst
        //      bbb: addressing mode
        //      cc: instruction group
	switch (inst & 0x03)
        {
                case 0x00:
                        decode_inst_00(core, ram, inst);
                        break;

                case 0x01:
                        decode_inst_01(core, ram, inst);
                        break;

                case 0x02:
                        decode_inst_10(core, ram, inst);
                        break;
        }

        #ifdef DEBUG
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
        #endif
}

void decode_inst_00(Core_t *core, uint8_t *ram, uint8_t inst)
{
        uint16_t addr;
        uint8_t addr_hi, addr_lo;

        uint8_t *val; // Save the address of our value rather than the value so we can change it with Store instructions

        // Addressing mode
        switch ((inst >> 2) & 0x07)
        {
                case 0x00: // #
			val = &ram[core->pc];

			core->pc++;
                        break;

                case 0x01: // zpg
                        val = &ram[ram[core->pc]];

                        core->pc++;
                        break;

                case 0x03: // abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        val = &ram[addr];

                        core->pc++;
                        break;

                case 0x05: // zpg,X
                        val = &ram[0xFF & (ram[core->pc] + core->x)];

                        core->pc++;
                        break;

                case 0x07: // abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        val = &ram[addr + core->x];

                        core->pc++;
                        break;
        }

        exec_inst_00(core, ram, inst, val);
}

void decode_inst_01(Core_t *core, uint8_t *ram, uint8_t inst)
{
        uint16_t addr;
        uint8_t addr_hi, addr_lo;

        uint8_t *val; // Save the address of our value rather than the value so we can change it with Store instructions

        // Addressing mode
        switch ((inst >> 2) & 0x07)
        {
                case 0x00: // X,ind
			addr_lo = ram[0xFF & (ram[core->pc] + core->x)];
                        addr_hi = ram[0xFF & (ram[core->pc] + core->x + 1)];

                        val = &ram[(addr_hi << 8) | addr_lo];

                        core->pc++;
                        break;

                case 0x01: // zpg
                        val = &ram[ram[core->pc]];

                        core->pc++;
                        break;
                        
                case 0x02: // #
			val = &ram[core->pc];

			core->pc++;
                        break;
                        
                case 0x03: // abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        val = &ram[addr];

                        core->pc++;
                        break;
                        
                case 0x04: // ind,Y
                        addr_lo = ram[ram[core->pc]];
                        addr_hi = ram[0xFF & ram[core->pc] + 1];

                        val = &ram[((addr_hi << 8) | addr_lo) + core->y];

                        core->pc++;
                        break;
                        
                case 0x05: // zpg,X
                        val = &ram[0xFF & (ram[core->pc] + core->x)];

                        core->pc++;
                        break;
                        
                case 0x06: // abs,Y
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        val = &ram[addr + core->y];

                        core->pc++;
                        break;
                        
                case 0x07: // abs,X
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        val = &ram[addr + core->x];

                        core->pc++;
                        break;
        }

        exec_inst_01(core, ram, inst, val);
}

void decode_inst_10(Core_t *core, uint8_t *ram, uint8_t inst)
{
        uint16_t addr;
        uint8_t addr_hi, addr_lo;

        uint8_t *val; // Save the address of our value rather than the value so we can change it with Store instructions
        
        // Addressing mode
        switch ((inst >> 2) & 0x07)
        {
                case 0x00: // #
			val = &ram[core->pc];

			core->pc++;
                        break;

                case 0x01: // zpg
                        val = &ram[ram[core->pc]];

                        core->pc++;
                        break;

                case 0x02: // A
                        val = &(core->a);
                        break;

                case 0x03: // abs
                        addr = ram[core->pc++] | (ram[core->pc] << 8);
                        val = &ram[addr];

                        core->pc++;
                        break;

                case 0x04: // zpg,X (zpg,Y for STX and LDX)
                        if (inst & 0xe0 == 0x80 || inst & 0xe0 == 0x90)
                                val = &ram[0xFF & (ram[core->pc] + core->y)];

                        else
                                val = &ram[0xFF & (ram[core->pc] + core->x)];;

                        core->pc++;
                        break;

                case 0x07: // abs,X (abs,Y for LDX)
                        addr = ram[core->pc++] | (ram[core->pc] << 8);

                        if (inst & 0xe0 == 0x90)
                                val = &ram[addr + core->y];

                        else
                                val = &ram[addr + core->x];

                        core->pc++;
                        break;
        }

        exec_inst_10(core, ram, inst, val);
}

void exec_inst_00(Core_t *core, uint8_t *ram, uint8_t inst, uint8_t *val)
{
        uint8_t num;

        switch (inst >> 5)
        {
                case 0x01: // BIT
                        num = core->a & *val;

                        core->sr = (core->sr & 0xbf) | (num & 0x40); // V flag
                        NZ_FLAGS(num);
                        break;

                case 0x04: // STY
                        *val = core->y;
                        break;

                case 0x05: // LDY
                        core->x = *val;

                        NZ_FLAGS(core->y);
                        break;

                case 0x06: // CPY
                        cmp_inst(core, core->y, *val);
                        break;

                case 0x07: // CPX
                        cmp_inst(core, core->x, *val);
                        break;
        }
}

void exec_inst_01(Core_t *core, uint8_t *ram, uint8_t inst, uint8_t *val)
{
        switch (inst >> 5)
        {
                case 0x00: // ORA
                        core->a |= *val;

                        NZ_FLAGS(core->a);
                        break;

                case 0x01: // AND
                        core->a &= *val;

                        NZ_FLAGS(core->a);
                        break;

                case 0x02: // EOR
                        core->a ^= *val;

                        NZ_FLAGS(core->a);
                        break;

                case 0x03: // ADC
                        adc_inst(core, *val);
                        break;

                case 0x04: // STA
                        *val = core->a;
                        break;

                case 0x05: // LDA
                        core->a = *val;

                        NZ_FLAGS(core->a);
                        break;

                case 0x06: // CMP
                        cmp_inst(core, core->a, *val);
                        break;

                case 0x07: // SBC
                        sbc_inst(core, *val);
                        break;
        }
}

void exec_inst_10(Core_t *core, uint8_t *ram, uint8_t inst, uint8_t *val)
{
        uint8_t c_in;

        switch (inst >> 5)
        {
                case 0x00: // ASL
                        core->sr = (core->sr & 0xfe) | (*val >> 7);

                        *val <<= 1;

                        NZ_FLAGS(*val);
                        break;

                case 0x01: // ROL
                        c_in = core->sr & 0x01;

                        core->sr = (core->sr & 0xfe) | (*val >> 7);

                        *val <<= 1;
                        *val |= c_in; // Rotate in carry

                        NZ_FLAGS(*val);
                        break;

                case 0x02: // LSR
                        core->sr = (core->sr & 0xfe) | (*val & 0x01);

                        *val >>= 1;

                        NZ_FLAGS(*val);
                        break;

                case 0x03: // ROR
                        c_in = core->sr & 0x01;

                        core->sr = (core->sr & 0xfe) | (*val & 0x01);

                        *val >>= 1;
                        *val |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(*val);
                        break;

                case 0x04: // STX
                        *val = core->x;
                        break;

                case 0x05: // LDX
                        core->x = *val;

                        NZ_FLAGS(core->x);
                        break;

                case 0x06: // DEC
                        (*val)--;

                        NZ_FLAGS(*val);
                        break;

                case 0x07: // INC
                        (*val)++;

                        NZ_FLAGS(*val);
                        break;
        }
}

void exec_inst_old(Core_t *core, uint8_t *ram)
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

void cmp_inst(Core_t *core, uint8_t num1, uint8_t num2)
{
        uint8_t c_in;

        c_in = core->sr & 0x1;

        (num1 < num2) ? 
                (core->sr |= 0x1) : 
                (core->sr &= 0xfe); // Carry out

        num1 -= (num2 + c_in);

        NZ_FLAGS(num1);
}