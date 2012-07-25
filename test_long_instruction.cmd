;@goto -)
		.686
		.MMX
		.XMM
		.model	flat,stdcall
		option	casemap:none
		COUNT equ 30000

include kernel32.inc
includelib kernel32.lib

		.code
main proc

commands MACRO
		rept COUNT
			db 67h, 0fdh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 03h, 3Ch
			db 26h, 077h, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 03h, 14h
			db 3fh, 0b8h, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 0fh, 01h, 24h
			endm
		endm
commandsAdd MACRO
		rept COUNT
			db 2eh, 26h, 65h, 03h, 7Ch
			db 3eh, 26h, 65h, 03h, 7Ch
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
