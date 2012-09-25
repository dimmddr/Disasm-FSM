//Пока что не очень значащий файл. Его наличие на данный момент под вопросом.
#pragma once

#define PREFIX_LOCK 0xf0
#define PREFIX_ADDRESS_SIZE 0x67

typedef struct {
	UINT8 opcode;
	
	UINT8 prefix;
	
	//для того чтобы можно было отличить случаи, когда значение этих байт = 0x0
	//от случая, когда этих байт нет в инструкции, вводим bool переменные
	UINT state;
	
	UINT displacement;
	UINT immediate;
	//флаг корректности инструкции. Надо еще обдумать его наличие, но пока пусть будет
	BOOL valid;
} INSTRUCTION, *PINSTRUCTION;