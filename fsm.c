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

#define COUNT 1000
#define TRYCOUNT 1
#define COND 796
#define BYTE 256
#define STARTLINE 16
#define PREFIXCOUNT 13
#define PREFIXSTATE 17

INT _stdcall disasm(PVOID, PVOID, PVOID, UINT);

//global variables
PVOID g_va;
PVOID ct;
UINT16 conditionTable[COND*BYTE];
UINT8 prefixArray[PREFIXCOUNT];
UINT8 prefixStateTable[PREFIXSTATE*BYTE];

void initializeTable() {
	UINT i, ii, state ;
	int f;
	FILE *in, *fopen();
	in = fopen("state_table.txt", "r");
	for(i = 0; i < COND*BYTE; ++i){
		fscanf(in, "%u", &conditionTable[i]);
	}
	fclose(in);
}

void initializePrefixFSMTable() {
	UINT i,ii;
	for(i = 0; i < PREFIXSTATE*BYTE; ++i)
		prefixStateTable[i] = 0;
		
	prefixStateTable[0x66] = 1;
	prefixStateTable[0x67] = 15;
	prefixStateTable[0x2f] = 14;
	prefixStateTable[0x3f] = 14;
	prefixStateTable[0x2e] = 14;
	prefixStateTable[0x3e] = 14;
	prefixStateTable[0x26] = 14;
	prefixStateTable[0x36] = 14;
	prefixStateTable[0x64] = 14;
	prefixStateTable[0x65] = 14;
	prefixStateTable[0xf0] = 14;
	prefixStateTable[0xf2] = 5;
	prefixStateTable[0xf3] = 9;
	
	prefixStateTable[0x66 + (256 * 1 - 1)] = 3;
	prefixStateTable[0x67 + (256 * 1 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 1 - 1)] = 16;
	prefixStateTable[0x3f + (256 * 1 - 1)] = 16;
	prefixStateTable[0x2e + (256 * 1 - 1)] = 16;
	prefixStateTable[0x3e + (256 * 1 - 1)] = 16;
	prefixStateTable[0x26 + (256 * 1 - 1)] = 16;
	prefixStateTable[0x36 + (256 * 1 - 1)] = 16;
	prefixStateTable[0x64 + (256 * 1 - 1)] = 16;
	prefixStateTable[0x65 + (256 * 1 - 1)] = 16;
	prefixStateTable[0xf0 + (256 * 1 - 1)] = 16;
	prefixStateTable[0xf2 + (256 * 1 - 1)] = 6;
	prefixStateTable[0xf3 + (256 * 1 - 1)] = 10;
	
	prefixStateTable[0x66 + (256 * 2 - 1)] = 4;
	prefixStateTable[0x67 + (256 * 2 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 2 - 1)] = 13;
	prefixStateTable[0x3f + (256 * 2 - 1)] = 13;
	prefixStateTable[0x2e + (256 * 2 - 1)] = 13;
	prefixStateTable[0x3e + (256 * 2 - 1)] = 13;
	prefixStateTable[0x26 + (256 * 2 - 1)] = 13;
	prefixStateTable[0x36 + (256 * 2 - 1)] = 13;
	prefixStateTable[0x64 + (256 * 2 - 1)] = 13;
	prefixStateTable[0x65 + (256 * 2 - 1)] = 13;
	prefixStateTable[0xf0 + (256 * 2 - 1)] = 13;
	prefixStateTable[0xf2 + (256 * 2 - 1)] = 7;
	prefixStateTable[0xf3 + (256 * 2 - 1)] = 11;
	
	prefixStateTable[0x66 + (256 * 3 - 1)] = 3;
	prefixStateTable[0x67 + (256 * 3 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 3 - 1)] = 16;
	prefixStateTable[0x3f + (256 * 3 - 1)] = 16;
	prefixStateTable[0x2e + (256 * 3 - 1)] = 16;
	prefixStateTable[0x3e + (256 * 3 - 1)] = 16;
	prefixStateTable[0x26 + (256 * 3 - 1)] = 16;
	prefixStateTable[0x36 + (256 * 3 - 1)] = 16;
	prefixStateTable[0x64 + (256 * 3 - 1)] = 16;
	prefixStateTable[0x65 + (256 * 3 - 1)] = 16;
	prefixStateTable[0xf0 + (256 * 3 - 1)] = 16;
	prefixStateTable[0xf2 + (256 * 3 - 1)] = 6;
	prefixStateTable[0xf3 + (256 * 3 - 1)] = 10;
	
	prefixStateTable[0x66 + (256 * 4 - 1)] = 4;
	prefixStateTable[0x67 + (256 * 4 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 4 - 1)] = 13;
	prefixStateTable[0x3f + (256 * 4 - 1)] = 13;
	prefixStateTable[0x2e + (256 * 4 - 1)] = 13;
	prefixStateTable[0x3e + (256 * 4 - 1)] = 13;
	prefixStateTable[0x26 + (256 * 4 - 1)] = 13;
	prefixStateTable[0x36 + (256 * 4 - 1)] = 13;
	prefixStateTable[0x64 + (256 * 4 - 1)] = 13;
	prefixStateTable[0x65 + (256 * 4 - 1)] = 13;
	prefixStateTable[0xf0 + (256 * 4 - 1)] = 13;
	prefixStateTable[0xf2 + (256 * 4 - 1)] = 7;
	prefixStateTable[0xf3 + (256 * 4 - 1)] = 11;
	
	prefixStateTable[0x66 + (256 * 5 - 1)] = 1;
	prefixStateTable[0x67 + (256 * 5 - 1)] = 15;
	prefixStateTable[0x2f + (256 * 5 - 1)] = 14;
	prefixStateTable[0x3f + (256 * 5 - 1)] = 14;
	prefixStateTable[0x2e + (256 * 5 - 1)] = 14;
	prefixStateTable[0x3e + (256 * 5 - 1)] = 14;
	prefixStateTable[0x26 + (256 * 5 - 1)] = 14;
	prefixStateTable[0x36 + (256 * 5 - 1)] = 14;
	prefixStateTable[0x64 + (256 * 5 - 1)] = 14;
	prefixStateTable[0x65 + (256 * 5 - 1)] = 14;
	prefixStateTable[0xf0 + (256 * 5 - 1)] = 14;
	prefixStateTable[0xf2 + (256 * 5 - 1)] = 5;
	prefixStateTable[0xf3 + (256 * 5 - 1)] = 9;
	
	prefixStateTable[0x66 + (256 * 6 - 1)] = 3;
	prefixStateTable[0x67 + (256 * 6 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 6 - 1)] = 16;
	prefixStateTable[0x3f + (256 * 6 - 1)] = 16;
	prefixStateTable[0x2e + (256 * 6 - 1)] = 16;
	prefixStateTable[0x3e + (256 * 6 - 1)] = 16;
	prefixStateTable[0x26 + (256 * 6 - 1)] = 16;
	prefixStateTable[0x36 + (256 * 6 - 1)] = 16;
	prefixStateTable[0x64 + (256 * 6 - 1)] = 16;
	prefixStateTable[0x65 + (256 * 6 - 1)] = 16;
	prefixStateTable[0xf0 + (256 * 6 - 1)] = 16;
	prefixStateTable[0xf2 + (256 * 6 - 1)] = 6;
	prefixStateTable[0xf3 + (256 * 6 - 1)] = 10;
	
	prefixStateTable[0x66 + (256 * 7 - 1)] = 4;
	prefixStateTable[0x67 + (256 * 7 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 7 - 1)] = 13;
	prefixStateTable[0x3f + (256 * 7 - 1)] = 13;
	prefixStateTable[0x2e + (256 * 7 - 1)] = 13;
	prefixStateTable[0x3e + (256 * 7 - 1)] = 13;
	prefixStateTable[0x26 + (256 * 7 - 1)] = 13;
	prefixStateTable[0x36 + (256 * 7 - 1)] = 13;
	prefixStateTable[0x64 + (256 * 7 - 1)] = 13;
	prefixStateTable[0x65 + (256 * 7 - 1)] = 13;
	prefixStateTable[0xf0 + (256 * 7 - 1)] = 13;
	prefixStateTable[0xf2 + (256 * 7 - 1)] = 7;
	prefixStateTable[0xf3 + (256 * 7 - 1)] = 11;
	
	prefixStateTable[0x66 + (256 * 8 - 1)] = 2;
	prefixStateTable[0x67 + (256 * 8 - 1)] = 15;
	prefixStateTable[0x2f + (256 * 8 - 1)] = 15;
	prefixStateTable[0x3f + (256 * 8 - 1)] = 15;
	prefixStateTable[0x2e + (256 * 8 - 1)] = 15;
	prefixStateTable[0x3e + (256 * 8 - 1)] = 15;
	prefixStateTable[0x26 + (256 * 8 - 1)] = 15;
	prefixStateTable[0x36 + (256 * 8 - 1)] = 15;
	prefixStateTable[0x64 + (256 * 8 - 1)] = 15;
	prefixStateTable[0x65 + (256 * 8 - 1)] = 15;
	prefixStateTable[0xf0 + (256 * 8 - 1)] = 15;
	prefixStateTable[0xf2 + (256 * 8 - 1)] = 8;
	prefixStateTable[0xf3 + (256 * 8 - 1)] = 12;
	
	prefixStateTable[0x66 + (256 * 9 - 1)] = 3;
	prefixStateTable[0x67 + (256 * 9 - 1)] = 15;
	prefixStateTable[0x2f + (256 * 9 - 1)] = 14;
	prefixStateTable[0x3f + (256 * 9 - 1)] = 14;
	prefixStateTable[0x2e + (256 * 9 - 1)] = 14;
	prefixStateTable[0x3e + (256 * 9 - 1)] = 14;
	prefixStateTable[0x26 + (256 * 9 - 1)] = 14;
	prefixStateTable[0x36 + (256 * 9 - 1)] = 14;
	prefixStateTable[0x64 + (256 * 9 - 1)] = 14;
	prefixStateTable[0x65 + (256 * 9 - 1)] = 14;
	prefixStateTable[0xf0 + (256 * 9 - 1)] = 14;
	prefixStateTable[0xf2 + (256 * 9 - 1)] = 5;
	prefixStateTable[0xf3 + (256 * 9 - 1)] = 9;
	
	prefixStateTable[0x66 + (256 * 10 - 1)] = 3;
	prefixStateTable[0x67 + (256 * 10 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 10 - 1)] = 16;
	prefixStateTable[0x3f + (256 * 10 - 1)] = 16;
	prefixStateTable[0x2e + (256 * 10 - 1)] = 16;
	prefixStateTable[0x3e + (256 * 10 - 1)] = 16;
	prefixStateTable[0x26 + (256 * 10 - 1)] = 16;
	prefixStateTable[0x36 + (256 * 10 - 1)] = 16;
	prefixStateTable[0x64 + (256 * 10 - 1)] = 16;
	prefixStateTable[0x65 + (256 * 10 - 1)] = 16;
	prefixStateTable[0xf0 + (256 * 10 - 1)] = 16;
	prefixStateTable[0xf2 + (256 * 10 - 1)] = 6;
	prefixStateTable[0xf3 + (256 * 10 - 1)] = 10;
	
	prefixStateTable[0x66 + (256 * 11 - 1)] = 4;
	prefixStateTable[0x67 + (256 * 11 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 11 - 1)] = 13;
	prefixStateTable[0x3f + (256 * 11 - 1)] = 13;
	prefixStateTable[0x2e + (256 * 11 - 1)] = 13;
	prefixStateTable[0x3e + (256 * 11 - 1)] = 13;
	prefixStateTable[0x26 + (256 * 11 - 1)] = 13;
	prefixStateTable[0x36 + (256 * 11 - 1)] = 13;
	prefixStateTable[0x64 + (256 * 11 - 1)] = 13;
	prefixStateTable[0x65 + (256 * 11 - 1)] = 13;
	prefixStateTable[0xf0 + (256 * 11 - 1)] = 13;
	prefixStateTable[0xf2 + (256 * 11 - 1)] = 7;
	prefixStateTable[0xf3 + (256 * 11 - 1)] = 11;
	
	prefixStateTable[0x66 + (256 * 12 - 1)] = 2;
	prefixStateTable[0x67 + (256 * 12 - 1)] = 15;
	prefixStateTable[0x2f + (256 * 12 - 1)] = 15;
	prefixStateTable[0x3f + (256 * 12 - 1)] = 15;
	prefixStateTable[0x2e + (256 * 12 - 1)] = 15;
	prefixStateTable[0x3e + (256 * 12 - 1)] = 15;
	prefixStateTable[0x26 + (256 * 12 - 1)] = 15;
	prefixStateTable[0x36 + (256 * 12 - 1)] = 15;
	prefixStateTable[0x64 + (256 * 12 - 1)] = 15;
	prefixStateTable[0x65 + (256 * 12 - 1)] = 15;
	prefixStateTable[0xf0 + (256 * 12 - 1)] = 15;
	prefixStateTable[0xf2 + (256 * 12 - 1)] = 8;
	prefixStateTable[0xf3 + (256 * 12 - 1)] = 12;
	
	prefixStateTable[0x66 + (256 * 13 - 1)] = 4;
	prefixStateTable[0x67 + (256 * 13 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 13 - 1)] = 13;
	prefixStateTable[0x3f + (256 * 13 - 1)] = 13;
	prefixStateTable[0x2e + (256 * 13 - 1)] = 13;
	prefixStateTable[0x3e + (256 * 13 - 1)] = 13;
	prefixStateTable[0x26 + (256 * 13 - 1)] = 13;
	prefixStateTable[0x36 + (256 * 13 - 1)] = 13;
	prefixStateTable[0x64 + (256 * 13 - 1)] = 13;
	prefixStateTable[0x65 + (256 * 13 - 1)] = 13;
	prefixStateTable[0xf0 + (256 * 13 - 1)] = 13;
	prefixStateTable[0xf2 + (256 * 13 - 1)] = 7;
	prefixStateTable[0xf3 + (256 * 13 - 1)] = 11;
	
	prefixStateTable[0x66 + (256 * 14 - 1)] = 1;
	prefixStateTable[0x67 + (256 * 14 - 1)] = 15;
	prefixStateTable[0x2f + (256 * 14 - 1)] = 14;
	prefixStateTable[0x3f + (256 * 14 - 1)] = 14;
	prefixStateTable[0x2e + (256 * 14 - 1)] = 14;
	prefixStateTable[0x3e + (256 * 14 - 1)] = 14;
	prefixStateTable[0x26 + (256 * 14 - 1)] = 14;
	prefixStateTable[0x36 + (256 * 14 - 1)] = 14;
	prefixStateTable[0x64 + (256 * 14 - 1)] = 14;
	prefixStateTable[0x65 + (256 * 14 - 1)] = 14;
	prefixStateTable[0xf0 + (256 * 14 - 1)] = 14;
	prefixStateTable[0xf2 + (256 * 14 - 1)] = 5;
	prefixStateTable[0xf3 + (256 * 14 - 1)] = 9;
	
	prefixStateTable[0x66 + (256 * 15 - 1)] = 2;
	prefixStateTable[0x67 + (256 * 15 - 1)] = 15;
	prefixStateTable[0x2f + (256 * 15 - 1)] = 15;
	prefixStateTable[0x3f + (256 * 15 - 1)] = 15;
	prefixStateTable[0x2e + (256 * 15 - 1)] = 15;
	prefixStateTable[0x3e + (256 * 15 - 1)] = 15;
	prefixStateTable[0x26 + (256 * 15 - 1)] = 15;
	prefixStateTable[0x36 + (256 * 15 - 1)] = 15;
	prefixStateTable[0x64 + (256 * 15 - 1)] = 15;
	prefixStateTable[0x65 + (256 * 15 - 1)] = 15;
	prefixStateTable[0xf0 + (256 * 15 - 1)] = 15;
	prefixStateTable[0xf2 + (256 * 15 - 1)] = 8;
	prefixStateTable[0xf3 + (256 * 15 - 1)] = 12;
	
	prefixStateTable[0x66 + (256 * 16 - 1)] = 3;
	prefixStateTable[0x67 + (256 * 16 - 1)] = 13;
	prefixStateTable[0x2f + (256 * 16 - 1)] = 16;
	prefixStateTable[0x3f + (256 * 16 - 1)] = 16;
	prefixStateTable[0x2e + (256 * 16 - 1)] = 16;
	prefixStateTable[0x3e + (256 * 16 - 1)] = 16;
	prefixStateTable[0x26 + (256 * 16 - 1)] = 16;
	prefixStateTable[0x36 + (256 * 16 - 1)] = 16;
	prefixStateTable[0x64 + (256 * 16 - 1)] = 16;
	prefixStateTable[0x65 + (256 * 16 - 1)] = 16;
	prefixStateTable[0xf0 + (256 * 16 - 1)] = 16;
	prefixStateTable[0xf2 + (256 * 16 - 1)] = 6;
	prefixStateTable[0xf3 + (256 * 16 - 1)] = 10;
}

