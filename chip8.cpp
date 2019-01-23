#include "chip8.h"

BYTE chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip8::Chip8()
{   
	memset(key, 0, sizeof key);
	memset(V, 0, sizeof V);
	memset(gfx, 0, sizeof gfx);
	I = 0;
	pc = 0x200;
	sp = 0;
	sound_timer = 0;
	delay_timer = 0;
	srand(time(NULL)); //init random function
	for (int i = 0; i < 80; ++i) {
		memory[i] = chip8_fontset[i];
	}

}

WORD Chip8::get_opcode() {
	WORD ret;
	ret = memory[pc];
	ret = ret << 8;
	ret |= memory[pc + 1];
	
	pc += 2;
	
	return ret;
}

bool Chip8::load_program(const char * gameFile) {
	FILE *in;

	in = fopen(gameFile, "rb");
	if (in == nullptr) {
		return false;
	}

	fread(&memory[0x200], 1, 0xfff, in);
	fclose(in);
    
	return true;
}

void Chip8::emulate_cycle()
{
    WORD opcode = get_opcode();

    WORD first = opcode & 0xF000;
    WORD second = opcode & 0x0F00; 
    WORD third = opcode & 0x00F0;
    WORD fourth = opcode & 0x000F; 
    WORD two_last = opcode & 0x00FF;

    switch(first)
    {    
    case 0x0000:
        switch(fourth)
        {
            case 0x0000:
       		    opcodeOOEO(opcode);
            	break;
            case 0x000E:          
    		    opcode00EE(opcode);
            	break;          
    	}
    	break;
    case 0x1000:
        opcode1NNN(opcode);
    	break;
    case 0x2000:
    	opcode2NNN(opcode);
	break;
    case 0x3000:
    	opcode3XNN(opcode);
    	break;
    case 0x4000:
       	opcode4XNN(opcode);
    	break;
    case 0x5000:
    	opcode5XYO(opcode);
	    break;
    case 0x6000:
    	opcode6XNN(opcode);
	    break;
    case 0x7000:
        opcode7XNN(opcode);
	    break;
    case 0x8000:
    	switch(fourth)
        {
            case 0x0000: 
                opcode8XY0(opcode);
		        break;
            case 0x0001:          
            	opcode8XY1(opcode);
		        break;
           case 0x0002:          
            	 opcode8XY2(opcode);
		        break;
            case 0x0003:
            	opcode8XY3(opcode);
		        break;
            case 0x0004:       
                opcode8XY4(opcode);
		        break;
            case 0x0005:
            	opcode8XY5(opcode);
		        break;
            case 0x0006:
            	opcode8XY6(opcode);
		        break;
	        case 0x0007:
            	opcode8XY7(opcode);
		        break;
            case 0x000E:
            	opcode8XYE(opcode);
		        break;                     
    	}
    	break;
    case 0x9000:
    	opcode9XY0(opcode);
	    break;
    case 0xA000:
        opcodeANNN(opcode);
	    break;
    case 0xB000:
        opcodeBNNN(opcode);
	    break;
    case 0xC000:
        opcodeCXNN(opcode);
	    break;
    case 0xD000:
       	opcodeDXYN(opcode);
        break;
    case 0xE000:
        switch(third) 
        {
		    case 0x0090:
			    opcodeEX9E(opcode);
			    break;
		    case 0x00A0:
			    opcodeEXA1(opcode);
			    break;
	    }
        break;
    case 0xF000:
        switch(two_last) 
        {
		    case 0x0007:
			     opcodeFX07(opcode);
			    break;
		    case 0x000A:
			    opcodeFX0A(opcode);
			    break;
            case 0x0015:
                opcodeFX15(opcode);
                break;
            case 0x0018:
                opcodeFX18(opcode);
                break;
            case 0x001E:
                opcodeFX1E(opcode);
                break;
            case 0x0029:
                opcodeFX29(opcode);
                break;
            case 0x0033:
                opcodeFX33(opcode);
                break;
            case 0x0055:
                opcodeFX55(opcode);
                break;
            case 0x0065:
                opcodeFX65(opcode);
                break;
        }
        break;
    }  

    // Update timers
    if(delay_timer > 0) {
    	--delay_timer;
    }
    if (sound_timer > 0) {
        if(sound_timer == 1) {
            printf("\b");
        }
        --sound_timer;
    }
        

}

void Chip8::opcode0NNN(WORD opcode) {
	; //Not implemented    
}

void Chip8::opcodeOOEO(WORD opcode){
	memset(gfx, 0, sizeof gfx);
	draw_flag = true;
}

//pop stack
void Chip8::opcode00EE(WORD opcode) {
	--sp;
	pc = stack[sp];
	pc += 2;
}

void Chip8::opcode1NNN(WORD opcode) {
	pc = (opcode & 0x0FFF);
}

