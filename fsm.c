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
UINT jumpTable[COND][BYTE];
char *prefixArray;//[PREFIXCOUNT];
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
	
	_asm{
	mov prefixArray[0], 0f0h
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
	//prefixArray[11] = "\0";
	//qwer db 0fh, 2fh, 3fh, 66h, 67h, 2eh, 3eh, 36h, 26h, 64h, 65h		
}

PVOID getPrefix(INSTRUCTION *instr) {	
	//getch();
	//printf("pre\n");
	_asm{
		mov esi, g_va
		cld
	start:	lodsb
		mov ecx, 11 ;количество префиксов
		mov edi, [prefixArray]
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
			fscanf(in, "%u", &jumpTable[i][ii]);
			}
	fclose(in);
}

void getInstruction(INSTRUCTION *instr) {
	int state = 0;
	int next = -1;
	int i = 0;
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