.686p
.model flat, C
.stack 4096

.code
DR_InvokeWithCallDeclStd proc
	mov eax, 0

	retn
DR_InvokeWithCallDeclStd endp

DR_InvokeWithCallDeclFast proc
	mov eax, 0

	retn
DR_InvokeWithCallDeclFast endp

DR_InvokeWithCallDeclVector proc
	mov eax, 0

	retn
DR_InvokeWithCallDeclVector endp

end