//call subroutine
void Chip8::opcode2NNN(WORD opcode) {
	stack[sp] = pc - 2;
	++sp;
	pc = (opcode & 0x0FFF);
}

void Chip8::opcode3XNN(WORD opcode) {
	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
		pc += 2;
	}
}

void Chip8::opcode4XNN(WORD opcode) {
	if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
		pc += 2;
	}
}

void Chip8::opcode5XYO(WORD opcode) {
	if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
		pc += 2;
	}
}

void Chip8::opcode6XNN(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
}

void Chip8::opcode7XNN(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);     
}

void Chip8::opcode8XY0(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
}

void Chip8::opcode8XY1(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
}

void Chip8::opcode8XY2(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
}

void Chip8::opcode8XY3(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
}

void Chip8::opcode8XY4(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];

	if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
		V[0xF] = 1;	
	} else {
		V[0xF] = 0;
	}
                        
}

void Chip8::opcode8XY5(WORD opcode) {
	if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]){
		V[0xF] = 0;
	} else {
		V[0xF] = 1;
	}
	V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
     
}

void Chip8::opcode8XY6(WORD opcode) {
	V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x01);
	V[(opcode & 0x0F00) >> 8] >>= 1; 
}

void Chip8::opcode8XY7(WORD opcode) {
	if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
		V[0xF] = 0;
	} else {
		V[0xF] = 1;
	}
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];               
}

void Chip8::opcode8XYE(WORD opcode) {
	V[0xF] = (V[(opcode & 0x0F00) >> 8]) >> 7;
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] << 1;
}

void Chip8::opcode9XY0(WORD opcode) {
	if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
		pc += 2;
	}
}

void Chip8::opcodeANNN(WORD opcode) {
	I = opcode & 0x0FFF;
}

void Chip8::opcodeBNNN(WORD opcode) {
	pc = V[0] + (opcode & 0x0FFF);
}

void Chip8::opcodeCXNN(WORD opcode) {
	BYTE random = rand() % (0xFF + 1);
	V[(opcode & 0x0F00) >> 8] = random & (opcode & 0x00FF); 
}

void Chip8::opcodeDXYN(WORD opcode) {
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	V[0xF] = 0;
    
	for (int yline = 0; yline < height; ++yline){
		pixel = memory[I + yline];
		for(int xline = 0; xline < 8; ++xline) {
            		if((pixel & (0x80 >> xline)) != 0) {
				if(gfx[(x + xline + ((y + yline) * 64))] == 1) {
					V[0xF] = 1;
				}
				gfx[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}
	
	draw_flag = true;     

}

void Chip8::opcodeEX9E(WORD opcode) {
	if (key[V[(opcode & 0x0F00) >> 8]] != 0) {
		pc += 2;
	}
}

void Chip8::opcodeEXA1(WORD opcode) {
	if (key[V[(opcode & 0x0F00) >> 8]] == 0) {
		pc += 2;
	}
}

void Chip8::opcodeFX07(WORD opcode) {
	V[(opcode & 0x0F00) >> 8] = delay_timer; 
}

void Chip8::opcodeFX0A(WORD opcode) {
    bool key_pressed = false;

    for(int i = 0; i < 16; ++i){
        if(key[i] != 0) {
            V[(opcode & 0x0F00) >> 8] = i;
            key_pressed = true;
        }
    }

    if(!key_pressed) {
        pc -= 2;
    }
}

void Chip8::opcodeFX15(WORD opcode) {
	delay_timer = V[(opcode & 0x0F00) >> 8];
}

void Chip8::opcodeFX18(WORD opcode) {
	sound_timer = V[(opcode & 0x0F00) >> 8];
}

void Chip8::opcodeFX1E(WORD opcode) {
	if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF) {
		V[0xF] = 1;
	} else {
		V[0xF] = 0;
	}
	I += V[(opcode & 0x0F00) >> 8];
}

void Chip8::opcodeFX29(WORD opcode) {	
	I = V[(opcode & 0x0F00) >> 8] * 0x5;
}

void Chip8::opcodeFX33(WORD opcode) {
	int value = V[(opcode & 0x0F00) >> 0];

	int hundreds = value / 100;
	int tens = (value / 10) % 10;
	int units = value % 10;

	memory[I] = hundreds;
	memory[I+1] = tens;
	memory[I+2] = units;
}

void Chip8::opcodeFX55(WORD opcode) {
	for (int i = 0 ; i <= ((opcode & 0x0F00) >> 8); ++i) {
		memory[I+i] = V[i];
	}
	I += ((opcode & 0x0F00) >> 8) + 1;
}

void Chip8::opcodeFX65(WORD opcode) {
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i) {
	V[i] = memory[I+i];
	}
	I += ((opcode & 0x0F00) >> 8) + 1;

}

Chip8::~Chip8() {}
