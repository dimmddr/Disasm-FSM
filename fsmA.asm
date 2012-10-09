	.686
	.MMX
	.XMM
	.model	flat,stdcall
	option	casemap:none
	
	BSIZE equ 15
	PrC equ 11 ;количество префиксов
	
	include user32.inc
	includelib user32.lib
	include kernel32.inc
	includelib kernel32.lib
	
	.data
	ifmt	db "%0lu", 0
	outp	db BSIZE dup(?)
	prefix 	dw PrC dup(?)
	state	db 10000 dup(?)
	
public disasm 
	.code
	disasm proc byteAddress, tableAddress, count
	;Пока что передаем в функцию три параметра: адрес первого байта, адрес таблицы состояний, количество итераций
	push ebp

	mov esi, byteAddress
	mov ebx, tableAddress
	mov ecx, count
	call prefixInit
	lea edi, prefix
	push ecx ;сохраняем количество итераций, для следующего цикла
		lfence
		rdtsc
		push eax
		push edx
			qwer:
				push ecx
					call getInstruction
				pop ecx
				push edi
					lea edi, state
					mov [edi + ecx], edx
				pop edi
			loop qwer
			
			
			rdtsc
			mov ebx, eax
			mov ecx, edx
		pop edx
		pop eax
		sub ebx, eax
		sub ecx, edx
		mov edx, ebx
	pop ecx;это если надо будет распечатывать результат
	invoke GetStdHandle, -11
	lea edi, state
	print:
		mov ebx, [edi + ecx]
		push ecx
			push eax
				invoke	wsprintf, addr outp, addr ifmt, ebx
			pop eax
			invoke	WriteConsoleA, eax, addr outp, 10, 0, 0
		pop ecx
	loop print
	;invoke	wsprintf, addr outp, addr ifmt, ebx
	;invoke GetStdHandle, -11
	;invoke	WriteConsoleA, eax, addr outp, 10, 0, 0	
	mov eax, ebx
	pop ebp
	ret
	disasm endp
	;адрес следующего байта хранится в регистре edi
;адрес таблицы переходов для опкодов - в esi
getInstruction proc
	push ebx
		call getPrefix
	pop ecx
	mov ebx, 0
	instructionStart:
		mov edx, ebx ;сохраняем текущее состояние
		shl ebx, 8 ;умножаем на ширину таблицы
		;shl ebx, 9 ;умножаем на ширину таблицы и еще на 2, потому что ячейки в таблице 2 байта
		;shl eax, 1 ; по аналогичной причине умножаем входной байт на 2
		add eax, ebx ;получаем смещение, по которому хранится следующее состояние
		mov ebx, 0
		mov bl, [ecx + eax] ;получаем следующее состояние
		test ebx, ebx ;сравниваем его с 0
		jz exit
		mov eax, 0
		lodsb
		jmp instructionStart
	exit:
	mov ebx, ecx
	;заканчиваем работу функции
	ret
getInstruction endp

getPrefix proc
		cld
		mov ebx, edi
	prefixStart:	
		mov edi, ebx
		mov eax, 0
		lodsb
		mov ecx, 11 ;количество префиксов
		;REPNE SCAS m16 Find AX, starting at ES:[(E)DI]
		;Compare AL with byte at ES:(E)DI or RDI then set status flags
		repe scasb
		;repne scasb
		;jnz q
		jz prefixStart
		;set bit
		;jmp prefixStart
	q:	ret
getPrefix endp

prefixInit proc
	push ecx
	mov ecx, 0
	mov [prefix + ecx],0f0h
	inc ecx
	mov [prefix + ecx],065h
	inc ecx
	mov [prefix + ecx],02fh
	inc ecx
	mov [prefix + ecx],03fh
	inc ecx
	mov [prefix + ecx],066h
	inc ecx
	mov [prefix + ecx],067h
	inc ecx
	mov [prefix + ecx],02eh
	inc ecx
	mov [prefix + ecx],03eh
	inc ecx
	mov [prefix + ecx],036h
	inc ecx
	mov [prefix + ecx],026h
	inc ecx
	mov [prefix + ecx],064h
	pop ecx
	ret
prefixInit endp
end
