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

#define COUNT 10000
#define COND 87408
#define BYTE 256
#define STARTLINE 16
#define PREFIXCOUNT 11

//global variables
PVOID g_va;
UINT jumpTable[COND][BYTE];
UINT8 prefixArray[PREFIXCOUNT];

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
	_asm{
		mov prefixArray[0], 0fh
		mov prefixArray[1], 2fh
		mov prefixArray[2], 3fh
		mov prefixArray[3], 66h
		mov prefixArray[4], 67h
		mov prefixArray[5], 2eh
		mov prefixArray[6], 3eh
		mov prefixArray[7], 36h
		mov prefixArray[8], 26h
		mov prefixArray[9], 64h
		mov prefixArray[10], 65h
	}
}

PVOID getPrefix(INSTRUCTION *instr) {
	
	_asm{
		mov esi, g_va
		lodsb
		mov ecx, 11 ;количество префиксов
		cld
		mov edi, [prefixArray[0]]
		repne scasb
		je q
		
	q:
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
			fscanf(in, "%u", &jumpTable[i][ii]);
			}
	fclose(in);
}

void getInstruction(INSTRUCTION *instr) {
	int state = 0;
	int next = -1;
	int i = 0;
	UINT8 b;
	getPrefix(instr);
	/*
	while (next != 0) {
		if(0 <= next) {
			state = next;
			}
		b = getByte();
		next = jumpTable[state][b];
	}
	*/
	for(;0 != next;) {
		if(0 <= next) 
			state = next;
		b = getByte();
		next = jumpTable[state][b];
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
	
	imageFilename = argv[1];
	
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
		
		//как оказалось, в данном случае lfence уменьшает стабильность, а не увеличивает. Впрочем не всегда
		//стабильность она может и уменьшает, но скорость увеличивает, впрочем только на 1 такт
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