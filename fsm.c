#include <windows.h>
#include <intrin.h>
#include <stdio.h>
#pragma intrinsic(__rdtsc)
typedef unsigned __int64 ticks;
#define getticks __rdtsc

#include <imagehlp.h>

#pragma comment(lib, "imagehlp")


#define PRINT_ERROR(f, file, line) \
  printf("%s: error %u at %s:%d\n", f, GetLastError(), file, line);
  
#define CURRENT_BYTE (*((PUINT8) g_va))  

#define COUNT 100000
#define COND 87408
#define BYTE 256
#define STARTLINE 16
#define PREFIXCOUNT 11

//global variables
PVOID g_va;
UINT jumpTable[COND][BYTE];
UINT8 prefixArray[PREFIXCOUNT];

UINT8 getByte() {
	UINT8 res = CURRENT_BYTE;
	++((PUINT8) g_va);
	return res;
}

VOID prefixCountInit() {
	_asm{
		mov prefixArray[0], 0fh
		mov prefixArray[1], 0fh
		mov prefixArray[2], 0fh
		mov prefixArray[3], 0fh
		mov prefixArray[4], 0fh
		mov prefixArray[5], 0fh
		mov prefixArray[6], 0fh
		mov prefixArray[7], 0fh
		mov prefixArray[8], 0fh
		mov prefixArray[9], 0fh
		mov prefixArray[10], 0fh
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
UINT8 transition(UINT8 state, UINT8 b) {
	UINT8 res = jumpTable[state][b];
	return res;
}

INT getInstruction() {
	INT state = 0;
	INT next = -1;
	int i = 0;
	UINT8 b = getByte();
	
	while (next != 0) {
		if(0 <= next) {
			state = next;
			}
		b = getByte();
		next = jumpTable[state][b];
	}
	//printf("%u\n",i);
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
	printf("let's rock\n");
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
		printf("%d \n", resInstr[ii]/COUNT);
}