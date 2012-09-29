	.686
	.MMX
	.XMM
	.model	flat,stdcall
	option	casemap:none
	.code
	
	include kernel32.inc
	includelib kernel32.lib

main proc
;enter code here
	main endp
	
;адрес следующего байта хранится в регистре edi
;адрес таблицы переходов для опкодов - в esi
getInstruction proc
	
	call getPrefix
	
	mov ebx, 0
	start:
		lodsb
		push ebx ;сохраняем текущее состояние
		shl ebx, 9 ;умножаем на ширину таблицы и еще на 2, потому что ячейки в таблице 2 байта
		shl eax, 1 ; по аналогичной причине умножаем входной байт на 2
		add eax, ebx ;получаем смещение, по которому хранится следующее состояние
		mov ebx, [esi + eax] ;получаем слдующее состояние
		or ebx, ebx ;сравниваем его с 0
		jnz start
		pop ebx 
	;заканчиваем работу функции
	;надо вернуть из нее что-то
	;и, возможно, сбалансировать стек
getInstruction endp

getPrefix proc

getPrefix endp
	
end main