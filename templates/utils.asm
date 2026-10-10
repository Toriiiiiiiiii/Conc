; TODO: DELETE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
format elf64 executable

segment readable executable
entry main
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Conc compiled program utilities

; Prints an integer from %rdi
print_int:
	push rbp
	mov rbp, rsp

	mov rax, rdi

	push rsi
	push rdi

	; Allocate space for string
	; 64-bit integer limit = 18,446,744,073,709,551,615
	;    => Need maximum of 20 characters for string,
	;       with one extra character for null terminator.
	;    => Multiply by 2 for normal and reversed string.
	sub rsp, 42
	mov rdi, rsp
	mov rsi, rdi

	; rdi = base of the string
	; rsi = next char index

.loop:
	mov rdx, 0

	mov rcx, 10
	div rcx

	add rdx, 48
	mov [rsi], dl
	inc rsi

	cmp rax, 0
	jne .loop


	; Reverse the string
	; rax = string length
	; rsi = end of source string
	; rdi = rev string pointer
	mov rax, rsi
	inc rax

	sub rax, rdi
	add rdi, 21

	push rax
	push rsi
	push rdi
.revloop:
	mov cl, [rsi]
	mov [rdi], cl

	inc rdi
	dec rsi
	dec rax

	cmp rax, 0
	jne .revloop

	; Add newline
	mov dl, 10
	mov [rdi], dl
	inc rdi

	pop rdi
	pop rsi
	pop rax

	mov rsi, rdi
	mov rdi, 1
	mov rdx, rax
	inc rdx
	mov rax, 1
	syscall

	pop rdi
	pop rsi

	mov rsp, rbp
	pop rbp
	ret

; TODO: DELETE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
main:
	push rbp
	mov rbp, rsp

	mov rdi, 9876543210
	call print_int

	mov rax, 60
	mov rdi, 0
	syscall

	mov rsp, rbp
	pop rbp
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
