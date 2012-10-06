	.686
	.MMX
	.XMM
	.model	flat,stdcall
	option	casemap:none
	
	BSIZE equ 15
	
	include user32.inc
	includelib user32.lib
	include kernel32.inc
	includelib kernel32.lib
	
	.data
	ifmt	db "%0lu", 0
	outp	db BSIZE dup(?)
	
;disasm PROTO :dword, :dword, :dword
	
	.code
public disasm 
	disasm proc byteAddress, tableAddress, count
	;Пока что передаем в функцию три параметра: адрес первого байта, адрес таблицы состояний, количество итераций
	;сохраняем значения системных регистров
	push ebp
	push esi
	push edi
	
	mov ecx, count
	mov edi, byteAddress
	mov esi, tableAddress
	push ecx ;сохраняем количество итераций, для следующего цикла
	qwer:
		call getInstruction
		;вообще не лучший вариант - использовать стек. Может стоит потом массив сделать
		;push eax
	loop qwer
	pop ecx
	
	invoke GetStdHandle, -11
	
	;print:
	;	pop ebx
	;	invoke	wsprintf, addr outp, addr ifmt, ebx
	;	invoke	WriteConsoleA, eax, addr outp, 10, 0, 0
	;loop print
	
	
	;восстанавливаем значения системных регистров
	pop edi
	pop esi
	pop ebp
	
	ret
	disasm endp
	
;адрес следующего байта хранится в регистре edi
;адрес таблицы переходов для опкодов - в esi
getInstruction proc
	
	call getPrefix
	
	mov ebx, 0
	instructionStart:
		lodsb
		mov edx, ebx ;сохраняем текущее состояние
		shl ebx, 9 ;умножаем на ширину таблицы и еще на 2, потому что ячейки в таблице 2 байта
		shl eax, 1 ; по аналогичной причине умножаем входной байт на 2
		add eax, ebx ;получаем смещение, по которому хранится следующее состояние
		mov ebx, [esi + eax] ;получаем слдующее состояние
		or ebx, ebx ;сравниваем его с 0
		jnz instructionStart
		mov eax, edx;возвращаем последнее не нулевое состояние
	;заканчиваем работу функции
	;и, возможно, надо сбалансировать стек
	ret
getInstruction endp

getPrefix proc
		cld
	prefixStart:	
		mov ecx, 11 ;количество префиксов
		;REPNE SCAS m16 Find AX, starting at ES:[(E)DI]
		;Compare AL with byte at ES:(E)DI or RDI then set status flags
		repe scasb
		;repne scasb
		;jnz q
		jz prefixStart
		;set bit
		;jmp start
	q:	ret
getPrefix endp
	
end disasm