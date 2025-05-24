.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	includelib ../Debug/SMD_2024Lib.lib
	ExitProcess PROTO :DWORD


	getCurrentDate PROTO :DWORD 
	getCurrentMinuts PROTO :DWORD 
	getCurrentHour PROTO :DWORD 
outputIntNum PROTO :DWORD
outputSymb PROTO :BYTE
outputLine PROTO :DWORD

.stack 4096
.const
divideOnZeroExeption BYTE "Попытка деления на ноль.", 0  ;STR, для вывода ошибки при делении на ноль
	Demo$LEX1 DWORD 5 ;INT
	Main$LEX2 DWORD 1 ;INT
	Main$LEX5 BYTE "Date", 0  ;STR
	Main$LEX6 BYTE "Hours", 0  ;STR
	Main$LEX7 BYTE "Minuts", 0  ;STR
	Main$LEX8 BYTE "Demo Function", 0  ;STR
	Main$LEX9 DWORD 2 ;INT
	Main$LEX10 DWORD 10 ;INT
	Main$LEX11 DWORD 200 ;INT
	Main$LEX12 DWORD 3 ;INT
	Main$LEX13 BYTE 'k' ;CHR
	Main$LEX14 BYTE "Symbol", 0  ;STR
	Main$LEX15 DWORD 18 ;INT
	Main$LEX18 BYTE "18<<1:", 0  ;STR
	Main$LEX19 BYTE "18>>1:", 0  ;STR
	Main$LEX20 DWORD 0 ;INT
.data
	Demob DWORD 0 ;INT
	Mainnum1 DWORD 0 ;INT
	Mainnum2 DWORD 0 ;INT
	Mainnum3 DWORD 0 ;INT
	Mainstr1 DWORD 0 ;STR
	Mainstr2 DWORD 0 ;STR
	Mainstr3 DWORD 0 ;STR
	Mainfunc DWORD 0 ;INT
	Mainx DWORD 0 ;INT
	Mainy DWORD 0 ;INT
	Mainsym BYTE 0 ;CHR
	Mainn DWORD 0 ;INT
	Mainm DWORD 0 ;INT
	Mainl DWORD 0 ;INT

.code
$Demo PROC uses ebx ecx edi esi ,	Demoa: DWORD 
; String #4 :ivl
push Demo$LEX1
pop Demob

If17Start: 
mov eax, Demoa
mov ebx, Demob
cmp eax, ebx
ja If17End

; String #9 :iviiv
push Demoa
push Demob
pop ebx
pop eax
add eax, ebx 
push eax
pop Demob
If17End: 

mov eax, Demob
ret
$Demo ENDP

main PROC

; String #23 :ivil@1
invoke getCurrentDate, Main$LEX2
push eax ;результат функции
pop Mainnum1

; String #25 :ivil@1
invoke getCurrentHour, Main$LEX2
push eax ;результат функции
pop Mainnum2

; String #27 :ivil@1
invoke getCurrentMinuts, Main$LEX2
push eax ;результат функции
pop Mainnum3

; String #35 :ivl
push offset Main$LEX5
pop Mainstr1

push Mainstr1
CALL outputLine

push Mainnum1
CALL outputIntNum

; String #38 :ivl
push offset Main$LEX6
pop Mainstr2

push Mainstr2
CALL outputLine

push Mainnum2
CALL outputIntNum

; String #41 :ivl
push offset Main$LEX7
pop Mainstr3

push Mainstr3
CALL outputLine

push Mainnum3
CALL outputIntNum

push offset Main$LEX8
CALL outputLine

; String #47 :ivil@1
invoke $Demo, Main$LEX9
push eax ;результат функции
pop Mainfunc

push Mainfunc
CALL outputIntNum

; String #50 :ivl
push Main$LEX10
pop Mainx

; String #52 :ivl
push Main$LEX11
pop Mainy

While174Start: 
mov eax, Mainx
mov ebx, Mainy
cmp eax, ebx
ja While174End

; String #55 :ivilv
push Mainx
push Main$LEX12
pop ebx
pop eax
mul ebx
push eax
pop Mainx

push Mainx
CALL outputIntNum
jmp While174Start
While174End: 

; String #61 :ivl
movzx eax, Main$LEX13
push eax 
pop eax
mov Mainsym, al

push offset Main$LEX14
CALL outputLine
push eax
movzx eax, Mainsym
push eax
CALL outputSymb
pop eax


; String #66 :ivl
push Main$LEX15
pop Mainn

; String #68 :ivilv
push Mainn
push Main$LEX2
pop ebx
pop eax
push ecx ; сохраняем данные регистра ecx
mov ecx, ebx
SHL eax, cl
pop ecx
push eax
pop Mainm

; String #70 :iviil@1v
push Mainn
invoke $Demo, Main$LEX9
push eax ;результат функции
pop ebx
pop eax
push ecx ; сохраняем данные регистра ecx
mov ecx, ebx
SHR eax, cl
pop ecx
push eax
pop Mainl

push offset Main$LEX18
CALL outputLine

push Mainl
CALL outputIntNum

push offset Main$LEX19
CALL outputLine

push Mainm
CALL outputIntNum

mov eax, Main$LEX20
	jmp endPoint
	div_by_0:
	push offset divideOnZeroExeption
CALL outputLine
endPoint:
	invoke		ExitProcess, eax
main ENDP
end main