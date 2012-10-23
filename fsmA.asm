	.686
	.MMX
	.XMM
	.model	flat,stdcall
	option	casemap:none
	
	BSIZE equ 15
	PrC equ 13 ;prefix count
	
	include user32.inc
	includelib user32.lib
	include kernel32.inc
	includelib kernel32.lib
	
	.data
	ifmt	db "%0lu", 0
	outp	db BSIZE dup(?)
	prefix 	dw PrC dup(?)
	state	db 100000 dup(?)
	
public disasm 
	.code
	disasm proc byteAddress, tableAddress, count
	;3 parameters: first byte address, state table address, iteration count
	push ebp

	mov esi, byteAddress
	mov ebx, tableAddress
	mov ecx, count
	call prefixInit
	lea edi, prefix
	push ecx ;save count of iteration for next cycle usage
	;сохраняем количество итераций, для следующего цикла
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
					;mov [edi + ecx], edx
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
	pop ecx;use this if print result needed
	;это если надо будет распечатывать результат
	mov eax, ebx
	push eax
		call printInstruction
	pop eax
	pop ebp
	ret
disasm endp
printInstruction proc
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
	ret
printInstruction endp
;next byte address in edi
;state table address in esi
;адрес следующего байта хранится в регистре edi
;адрес таблицы переходов для опкодов - в esi
getInstruction proc
	push ebx
		call getPrefix
	pop ecx
	mov ebx, 0
	instructionStart:
		mov edx, ebx ;keep currnet state 
		;сохраняем текущее состояние
		;shl ebx, 8 ;multiply by width of the state table
		;умножаем на ширину таблицы
		shl ebx, 9 ;9 - if size of the cell in the state table will be 2 byte
		shl eax, 1 ; for some reasons multiply next byte by 2
		add eax, ebx ;we receive shift on which the following condition is stored
		;получаем смещение, по которому хранится следующее состояние
		mov ebx, 0
		mov bl, [ecx + eax] ;take the next state
		;получаем следующее состояние
		test ebx, ebx ;compare state and 0
		;сравниваем его с 0
		jz exit
		mov eax, 0
		lodsb
		jmp instructionStart
	exit:
	mov ebx, ecx
	;end of work. KO
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
		mov ecx, PrC
		repne scasb
		test ecx, ecx
		jz q
		jmp prefixStart
	q:	
		mov edi, ebx
		ret
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
	inc ecx
	mov [prefix + ecx],0f2h
	inc ecx
	mov [prefix + ecx],0f3h
	pop ecx
	ret
prefixInit endp
end
