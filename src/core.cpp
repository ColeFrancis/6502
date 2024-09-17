#include <iostream>
#include "core.h"

using namespace std;

#define NZ_FLAGS(x) sr = (sr & 0x7f) | ((x) & 0x80); \
                    (x) ? (sr &= 0xfd) : (sr |= 0x2);

void Core::decodeGrp00()
{
        uint16_t addr;
        uint8_t addr_hi, addr_lo;

        uint8_t *val;

        // Addressing mode
        switch ((inst >> 2) & 0x07)
        {
                case 0x00: // #
			val = &ram[pc];

			pc++;
                        break;

                case 0x01: // zpg
                        val = &ram[ram[pc]];

                        pc++;
                        break;

                case 0x03: // abs
                        addr = ram[pc++] | (ram[pc] << 8);
                        val = &ram[addr];

                        pc++;
                        break;

                case 0x05: // zpg,X
                        val = &ram[0xFF & (ram[pc] + x)];

                        pc++;
                        break;

                case 0x07: // abs,X
                        addr = ram[pc++] | (ram[pc] << 8);
                        val = &ram[addr + x];

                        pc++;
                        break;
        }

        execInst00(val);
}

void Core::decodeGrp01()
{
        uint16_t addr;
        uint8_t addr_hi, addr_lo;

        uint8_t *val;

        // Addressing mode
        switch ((inst >> 2) & 0x07)
        {
                case 0x00: // X,ind
			addr_lo = ram[0xFF & (ram[pc] + x)];
                        addr_hi = ram[0xFF & (ram[pc] + x + 1)];

                        val = &ram[(addr_hi << 8) | addr_lo];

                        pc++;
                        break;

                case 0x01: // zpg
                        val = &ram[ram[pc]];

                        pc++;
                        break;
                        
                case 0x02: // #
			val = &ram[pc];

			pc++;
                        break;
                        
                case 0x03: // abs
                        addr = ram[pc++] | (ram[pc] << 8);
                        val = &ram[addr];

                        pc++;
                        break;
                        
                case 0x04: // ind,Y
                        addr_lo = ram[ram[pc]];
                        addr_hi = ram[0xFF & ram[pc] + 1];

                        val = &ram[((addr_hi << 8) | addr_lo) + y];

                        pc++;
                        break;
                        
                case 0x05: // zpg,X
                        val = &ram[0xFF & (ram[pc] + x)];  // & with 0xff to ignore carry bit moving reference into another page

                        pc++;
                        break;
                        
                case 0x06: // abs,Y
                        addr = ram[pc++] | (ram[pc] << 8);
                        val = &ram[addr + y];

                        pc++;
                        break;
                        
                case 0x07: // abs,X
                        addr = ram[pc++] | (ram[pc] << 8);
                        val = &ram[addr + x];

                        pc++;
                        break;
        }

        execInst01(val);
}

void Core::decodeGrp10()
{
        uint16_t addr;
        uint8_t addr_hi, addr_lo;


        uint8_t *val;

         // Save the address of our value rather than the value so we can change it with Store instructions
        
        // Addressing mode
        switch ((inst >> 2) & 0x07)
        {
                case 0x00: // #
			val = &ram[pc];

			pc++;
                        break;

                case 0x01: // zpg
                        val = &ram[ram[pc]];

                        pc++;
                        break;

                case 0x02: // A
                        val = &a;
                        break;

                case 0x03: // abs
                        addr = ram[pc++] | (ram[pc] << 8);
                        val = &ram[addr];

                        pc++;
                        break;

                case 0x05: // zpg,X (zpg,Y for STX and LDX)
                        if (inst == 0x96 || inst == 0xb6)
                                val = &ram[0xff & (ram[pc] + y)]; // & with 0xff to ignore carry bit moving reference into another page
                        
                        else
                                val = &ram[0xff & (ram[pc] + x)];;
                        
                        pc++;
                        break;

                case 0x07: // abs,X (abs,Y for LDX)
                        addr = ram[pc++] | (ram[pc] << 8);

                        if (inst == 0xbe)
                                val = &ram[addr + y];
                        
                        else
                                val = &ram[addr + x];
                        
                        pc++;
                        break;
        }

        execInst10(val);
}


