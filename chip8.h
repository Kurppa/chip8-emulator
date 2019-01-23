#ifndef CHIP8_H
#define CHIP8_H

#include <fstream>
#include <iostream>
#include <cstring>
#include <iostream>
#include <stdio.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;

class Chip8
{
private:
	
	BYTE memory[4096];
	BYTE V[16];
	WORD I;
	WORD pc;
	BYTE delay_timer;
	BYTE sound_timer;
	WORD stack[16];
	WORD sp;

public:
	bool draw_flag = true;
	BYTE key[16];
	BYTE gfx[32*64];

	Chip8();
	~Chip8();
	bool load_program(const char * gameFile);
	void emulate_cycle();
	WORD get_opcode();

	//opcodes
	void opcode0NNN(WORD opcode); //not implemented
	void opcodeOOEO(WORD opcode);
	void opcode00EE(WORD opcode);
	void opcode1NNN(WORD opcode);
	void opcode2NNN(WORD opcode);
	void opcode3XNN(WORD opcode);
	void opcode4XNN(WORD opcode);
	void opcode5XYO(WORD opcode);
	void opcode6XNN(WORD opcode);
	void opcode7XNN(WORD opcode);
	void opcode8XY0(WORD opcode);
	void opcode8XY1(WORD opcode);
	void opcode8XY2(WORD opcode);
	void opcode8XY3(WORD opcode);
	void opcode8XY4(WORD opcode);
	void opcode8XY5(WORD opcode);
	void opcode8XY6(WORD opcode);
	void opcode8XY7(WORD opcode);
	void opcode8XYE(WORD opcode);
	void opcode9XY0(WORD opcode);
	void opcodeANNN(WORD opcode);
	void opcodeBNNN(WORD opcode);
	void opcodeCXNN(WORD opcode);
	void opcodeDXYN(WORD opcode);
	void opcodeEX9E(WORD opcode);
	void opcodeEXA1(WORD opcode);
	void opcodeFX07(WORD opcode);
	void opcodeFX0A(WORD opcode);
	void opcodeFX15(WORD opcode);
	void opcodeFX18(WORD opcode);
	void opcodeFX1E(WORD opcode);
	void opcodeFX29(WORD opcode);
	void opcodeFX33(WORD opcode);
	void opcodeFX55(WORD opcode);
	void opcodeFX65(WORD opcode);

};

#endif
