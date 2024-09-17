#ifndef CORE_H
#define CORE_H

#include <cstdint>

#define MEM_SIZE 65536

class Core {
	private: 
		uint8_t a;
		uint8_t x;
		uint8_t y;
		uint8_t sp;
		uint16_t pc;
		uint8_t sr;

		uint8_t prev_res;
		uint8_t prev_nmi;

		uint8_t NMI_pin;
		uint8_t IRQ_pin;
		uint8_t RES_pin;

		uint8_t inst;

		void decodeGrp00();
		void decodeGrp01();
		void decodeGrp10();

		void execInst00(uint8_t *val);
		void execInst01(uint8_t *val);
		void execInst10(uint8_t *val);
		void execInstBr(); // Branch instructions
		void execInstSb(); // Set bit instructions
		void execSp00(); // Execute special 00 instructions
		void execSp10(); // Execute special 10 instructions

		void adcInst(uint8_t num);
		void sbcInst(uint8_t num);
		void cmpInst(uint8_t &num1, uint8_t num2);

		void coreRST();
		void coreNMI();
		void coreIRQ();

	protected:
		uint8_t *ram;

	public:
		Core(uint8_t *p) : ram(p) {}

		void decodeInst();

		void setRES(uint8_t pin) {RES_pin = pin;}
		void setNMI(uint8_t pin) {NMI_pin = pin;}
		void setIRQ(uint8_t pin) {IRQ_pin = pin;}
};

#endif