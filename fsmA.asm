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
	prefix 	dw 1024 dup(?)
	;fsm for prefix
	state	db 100000 dup(?)
	
public disasm 
	.code
	disasm proc byteAddress, tableAddress, prefixTableAddress, count
	;4 parameters: first byte address, state table address, prefix state table iteration count
	push ebp

	mov esi, byteAddress
	mov ebx, tableAddress
	mov edi, prefixTableAddress
	mov ecx, count
	push ecx ;save count of iteration for next cycle usage
	;сохраняем количество итераций, для следующего цикла
		lfence
		rdtsc
		cld
		push eax
		push edx
			qwer:
				push ecx
					call getInstruction
				pop ecx
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
	pop ebp
	ret
disasm endp

getPrefix proc
	mov ebx, 0
	prefixStart:
		mov edx, ebx
		mov eax, 0
		lodsb
		shl ebx, 8 ;only one byte for cell in prefix state table
		mov ecx, eax
		add ecx, ebx
		mov ebx, 0
		mov bl, [edi + ecx]
		test ebx, ebx ;compare state and 0
		jnz prefixStart
		test edx, edx
		jz q
		sub edx, 12
		ja q
		sub esi, 2
		lodsb
	q:
		ret
getPrefix endp

;next byte address in edi
;state table address in esi
;адрес следующего байта хранится в регистре edi
;адрес таблицы переходов для опкодов - в esi
getInstruction proc
	push ebx
		call getPrefix
	pop ecx
	push edx ;save prefix state
		mov ebx, 0
		instructionStart:
			mov edx, ebx ;keep current state 
			;сохраняем текущее состояние
			;shl ebx, 8 ;multiply by width of the state table
			;умножаем на ширину таблицы
			shl ebx, 9 ;9 - if size of the cell in the state table will be 2 byte
			shl eax, 1 ; for some reasons multiply next byte by 2
			add eax, ebx ;we receive shift on which the following condition is stored
			;получаем смещение, по которому хранится следующее состояние
			mov ebx, 0
			mov bx, [ecx + eax] ;take the next state
			;получаем следующее состояние
			test ebx, ebx ;compare state and 0
			;сравниваем его с 0
			jz exit
			mov eax, 0
			lodsb
			jmp instructionStart
		exit:
		sub esi, 1
		mov ebx, ecx
		call getModRM
		
	pop edx ;load prefix state
	;end of work. KO
	;заканчиваем работу функции
	ret
getInstruction endp

getModRM proc
	
	ret
getModRM endp

prefixInit proc
	push ecx
	push eax
	pop eax
	pop ecx
	ret
prefixInit endp
end
