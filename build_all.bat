@echo off
build_instruction_list.pl template_full
build_table.pl
build_modRM_table.pl
build_prefix_table.pl
for %%A in (ml.exe,link.exe, cl.exe) do if "%%~$path:A"=="" call vspath
ml /nologo /c /coff fsmA.asm
cl /nologo /c fsm.c
link /nologo /subsystem:console fsm.obj  fsmA.obj
del fsm.obj
del fsmA.obj