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

INT _stdcall disasm(PVOID, UINT);

//global variables
PVOID g_va;
PVOID ct;
// UINT16 conditionTable[COND*BYTE];
// UINT8 prefixArray[PREFIXCOUNT];
// UINT16 prefixStateTable[PREFIXSTATE*BYTE];
// UINT8 modRMAndImmTable[COND*BYTE];

void initializeFSM() {
	LOADED_IMAGE image;
	PSTR imageFilename;
	//imageFilename = argv[1];
	//imageFilename = "test_short_instruction.exe";
	//imageFilename = "test_prefix.exe";
	//imageFilename = "prefix_4_opcode_1.exe";
	imageFilename = "prefix_2_opcode_1_modRM_SIB_imm.exe";
	//imageFilename = "opcode_1_modRM_SIB_imm.exe";
	
	if (!MapAndLoad(imageFilename, NULL, &image, FALSE, TRUE)) {
		PRINT_ERROR("MapAndLoad", __FILE__, __LINE__);
		return;
	}
	g_va = ImageRvaToVa(image.FileHeader, image.MappedAddress,
						  image.FileHeader->OptionalHeader.BaseOfCode, NULL);
}

void main(int argc, PSTR argv[])
{
	UINT resInstr[TRYCOUNT];
	unsigned __int64 tickCount;
	INSTRUCTION instr[COUNT];
	int i;
	for(i = 0; i < COUNT; ++i) {
		initializeFSM();
		tickCount = disasm(g_va, COUNT*10);	
		printf("Time: %d \n",tickCount/(COUNT*10));
	}
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
	void initializePrefixFSMTable() {
	UINT i,ii;
	for(i = 0; i < PREFIXSTATE*BYTE; ++i)
		prefixStateTable[i] = 0;
		
	prefixStateTable[0x66] = 256 *  1;
	prefixStateTable[0x67] = 256 *  15;
	prefixStateTable[0x2f] = 256 *  14;
	prefixStateTable[0x3f] = 256 *  14;
	prefixStateTable[0x2e] = 256 *  14;
	prefixStateTable[0x3e] = 256 *  14;
	prefixStateTable[0x26] = 256 *  14;
	prefixStateTable[0x36] = 256 *  14;
	prefixStateTable[0x64] = 256 *  14;
	prefixStateTable[0x65] = 256 *  14;
	prefixStateTable[0xf0] = 256 *  14;
	prefixStateTable[0xf2] = 256 *  5;
	prefixStateTable[0xf3] = 256 *  9;
	
	
	prefixStateTable[0x66 + (256 * 1) ] = 256 *  3;
	prefixStateTable[0x67 + (256 * 1) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x3f + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x2e + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x3e + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x26 + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x36 + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x64 + (256 * 1) ] = 256 *  16;
	prefixStateTable[0x65 + (256 * 1) ] = 256 *  16;
	prefixStateTable[0xf0 + (256 * 1) ] = 256 *  16;
	prefixStateTable[0xf2 + (256 * 1) ] = 256 *  6;
	prefixStateTable[0xf3 + (256 * 1) ] = 256 *  10;
	
	prefixStateTable[0x66 + (256 * 2) ] = 256 *  4;
	prefixStateTable[0x67 + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x3f + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x2e + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x3e + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x26 + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x36 + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x64 + (256 * 2) ] = 256 *  13;
	prefixStateTable[0x65 + (256 * 2) ] = 256 *  13;
	prefixStateTable[0xf0 + (256 * 2) ] = 256 *  13;
	prefixStateTable[0xf2 + (256 * 2) ] = 256 *  7;
	prefixStateTable[0xf3 + (256 * 2) ] = 256 *  11;
	
	prefixStateTable[0x66 + (256 * 3) ] = 256 *  3;
	prefixStateTable[0x67 + (256 * 3) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x3f + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x2e + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x3e + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x26 + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x36 + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x64 + (256 * 3) ] = 256 *  16;
	prefixStateTable[0x65 + (256 * 3) ] = 256 *  16;
	prefixStateTable[0xf0 + (256 * 3) ] = 256 *  16;
	prefixStateTable[0xf2 + (256 * 3) ] = 256 *  6;
	prefixStateTable[0xf3 + (256 * 3) ] = 256 *  10;
	
	prefixStateTable[0x66 + (256 * 4) ] = 256 *  4;
	prefixStateTable[0x67 + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x3f + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x2e + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x3e + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x26 + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x36 + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x64 + (256 * 4) ] = 256 *  13;
	prefixStateTable[0x65 + (256 * 4) ] = 256 *  13;
	prefixStateTable[0xf0 + (256 * 4) ] = 256 *  13;
	prefixStateTable[0xf2 + (256 * 4) ] = 256 *  7;
	prefixStateTable[0xf3 + (256 * 4) ] = 256 *  11;
	
	prefixStateTable[0x66 + (256 * 5) ] = 256 *  1;
	prefixStateTable[0x67 + (256 * 5) ] = 256 *  15;
	prefixStateTable[0x2f + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x3f + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x2e + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x3e + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x26 + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x36 + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x64 + (256 * 5) ] = 256 *  14;
	prefixStateTable[0x65 + (256 * 5) ] = 256 *  14;
	prefixStateTable[0xf0 + (256 * 5) ] = 256 *  14;
	prefixStateTable[0xf2 + (256 * 5) ] = 256 *  5;
	prefixStateTable[0xf3 + (256 * 5) ] = 256 *  9;
	
	prefixStateTable[0x66 + (256 * 6) ] = 256 *  3;
	prefixStateTable[0x67 + (256 * 6) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x3f + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x2e + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x3e + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x26 + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x36 + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x64 + (256 * 6) ] = 256 *  16;
	prefixStateTable[0x65 + (256 * 6) ] = 256 *  16;
	prefixStateTable[0xf0 + (256 * 6) ] = 256 *  16;
	prefixStateTable[0xf2 + (256 * 6) ] = 256 *  6;
	prefixStateTable[0xf3 + (256 * 6) ] = 256 *  10;
	
	prefixStateTable[0x66 + (256 * 7) ] = 256 *  4;
	prefixStateTable[0x67 + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x3f + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x2e + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x3e + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x26 + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x36 + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x64 + (256 * 7) ] = 256 *  13;
	prefixStateTable[0x65 + (256 * 7) ] = 256 *  13;
	prefixStateTable[0xf0 + (256 * 7) ] = 256 *  13;
	prefixStateTable[0xf2 + (256 * 7) ] = 256 *  7;
	prefixStateTable[0xf3 + (256 * 7) ] = 256 *  11;
	
	prefixStateTable[0x66 + (256 * 8) ] = 256 *  2;
	prefixStateTable[0x67 + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x2f + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x3f + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x2e + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x3e + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x26 + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x36 + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x64 + (256 * 8) ] = 256 *  15;
	prefixStateTable[0x65 + (256 * 8) ] = 256 *  15;
	prefixStateTable[0xf0 + (256 * 8) ] = 256 *  15;
	prefixStateTable[0xf2 + (256 * 8) ] = 256 *  8;
	prefixStateTable[0xf3 + (256 * 8) ] = 256 *  12;
	
	prefixStateTable[0x66 + (256 * 9) ] = 256 *  3;
	prefixStateTable[0x67 + (256 * 9) ] = 256 *  15;
	prefixStateTable[0x2f + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x3f + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x2e + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x3e + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x26 + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x36 + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x64 + (256 * 9) ] = 256 *  14;
	prefixStateTable[0x65 + (256 * 9) ] = 256 *  14;
	prefixStateTable[0xf0 + (256 * 9) ] = 256 *  14;
	prefixStateTable[0xf2 + (256 * 9) ] = 256 *  5;
	prefixStateTable[0xf3 + (256 * 9) ] = 256 *  9;
	
	prefixStateTable[0x66 + (256 * 10) ] = 256 *  3;
	prefixStateTable[0x67 + (256 * 10) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x3f + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x2e + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x3e + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x26 + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x36 + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x64 + (256 * 10) ] = 256 *  16;
	prefixStateTable[0x65 + (256 * 10) ] = 256 *  16;
	prefixStateTable[0xf0 + (256 * 10) ] = 256 *  16;
	prefixStateTable[0xf2 + (256 * 10) ] = 256 *  6;
	prefixStateTable[0xf3 + (256 * 10) ] = 256 *  10;
	
	prefixStateTable[0x66 + (256 * 11) ] = 256 *  4;
	prefixStateTable[0x67 + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x3f + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x2e + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x3e + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x26 + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x36 + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x64 + (256 * 11) ] = 256 *  13;
	prefixStateTable[0x65 + (256 * 11) ] = 256 *  13;
	prefixStateTable[0xf0 + (256 * 11) ] = 256 *  13;
	prefixStateTable[0xf2 + (256 * 11) ] = 256 *  7;
	prefixStateTable[0xf3 + (256 * 11) ] = 256 *  11;
	
	prefixStateTable[0x66 + (256 * 12) ] = 256 *  2;
	prefixStateTable[0x67 + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x2f + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x3f + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x2e + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x3e + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x26 + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x36 + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x64 + (256 * 12) ] = 256 *  15;
	prefixStateTable[0x65 + (256 * 12) ] = 256 *  15;
	prefixStateTable[0xf0 + (256 * 12) ] = 256 *  15;
	prefixStateTable[0xf2 + (256 * 12) ] = 256 *  8;
	prefixStateTable[0xf3 + (256 * 12) ] = 256 *  12;
	
	prefixStateTable[0x66 + (256 * 13) ] = 256 *  4;
	prefixStateTable[0x67 + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x3f + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x2e + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x3e + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x26 + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x36 + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x64 + (256 * 13) ] = 256 *  13;
	prefixStateTable[0x65 + (256 * 13) ] = 256 *  13;
	prefixStateTable[0xf0 + (256 * 13) ] = 256 *  13;
	prefixStateTable[0xf2 + (256 * 13) ] = 256 *  7;
	prefixStateTable[0xf3 + (256 * 13) ] = 256 *  11;
	
	prefixStateTable[0x66 + (256 * 14) ] = 256 *  1;
	prefixStateTable[0x67 + (256 * 14) ] = 256 *  15;
	prefixStateTable[0x2f + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x3f + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x2e + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x3e + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x26 + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x36 + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x64 + (256 * 14) ] = 256 *  14;
	prefixStateTable[0x65 + (256 * 14) ] = 256 *  14;
	prefixStateTable[0xf0 + (256 * 14) ] = 256 *  14;
	prefixStateTable[0xf2 + (256 * 14) ] = 256 *  5;
	prefixStateTable[0xf3 + (256 * 14) ] = 256 *  9;
	
	prefixStateTable[0x66 + (256 * 15) ] = 256 *  2;
	prefixStateTable[0x67 + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x2f + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x3f + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x2e + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x3e + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x26 + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x36 + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x64 + (256 * 15) ] = 256 *  15;
	prefixStateTable[0x65 + (256 * 15) ] = 256 *  15;
	prefixStateTable[0xf0 + (256 * 15) ] = 256 *  15;
	prefixStateTable[0xf2 + (256 * 15) ] = 256 *  8;
	prefixStateTable[0xf3 + (256 * 15) ] = 256 *  12;
	
	prefixStateTable[0x66 + (256 * 16) ] = 256 *  3;
	prefixStateTable[0x67 + (256 * 16) ] = 256 *  13;
	prefixStateTable[0x2f + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x3f + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x2e + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x3e + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x26 + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x36 + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x64 + (256 * 16) ] = 256 *  16;
	prefixStateTable[0x65 + (256 * 16) ] = 256 *  16;
	prefixStateTable[0xf0 + (256 * 16) ] = 256 *  16;
	prefixStateTable[0xf2 + (256 * 16) ] = 256 *  6;
	prefixStateTable[0xf3 + (256 * 16) ] = 256 *  10;
}

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