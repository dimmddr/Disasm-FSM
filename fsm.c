#include <windows.h>
#include <intrin.h>
#pragma intrinsic(__rdtsc)
typedef unsigned __int64 ticks;
#define getticks __rdtsc

#include <imagehlp.h>

#pragma comment(lib, "imagehlp")


#define PRINT_ERROR(f, file, line) \
  printf("%s: error %u at %s:%d\n", f, GetLastError(), file, line);
  
#define CURRENT_BYTE (*((PUINT8) g_va))  

#define COUNT 10000
#define COND 65540
#define BYTE 256
#define STARTLINE 16

//global variables
PVOID g_va;
INT jumpTable[COND][BYTE];

UINT8 getByte() {
	UINT8 res = CURRENT_BYTE;
	++((PUINT8) g_va);
	return res;
}

void initializeFSM(PVOID va) {
	g_va = va;
}

void initializeTable() {
	INT i, ii, state;
	for(i = 0; i < COND; ++i)
		for(ii = 0; ii < BYTE; ++ii)
		/*
			if(((4 <= i) && (19 >= i)) && (0 == ii || 8 == ii || 32 == ii || 103 == ii || 240 == ii || 246 == ii)) 
				jumpTable[i][ii] = 100;
			else 
			*/
				jumpTable[i][ii] = 0;
	//Потом я сделаю лучше -_-'
	jumpTable[0][0] = 4;
	jumpTable[1][0] = 8;
	jumpTable[2][0] = 12;
	jumpTable[3][0] = 16;
	jumpTable[0][32] = 5;
	jumpTable[1][32] = 9;
	jumpTable[2][32] = 13;
	jumpTable[3][32] = 17;
	jumpTable[0][8] = 6;
	jumpTable[1][8] = 10;
	jumpTable[2][8] = 14;
	jumpTable[3][8] = 18;
	jumpTable[0][246] = 7;
	jumpTable[1][246] = 11;
	jumpTable[2][246] = 15;
	jumpTable[3][246] = 19;
	jumpTable[0][240] = 1;
	jumpTable[1][240] = 1;
	jumpTable[2][240] = 3;
	jumpTable[3][240] = 3;
	jumpTable[0][103] = 2;
	jumpTable[1][103] = 3;
	jumpTable[2][103] = 2;
	jumpTable[3][103] = 3;
	state = 20;
	for(i = 4; i < 20; ++i)
		for(ii = 0; ii < BYTE; ++ii) {
			jumpTable[i][ii] = state;
			++state;
		}
	for(i = 12; i < COND; ++i)
		for(ii = 0; ii < BYTE; ++ii)
			if((0x04 == ii % 0x10) || (0x0C == ii % 0x10)){
				jumpTable[i][ii] = state;
				++state;
			}		
}
UINT8 transition(UINT8 state, UINT8 b) {
	UINT8 res = jumpTable[state][b];
	return res;
}

INT getInstruction() {
	INT state = 0;
	INT next = -1;
	UINT8 b = getByte();
	
	while (next != 0) {
		if(0 <= next) {
			state = next;
			}
		b = getByte();
		next = jumpTable[state][b];
	}
	return state;
}

VOID main(INT argc, PSTR argv[])
{
	INT resInstr[COUNT];
	UINT8 state = 0;
	UINT8 b;
	LOADED_IMAGE image;
	PSTR imageFilename;
	INT i;
	INT ii;
	PVOID va;
	unsigned __int64 tickCount;
	
	imageFilename = argv[1];
	
	if (!MapAndLoad(imageFilename, NULL, &image, FALSE, TRUE)) {
		PRINT_ERROR("MapAndLoad", __FILE__, __LINE__);
		return;
	}
	va = ImageRvaToVa(image.FileHeader, image.MappedAddress,
						  image.FileHeader->OptionalHeader.BaseOfCode, NULL);
	initializeTable();
	
	for(ii = 0; ii < COUNT; ++ii) {
		initializeFSM(va);
		_asm{
			lfence
		}
		tickCount = getticks();
		for(i = 0; i < COUNT; ++i) {
			getInstruction();
		}
		
		tickCount = (getticks() - tickCount);
		resInstr[ii] = tickCount;
	}
	for(ii = 0; ii < COUNT; ++ii) 
		printf("%d \n\r", resInstr[ii]/50000);
	/*
	for(i = 0; i < COUNT; ++i) {
		if(0 < resInstr[i]) {
			printf("%d : %d \n", i, resInstr[i]);
			break;
			}
	}
	*/
}