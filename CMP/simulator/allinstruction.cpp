#include <iostream>
#include "allinstruction.h"
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

void instruction(char buffer[])
{
	unsigned int ins[32] = {0};
	unsigned int temp, a, index=0;
	for(int i=0; i<4; i++){
		temp = buffer[i];
		for(int j=7; j>=0; j--){
			a = temp % 2;
			temp = temp/2;
			ins[index+j] = a;
		}
		index += 8;
	}
	int opcode = 0, b = 32;
	for(int i=0; i<6; i++){
		opcode += b*ins[i];
		b /= 2;
	}
	
	unsigned int funct = getfunct(ins);
	unsigned int rd = getrd(ins), rs = getrs(ins), rt = getrt(ins), shamt = getshamt(ins), imu = getimu(ins), addr = getaddr(ins);

	int im = getim(ins);
	long long int mul;
	unsigned long long int mulu;
	int sum;
	int forlo;

	switch(opcode){
		case 0:
			switch(funct){
				case 32: //add
					memoryAccess();
					reg[rd] = reg[rs] + reg[rt];
					if(rd == 0) reg[rd] = 0;
					break;
				case 33: //addu
					memoryAccess();
					reg[rd] = reg[rs] + reg[rt];
					if(rd == 0) reg[rd] = 0;
					break;
				case 34: //sub
					memoryAccess();
					reg[rd] = reg[rs]+(-reg[rt]);
					if(rd == 0) reg[rd] = 0;
					break;
				case 36: //and
					memoryAccess();
					reg[rd] = (reg[rs] & reg[rt]);
					if(rd == 0) reg[rd] = 0;
					break;
				case 37: //or
					memoryAccess();
					reg[rd] = (reg[rs] | reg[rt]);
					if(rd == 0) reg[rd] = 0;
					break;					
				case 38: //xor
					memoryAccess();
					reg[rd] = (reg[rs] ^ reg[rt]);
					if(rd == 0) reg[rd] = 0;
					break;
				case 39: //nor
					memoryAccess();
					reg[rd] = ~(reg[rs] | reg[rt]);
					if(rd == 0) reg[rd] = 0;
					break;
				case 40: //nand
					memoryAccess();
					reg[rd] = ~(reg[rs] & reg[rt]);
					if(rd == 0) reg[rd] = 0;
					break;
				case 42: //slt
					memoryAccess();
					reg[rd] = reg[rs] < reg[rt];
					if(rd == 0) reg[rd] = 0;
					break;
				case 0: //sll
					memoryAccess();
					reg[rd] = reg[rt] << shamt;
					if(rd == 0) reg[rd] = 0;
					break;
				case 2: //srl
				{
					memoryAccess();
					reg[rd] = (unsigned int)reg[rt] >> shamt;	
					if(rd == 0) reg[rd] = 0;
					break;
				}
				case 3: //sra
					memoryAccess();
					reg[rd] = reg[rt] >> shamt;
					if(rd == 0) reg[rd] = 0;
					break;
				case 8: //jr
					memoryAccess();
					jump = 1;
					reg[34] = reg[rs];
					break;
				case 24: //mult problem
				{
					memoryAccess();
					if(error3 == 1) /*printf("hi lo\n")*/;
					else error3 = 1;
					long long int ma = reg[rs], mb = reg[rt];
					mul = ma * mb;
					forlo = mul;
					reg[32] = mul>>32;
					reg[33] = forlo;
					break;
				}
				case 25: //multu
				{
					memoryAccess();
					if(error3 == 1) /*printf("hi lo\n")*/;
					else error3 = 1;
					unsigned long long int a = (unsigned int)reg[rs], b = (unsigned int)reg[rt];
					mulu = a * b;
					forlo = mulu;
					reg[32] = mulu >> 32;
					reg[33] = forlo;
					break;
				}
				case 16: //mfhi
					memoryAccess();
					if(error3 == 1) error3 = 0;
					reg[rd] = reg[32];
					if(rd == 0) reg[rd] = 0;
					break;
				case 18: //mflo
					memoryAccess();
					if(error3 == 1) error3 = 0;
					reg[rd] = reg[33];
					if(rd == 0) reg[rd] = 0;
					break;
			}
			break;
		case 8: //addi
			memoryAccess();
			reg[rt] = reg[rs] + im;
			if(rt == 0) reg[rt] = 0;
			break;
		case 9: //addiu
			memoryAccess();
			reg[rt] = reg[rs] + im;
			if(rt == 0) reg[rt] = 0;
			break;
		case 35: //lw
		{
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			int temp1;
			temp1 = (unsigned char)data[reg[rs]+im];
			for(int r=1; r<4; r++){
				temp1 = temp1 << 8;
				temp1 = temp1 | (unsigned char)data[reg[rs]+im+r];
			}
			reg[rt] = temp1;
			if(rt == 0) reg[rt] = 0;
			break;
		}
		case 33: //lh
		{
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			int temp1;
			temp1 = data[reg[rs]+im];
			for(int r=1; r<2; r++){
				temp1 = temp1 << 8;
				temp1 = temp1 | ((unsigned char)data[reg[rs]+im+r]);
			}
			reg[rt] = temp1;
			if(rt == 0) reg[rt] = 0;
			break;
		}
		case 37: //lhu
		{
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			unsigned int temp1;
			temp1 = (unsigned char)data[reg[rs]+im];
			temp1 = temp1 << 8;
			temp1 = temp1 | (unsigned char)data[reg[rs]+im+1];
			reg[rt] = temp1;
			if(rt == 0) reg[rt] = 0;
			break;
		}
		case 32: //lb
		{
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			reg[rt] = data[reg[rs]+im];
			if(rt == 0) reg[rt] = 0;
			break;
		}
		case 36: //lbu
		{
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			unsigned int temp = (unsigned char)data[reg[rs]+im];
			reg[rt] = temp;
			if(rt == 0) reg[rt] = 0;
			break;
		}
		case 43: //sw
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			data[reg[rs]+im+3] = reg[rt];
			for(int r=2; r>=0; r--){
				data[reg[rs]+im+r] = reg[rt] >> 8*(3-r);
			}
			break;
		case 41: //sh
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			data[reg[rs]+im+1] = reg[rt];
			data[reg[rs]+im] = reg[rt] >> 8;
			break;
		case 40: //sb
			//printf("cycle%d: %d %d %d", cycle, reg[rs], im, reg[rs]+im);
			memoryAccess();
			sum = reg[rs]+im;
			Dva = (unsigned int) sum;
			//printf(" %u\n", Dva);
			memoryDaccess();
			data[reg[rs]+im] = reg[rt];
			break;
		case 15: //lui
			memoryAccess();
			reg[rt] = im << 16;
			if(rt == 0) reg[rt] = 0;
			break;
		case 12: //andi
			memoryAccess();
			reg[rt] = (reg[rs] & imu);
			if(rt == 0) reg[rt] = 0;
			break;
		case 13: //ori
			memoryAccess();
			reg[rt] = (reg[rs] | imu);
			if(rt == 0) reg[rt] = 0;
			break;
		case 14: //nori
			memoryAccess();
			reg[rt] = ~(reg[rs] | imu);
			if(rt == 0) reg[rt] = 0;
			break;
		case 10: //slti
			memoryAccess();
			reg[rt] = (reg[rs] < im);
			if(rt == 0) reg[rt] = 0;
			break;
		case 4: //beq
			memoryAccess();
			if(reg[rs] == reg[rt]) reg[34] = reg[34]+4*im;
			break;
		case 5: //bne
			memoryAccess();
			if(reg[rs] != reg[rt]) reg[34] = reg[34]+4*im;
			break;
		case 7: //bgtz
			memoryAccess();
			if(reg[rs] > 0) reg[34] = reg[34]+4*im;
			break;
		case 2: //j
			memoryAccess();
			reg[34] = (((reg[34]+4) >> 28) << 28) | addr << 2;
			jump = 1;
			break;
		case 3: //jal
			memoryAccess();
			jump = 1;
			reg[31] = reg[34]+4;
			reg[34] = (((reg[34]+4) >> 28) << 28) | addr << 2;
			break;
		case 63: //halt
			memoryAccess();
			termi = 1;
			break;
		default: break;
	}
}
