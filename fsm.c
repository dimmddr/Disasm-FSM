#include <windows.h>
#include <intrin.h>
#include <stdio.h>
#include "fsm.h"
#pragma intrinsic(__rdtsc)
typedef unsigned __int64 ticks;
#define getticks __rdtsc

#include <imagehlp.h>

#pragma comment(lib, "imagehlp")

#define PRINT_ERROR(f, file, line) \
  printf("%s: error %u at %s:%d\n", f, GetLastError(), file, line);
  
#define CURRENT_BYTE (*((PUINT8) g_va))  

#define COUNT 100
#define COND 29188
#define BYTE 256
#define STARTLINE 16
#define PREFIXCOUNT 11

//global variables
PVOID g_va;
UINT conditionTable[COND][BYTE];
UINT8 prefixArray[PREFIXCOUNT];
char *qwer;

UINT8 getByte() {
	UINT8 res  = CURRENT_BYTE;
	//все регистры при выходе из функции сбросятся в состояние, в котором они были до начала функции
	//как убрать два mov с глобальной переменной - я пока не знаю
	//если их убрать - скорость должна заметно повысится
	/*
	_asm{
		mov esi, g_va
		lodsb
		mov res, al
		mov g_va, esi
	}
	*/
	//так работает быстрее, чем с ассемблерной вставкой выше
	++((PUINT8) g_va);
	return res;
}

void prefixArrayInit() {
	prefixArray[0] = 0xf0;
	prefixArray[1] = 0x2f;
	prefixArray[2] = 0x3f;
	prefixArray[3] = 0x66;
	prefixArray[4] = 0x67;
	prefixArray[5] = 0x2e;
	prefixArray[6] = 0x3e;
	prefixArray[7] = 0x36;
	prefixArray[8] = 0x26;
	prefixArray[9] = 0x64;
	prefixArray[10] = 0x65;
}

PVOID getPrefix(INSTRUCTION *instr) {	
	_asm{
		mov esi, g_va
		cld
	start:	lodsb
		mov ecx, 11 ;количество префиксов
		lea edi, prefixArray
		repne scasb
		jnz q
		;set bit
		jmp start
	q:	mov g_va, esi
	}
}

void initializeFSM(PVOID va) {
	g_va = va;
}

void initializeTable() {
	UINT i, ii, state ;
	int f;
	FILE *in, *fopen();
	in = fopen("result_table.txt", "r");
	for(i = 0; i < COND; ++i)
		for(ii = 0; ii < BYTE; ++ii){
			fscanf(in, "%u", &conditionTable[i][ii]);
			}
	fclose(in);
}

void getInstruction(INSTRUCTION *instr) {
	int state = 0;
	int next = -1;
	UINT8 b;
	
	//getch();
	getPrefix(instr);
	//printf("instr\n");
	/*
	while (next != 0) {
		if(0 <= next) {
			state = next;
			}
		b = getByte();
		next = conditionTable[state][b];
	}
	*/
	
	for(;0 != next;) {
		if(0 <= next) 
			state = next;
		b = getByte();
		next = conditionTable[state][b];
	} 
	instr->state = state;
}

void main(int argc, PSTR argv[])
{
	UINT resInstr[COUNT];
	UINT8 state = 0;
	UINT8 b;
	LOADED_IMAGE image;
	PSTR imageFilename;
	int i;
	int ii;
	PVOID va;
	unsigned __int64 tickCount;
	INSTRUCTION instr;
	
	//imageFilename = argv[1];
	imageFilename = "test_long_instruction.exe";
	
	if (!MapAndLoad(imageFilename, NULL, &image, FALSE, TRUE)) {
		PRINT_ERROR("MapAndLoad", __FILE__, __LINE__);
		return;
	}
	va = ImageRvaToVa(image.FileHeader, image.MappedAddress,
						  image.FileHeader->OptionalHeader.BaseOfCode, NULL);
	initializeTable();
	prefixArrayInit();
	
	for(ii = 0; ii < COUNT; ++ii) {
		initializeFSM(va);
		
		_asm{
			lfence
		}
		tickCount = getticks();
		for(i = 0; i < COUNT; ++i) {
			//getByte();
			getInstruction(&instr);
		}
		
		tickCount = (getticks() - tickCount);
		resInstr[ii] = tickCount;
	}
	for(ii = 0; ii < COUNT; ++ii) 
		printf("%d \n", resInstr[ii]/COUNT);
		
}