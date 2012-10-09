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
#define TRYCOUNT 1
#define COND 37
#define BYTE 256
#define STARTLINE 16
#define PREFIXCOUNT 11

INT _stdcall disasm(PVOID, PVOID, UINT);

//global variables
PVOID g_va;
PVOID ct;
PVOID pa;
UINT8 conditionTable[COND*BYTE];
UINT8 prefixArray[PREFIXCOUNT];
char *qwer;

void initializeFSM(PVOID va) {
	g_va = va;
}

void initializeTable() {
	UINT i, ii, state ;
	int f;
	FILE *in, *fopen();
	in = fopen("result_table.txt", "r");
	for(i = 0; i < COND*BYTE; ++i){
			fscanf(in, "%u", &conditionTable[i]);
			}
	fclose(in);
}



void main(int argc, PSTR argv[])
{
	UINT resInstr[TRYCOUNT];
	LOADED_IMAGE image;
	PSTR imageFilename;
	int i;
	int ii;
	PVOID va;
	unsigned __int64 tickCount;
	INSTRUCTION instr[COUNT];
	
	//imageFilename = argv[1];
	imageFilename = "test_prefix.exe";
	
	if (!MapAndLoad(imageFilename, NULL, &image, FALSE, TRUE)) {
		PRINT_ERROR("MapAndLoad", __FILE__, __LINE__);
		return;
	}
	va = ImageRvaToVa(image.FileHeader, image.MappedAddress,
						  image.FileHeader->OptionalHeader.BaseOfCode, NULL);
	initializeFSM(va);
	initializeTable();
	_asm{
		lea eax, conditionTable
		mov ct, eax
		lfence
		}
	tickCount = disasm(g_va, ct, COUNT);	
	//printf("in c: %d \n",tickCount/COUNT);
	//printf("%x \n",conditionTable);
	//printf("%x \n",&conditionTable);
	//printf("%x \n",ct);
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