void initializeFSM() {
	LOADED_IMAGE image;
	PSTR imageFilename;
	//imageFilename = argv[1];
	//imageFilename = "test_short_instruction.exe";
	imageFilename = "test_prefix.exe";
	
	if (!MapAndLoad(imageFilename, NULL, &image, FALSE, TRUE)) {
		PRINT_ERROR("MapAndLoad", __FILE__, __LINE__);
		return;
	}
	g_va = ImageRvaToVa(image.FileHeader, image.MappedAddress,
						  image.FileHeader->OptionalHeader.BaseOfCode, NULL);
	initializeTable();
	initializePrefixFSMTable();
}

void main(int argc, PSTR argv[])
{
	UINT resInstr[TRYCOUNT];
	unsigned __int64 tickCount;
	INSTRUCTION instr[COUNT];
	
	
	initializeFSM();
	_asm{
		lea eax, conditionTable
		mov ct, eax
		lfence
		}
	tickCount = disasm(g_va, ct, prefixStateTable, COUNT);	
	printf("Time: %d \n",tickCount/COUNT);
	/*
	for(ii = 0; ii < TRYCOUNT; ++ii) {
		initializeFSM(va);
		
		_asm{
			lfence
		}
		tickCount = getticks();
		for(i = 0; i < COUNT; ++i) {
			//getByte();
			//printf("%d\n", i);
			getInstruction(&instr[i]);
		}
		tickCount = (getticks() - tickCount);
		//printf("stop\n");
		resInstr[ii] = tickCount;
	}
	
	for(ii = 0; ii < COUNT; ++ii) {
		printf("%u \n", instr[ii].state);
	}
	
	for(ii = 0; ii < TRYCOUNT; ++ii) 
		printf("%d \n",resInstr[TRYCOUNT]/COUNT);
	
	*/
	/*
	void getInstruction(INSTRUCTION *instr) {
	UINT state = 0;
	int next = -1;
	UINT8 b;
	getPrefix(instr);
	for(;0 != next;) {
		if(0 < next) 
			state = (UINT) next;
		b = getByte();
		next = conditionTable[state][b];
	} 
	instr->state = state;
}
	*/
	/*	
void prefixArrayInit() {
	prefixArray[0] = 0xf0;
	prefixArray[1] = 0x65;
	prefixArray[2] = 0x2f;
	prefixArray[3] = 0x3f;
	prefixArray[4] = 0x66;
	prefixArray[5] = 0x67;
	prefixArray[6] = 0x2e;
	prefixArray[7] = 0x3e;
	prefixArray[8] = 0x36;
	prefixArray[9] = 0x26;
	prefixArray[10] = 0x64;
}
	*/
	/*
	PVOID getPrefix(INSTRUCTION *instr) {
	_asm{
		mov esi, g_va
		cld
	start:	
		lea edi, prefixArray
		mov ecx, 11 ;количество префиксов
		;REPNE SCAS m16 Find AX, starting at ES:[(E)DI]
		;Compare AL with byte at ES:(E)DI or RDI then set status flags
		repe scasb
		;repne scasb
		;jnz q
		jz start
		;set bit
		;jmp start
	q:	mov g_va, esi
	}
	}
	*/
	/*	
UINT8 getByte() {
	UINT8 res  = CURRENT_BYTE;
	++((PUINT8) g_va);
	return res;
}
	*/
}