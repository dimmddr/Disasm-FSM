;@goto -)
		.686
		.MMX
		.XMM
		.model	flat,stdcall
		option	casemap:none
		COUNT equ 100000

include kernel32.inc
includelib kernel32.lib

		.code
main proc

commands MACRO
		rept COUNT
			db 066h, 036h, 069h, 080h, 036h, 010h, 036h, 010h
			db 065h, 03eh, 026h, 0e9h, 036h, 049h, 036h, 049h
			db 0f0h, 064h, 06bh, 084h, 06bh, 084h, 036h, 0b6h
			endm
		endm
	
		commands
		invoke	ExitProcess, 0
		main endp
end main

:-)
@echo off
for %%A in (ml.exe,link.exe) do if "%%~$path:A"=="" call vspath
ml /nologo /c /coff %1 %~f0
link /merge:.rdata=.text /subsystem:console /nologo %~dpn0.obj
del %~dpn0.obj
