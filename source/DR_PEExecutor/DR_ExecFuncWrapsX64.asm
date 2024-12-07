.data

.code 

DR_InvokeWithCallDeclX64 proc
	push rbp
	mov rbp, rsp
	sub rsp, 24

	callbackAddressOffset equ 8
	argsCountOffset equ 16
	argsPtrOffset equ 24

	mov [rbp - callbackAddressOffset], rcx
	mov [rbp - argsCountOffset], rdx
	mov [rbp - argsPtrOffset], r8

	mov r11, r8
	cmp rdx, 0
	jz _noArgs

	cmp rdx, 4
	jz _setArgs4
	jns _setStackArgs

	cmp rdx, 3
	jz _setArgs3

	cmp rdx, 2
	jz _setArgs2

	cmp rdx, 1
	jz _setArgs1

_setStackArgs:
	mov rax, rdx
	sub rax, 5

_loopStart:
	add rax, 4
	mov r10, [r11 + rax * 8]
	sub rax, 4

	push r10

	dec rax
	jns _loopStart

_setArgs4:
	mov r9, [r11 + 24]
_setArgs3: 
	mov r8, [r11 + 16]
_setArgs2:
	mov rdx, [r11 + 8]
_setArgs1:
	mov rcx, [r11 + 0]
_noArgs:
	push rcx
	push rdx
	push r8
	push r9

	call QWORD ptr [rbp - callbackAddressOffset]

	pop r9
	pop r8
	pop rdx
	pop rcx

	mov rdx, [rbp - argsCountOffset]
	cmp rdx, 5
	js _noClearStackArgs

	mov rdx, [rbp - argsCountOffset]
	sub rdx, 5

_clearStackArgs:
	pop r8
	dec rdx
	jns _clearStackArgs

_noClearStackArgs:
	add rsp, 24
	pop rbp
	ret
DR_InvokeWithCallDeclX64 endp

end