void Core::execInst00(uint8_t *val)
{
        uint8_t num;

        switch (inst >> 5)
        {
                case 0x01: // BIT
                        num = a & *val;

                        sr = (sr & 0xbf) | (num & 0x40); // V flag
                        NZ_FLAGS(num);
                        break;

                case 0x02: // JMP

                        break;

                case 0x03: // JMP abs

                        break;

                case 0x04: // STY
                        *val = y;
                        break;

                case 0x05: // LDY
                        y = *val;

                        NZ_FLAGS(y);
                        break;

                case 0x06: // CPY
                        cmpInst(y, *val);
                        break;

                case 0x07: // CPX
                        cmpInst(x, *val);
                        break;
        }
}

void Core::execInst01(uint8_t *val)
{
        switch (inst >> 5)
        {
                case 0x00: // ORA
                        a |= *val;

                        NZ_FLAGS(a);
                        break;

                case 0x01: // AND
                        a &= *val;

                        NZ_FLAGS(a);
                        break;

                case 0x02: // EOR
                        a ^= *val;

                        NZ_FLAGS(a);
                        break;

                case 0x03: // ADC
                        adcInst(*val);
                        break;

                case 0x04: // STA
                        *val = a;
                        break;

                case 0x05: // LDA
                        a = *val;

                        NZ_FLAGS(a);
                        break;

                case 0x06: // CMP
                        cmpInst(a, *val);
                        break;

                case 0x07: // SBC
                        sbcInst(*val);
                        break;
        }
}

