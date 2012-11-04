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
			db 065h, 064h, 026h, 036h, 03eh, 02eh, 067h, 066h, 03fh, 02fh, 0f0h, 02fh, 067h, 066h, 00fh, 012h
			db 065h, 064h, 026h, 036h, 03eh, 02eh, 067h, 066h, 03fh, 02fh, 0f0h, 02fh, 067h, 026h, 00fh, 049h
			db 065h, 064h, 026h, 036h, 03eh, 02eh, 067h, 066h, 03fh, 02fh, 0f0h, 02fh, 067h, 02eh, 00fh, 0b6h
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
