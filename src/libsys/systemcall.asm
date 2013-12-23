global systemcall
systemcall:
	mov ebp, esp

	push ecx
	push edx
	push ebx
	push ebp
	push esi
	push edi

	mov eax, [ebp + 4]
	mov ebx, [ebp + 8]
	mov ecx, [ebp + 12]
	mov edx, [ebp + 16]
	int 0x80

	pop edi
	pop esi
	pop ebp
	pop ebx
	pop edx
	pop ecx

	ret