void Core::execInst10(uint8_t *val)
{
        uint8_t c_in;

        switch (inst >> 5)
        {
                case 0x00: // ASL
                        sr = (sr & 0xfe) | (*val >> 7);

                        *val <<= 1;

                        NZ_FLAGS(*val);
                        break;

                case 0x01: // ROL
                        c_in = sr & 0x01;

                        sr = (sr & 0xfe) | (*val >> 7);

                        *val <<= 1;
                        *val |= c_in; // Rotate in carry

                        NZ_FLAGS(*val);
                        break;

                case 0x02: // LSR
                        sr = (sr & 0xfe) | (*val & 0x01);

                        *val >>= 1;

                        NZ_FLAGS(*val);
                        break;

                case 0x03: // ROR
                        c_in = sr & 0x01;

                        sr = (sr & 0xfe) | (*val & 0x01);

                        *val >>= 1;
                        *val |= c_in << 7; // Rotate in carry

                        NZ_FLAGS(*val);
                        break;

                case 0x04: // STX
                        *val = x;
                        break;

                case 0x05: // LDX
                        x = *val;

                        NZ_FLAGS(x);
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

void Core::execInstBr()
{
        uint16_t offset;

        offset = ram[pc];

        if (offset & 0x80)
                offset |= 0xff00; // Convert 8 bit neg number to 16 bit neg number (two's comp.)

        switch (inst & 0xc0)
        {
                case 0x00: // N flag --- Tested
                        if ( (sr >> 7) == ((inst >> 5) & 1) )
                                pc += (offset - 1);
                        else    
                                pc++;
                        break;

                case 0x40: // V flag --- Tested
                        if ( ((sr >> 6) & 1) == ((inst >> 5) & 1) )
                                pc += (offset - 1);
                        else    
                                pc++;
                        break;

                case 0x80: // C flag --- Tested
                        if ( (sr & 1) == ((inst >> 5) & 1) )
                                pc += (offset - 1);
                        else    
                                pc++;
                        break;

                case 0xc0: // Z flag --- Tested
                        if ( ((sr >> 1) & 1) == ((inst >> 5) & 1) )
                                pc += (offset - 1);
                        else    
                                pc++;
                        break;
        }
}

void Core::execInstSb()
{
        if (inst == 0x98) // TYA --- Tested
        {
                a = y;
                NZ_FLAGS(a);

                return;
        }

        switch (inst & 0xc0)
        {
                case 0x00: // C bit --- Tested
                        sr = (sr & 0xfe) | ((inst >> 5) & 0x01);
                        break;

                case 0x40: // I bit --- Tested
                        sr = (sr & 0xfb) | ((inst >> 3) & 0x04);
                        break;

                case 0x80: // V bit --- Tested
                        sr = (sr & 0xbf) | ((inst << 1) & 0x04);
                        break;

                case 0xc0: // D bit --- Tested
                        sr = (sr & 0xf7) | ((inst >> 2) & 0x08);
                        break;
        }
}

void Core::execSp00()
{
        uint8_t hi_byte;
        // Inst TYA is found in exec_inst_sb
        // Inst DEX is found in exec_sp_10

        switch (inst)
        {
                case 0x00: // BRK
                        sr |= 0x30; // Break flag set
                        
                        coreIRQ();
                        break;

                case 0x20: // JSR --- Tested
                        ram[0x100 | sp] = (pc + 1) >> 8; // Push high byte of return addr-1
                        sp--;

                        ram[0x100 | sp] = (pc + 1); // Push low byte of return addr-1
                        sp--;
                        
                        pc = ram[pc++] | (ram[pc] << 8);
                        break;

                case 0x40: // RTI
                        sp++;
                        sr = ram[0x100 | sp]; // Pull flags
                        sr &= 0xcf; // Break flag clear

                        sp++;
                        pc = ram[0x100 | sp];

                        sp++;
                        pc |= ram[0x100 | sp] << 8;

                        break;

                case 0x60: // RTS --- Tested
                        sp++;
                        pc = ram[0x100 | sp];

                        sp++;
                        pc |= ram[0x100 | sp] << 8;

                        pc++;
                        break;

                case 0x08: // PHP --- Tested
                        sr |= 0x30; // Break flag set

                        ram[0x100 | sp] = sr;

                        sp--;
                        break;
                
                case 0x28: // PLP --- Tested
                        sp++;

                        sr = ram[0x100 | sp];

                        sr &= 0xcf; // Break flag clear
                        break;

                case 0x48: // PHA --- Tested
                        ram[0x100 | sp] = a;

                        sp--;
                        break;

                case 0x68: // PLA --- Tested
                        sp++;

                        a = ram[0x100 | sp];

                        NZ_FLAGS(a);
                        break;

                case 0x88: // DEY --- Tested
                        y--;
                        NZ_FLAGS(y);
                        break;

                case 0xa8: // TAY --- Tested
                        y = a;
                        NZ_FLAGS(y);
                        break;

                case 0xc8: // INY --- Tested
                        y++;
                        NZ_FLAGS(y);
                        break;

                case 0xe8: // INX --- Tested
                        x++;
                        NZ_FLAGS(x);
                        break;

                case 0x4c: // JMP abs --- Tested
                        pc = ram[pc++] | (ram[pc] << 8); 
                        break;

                case 0x6c: // JMP ind --- Tested
                        pc = ram[pc++] | (ram[pc] << 8); // Perform this once again below because its indirect
                        
                        ((pc & 0xff) == 0xff) ? // There is no carry on the indirect jump. The address does not change pages
                                (hi_byte = ram[pc - 0xff]) :
                                (hi_byte = ram[pc + 1]);
                                
                        pc = ram[pc] | (hi_byte << 8); 
                        break;

                default:
                        decodeGrp00();
        }
}

void Core::execSp10()
{
        switch (inst)
        {
                case 0x8a: // TXA -- Tested
                        a = x;
                        NZ_FLAGS(a);
                        break;

                case 0x9a: // TXS --- Tested
                        sp = x;
                        NZ_FLAGS(sp);
                        break;

                case 0xaa: // TAX --- Tested
                        x = a;
                        NZ_FLAGS(x);
                        break;

                case 0xba: // TSX --- Tested
                        x = sp;
                        NZ_FLAGS(x);
                        break;

                case 0xca: // DEX --- Tested
                        x--;
                        NZ_FLAGS(x);
                        break;

                case 0xea: // NOP --- Tested
                        break;

                default:
                        decodeGrp10();
        }
}


void Core::adcInst(uint8_t num)
{
        uint8_t test_v, c_in;
        uint8_t a_hi, num_hi;

        c_in = sr & 0x1;

        if (sr & 0x08) // BDC Mode
        {
                a_hi = a >> 4; // Isolate high nibbles
                num_hi = num >> 4;

                a &= 0x0f; // Isolate low nibbles
                num &= 0x0f;

                a += num + c_in;

                if (a > 0x09) // a..f must be hanged to 11..15
                        a += 0x06;


                a_hi += num_hi + (a >> 4);

                if (a_hi > 0x09) // a..f must be changed to 11..15
                        a_hi += 0x06;


                sr = (sr & 0xfe) | (a_hi >> 4); // Carry out
                a = (a & 0x0f) | (a_hi << 4);
        }
        else
        {
                if (a > 0xff - num) // Carry out
                        sr |= 0x1;
                else    
                        sr &= 0xfe;

                test_v = (a & 0x80) ^ (num & 0x80); // Overflow flag part 1

                a += num + c_in;

                if (!test_v)
                        sr |= ((num & 0x80) ^ (a & 0x80)) >> 1; // Overflow flag part 2
        }

        NZ_FLAGS(a);
}

void Core::sbcInst(uint8_t num)
{
        uint8_t test_v, c_in;
        uint8_t a_hi, num_hi;

        c_in = sr & 0x1;

        if (sr & 0x08) // BDC mode
        {
                a_hi = a >> 4; // Isolate high nibbles
                num_hi = num >> 4;

                a &= 0x0f; // Isolate low nibbles
                num &= 0x0f;

                a -= (num + c_in);
                
                if (a > 0x09) // Underflow test
                        a -= 0x06; 

                a_hi -= (num_hi + (a >> 7));

                if (a_hi > 0x09) // Underflow test
                        a_hi -= 0x06;

                sr = (sr & 0xfe) | (a_hi >> 7); // Carry out
                a = (a & 0x0f) | (a_hi << 4);
        }
        else
        {
                if (a < num) // Carry out
                        sr |= 0x1;
                else    
                        sr &= 0xfe;
                
                test_v = (a & 0x80) ^ (num & 0x80); // Overflow flag part 1
                
                a -= (num + c_in);

                if (!test_v)
                        sr |= ((num & 0x80) ^ (a & 0x80)) >> 1; // Overflow flag part 2
        }

        NZ_FLAGS(a);
}

void Core::cmpInst(uint8_t &num1, uint8_t num2)
{
        uint8_t c_in;

        c_in = sr & 0x1;

        (num1 < num2) ? 
                (sr |= 0x1) : 
                (sr &= 0xfe); // Carry out

        num1 -= (num2 + c_in);

        NZ_FLAGS(num1);
}


void Core::coreRST()
{
        sr |= 0x04; // Set I flag

        pc = (ram[0xfffd] << 8) | (ram[0xfffc]); // Load RES vector into PC
}

void Core::coreNMI()
{
        ram[0x100 | sp] = (pc) >> 8;
        sp--;

        ram[0x100 | sp] = (pc);
        sp--;

        ram[0x100 | sp] = sr;
        sp--;

        pc = (ram[0xfffb] << 8) | ram[0xfffa]; // Load NMI vector into PC
}

void Core::coreIRQ()
{
        ram[0x100 | sp] = (pc) >> 8;
        sp--;

        ram[0x100 | sp] = (pc);
        sp--;

        ram[0x100 | sp] = sr;
        sp--;

        sr |= 0x04; // Set I flag

        pc = (ram[0xffff] << 8) | ram[0xfffe]; // Load IRQ vector into PC
}


void Core::decodeInst()
{
        if (!prev_res)
        {
                if (!RES_pin)
                        return; // Remain in reset mode
                else
                        coreRST();
        }
        prev_res = RES_pin;
	
	inst = ram[pc];
	pc++;
        
        // Inst format: aaabbbcc 
        //      aaa: inst
        //      bbb: addressing mode
        //      cc: instruction group
	switch (inst & 0x03)
        {
                case 0x00:
                        if ((inst & 0x1f) == 0x10)
                                execInstBr();

                        else if ((inst & 0x1f) == 0x18)
                                execInstSb();

                        else if ((inst & 0x17) == 0)
                                execSp00();

                        else if ((inst & 0xdf) == 0x4c)
                                execSp00();
                        
                        else
                                decodeGrp00();
                        break;

                case 0x01:
                        decodeGrp01();
                        break;

                case 0x02:
                        if ((inst & 0x8f) == 0x8a)
                                execSp10();
                        
                        else
                                decodeGrp10();
                        
                        break;
        }

        if (prev_nmi && !NMI_pin)
                coreNMI();
                
        else if (!IRQ_pin && !(sr & 0x04))
                coreIRQ();

        prev_nmi = NMI_pin;
}