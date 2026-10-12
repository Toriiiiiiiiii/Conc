format elf64
section '.text' executable

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

.clear:
	push rdi
	push rsi
	push rax

	mov rax, 0
	add rsi, 42
.clearlp:
	mov [rdi], al
	inc rdi
	cmp rdi, rsi
	jne .clearlp
.clear_dn:
	pop rax
	pop rsi
	pop rdi

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

public _start

_start:
	push rbp
	mov rbp, rsp

	call FN_main

	call stk_pop
	mov rdi, rax
	mov rax, 60
	syscall
	
	mov rsp, rbp
	pop rbp
	ret

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; AUTO-GENERATED CODE
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
FN_factorial:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	call stk_pop
	mov [rsp+0], rax
	mov rax, [rsp+0]
	call stk_push
	mov rax, 0
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	cmp rax, rdi
	jne FL_0
	mov rax, 1
	call stk_push
	jmp CMP_0_DN
FL_0:
	mov rax, 0
	call stk_push
CMP_0_DN:
	call stk_pop
	cmp rax, 0
	je ELSE_0
	mov rax, 1
	call stk_push
	jmp END_0
ELSE_0:
	mov rax, [rsp+0]
	call stk_push
	mov rax, 1
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	sub rax, rdi
	call stk_push
	call FN_factorial
	mov rax, [rsp+0]
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	mul rdi
	call stk_push
END_0:
	mov rsp, rbp
	pop rbp
	ret
FN_fib:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	call stk_pop
	mov [rsp+0], rax
	mov rax, [rsp+0]
	call stk_push
	mov rax, 2
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	cmp rax, rdi
	jge FL_1
	mov rax, 1
	call stk_push
	jmp CMP_1_DN
FL_1:
	mov rax, 0
	call stk_push
CMP_1_DN:
	call stk_pop
	cmp rax, 0
	je ELSE_1
	mov rax, 1
	call stk_push
	jmp END_1
ELSE_1:
	mov rax, [rsp+0]
	call stk_push
	mov rax, 1
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	sub rax, rdi
	call stk_push
	call FN_fib
	mov rax, [rsp+0]
	call stk_push
	mov rax, 2
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	sub rax, rdi
	call stk_push
	call FN_fib
	call stk_pop
	mov rdi, rax
	call stk_pop
	add rax, rdi
	call stk_push
END_1:
	mov rsp, rbp
	pop rbp
	ret
FN_add:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	call stk_pop
	mov [rsp+0], rax
	call stk_pop
	mov [rsp+8], rax
	mov rax, [rsp+8]
	call stk_push
	mov rax, [rsp+0]
	call stk_push
	call stk_pop
	mov rdi, rax
	call stk_pop
	add rax, rdi
	call stk_push
	mov rsp, rbp
	pop rbp
	ret
FN_main:
	push rbp
	mov rbp, rsp
	sub rsp, 0
	mov rax, 10
	call stk_push
	call FN_factorial
	call stk_pop
	mov rdi, rax
	call print_int
	mov rax, 10
	call stk_push
	call FN_fib
	call stk_pop
	mov rdi, rax
	call print_int
	mov rax, 34
	call stk_push
	mov rax, 35
	call stk_push
	call FN_add
	call stk_pop
	mov rdi, rax
	call print_int
	mov rax, 0
	call stk_push
	mov rsp, rbp
	pop rbp
	ret


; rax = value to push
stk_push:
	push rbp
	mov rbp, rsp

	push rdi

	mov rdi, [stk_ptr]
	add rdi, stk
	mov [rdi], rax

	mov rdi, [stk_ptr]
	add rdi, 8
	mov [stk_ptr], rdi

	pop rdi

	mov rsp, rbp
	pop rbp
	ret

stk_pop:
	push rbp
	mov rbp, rsp

	push rdi

	mov rdi, [stk_ptr]
	sub rdi, 8
	mov [stk_ptr], rdi
	add rdi, stk
	mov rax, [rdi]

	pop rdi

	mov rsp, rbp
	pop rbp
	ret

section '.data' writeable

stk: rq 65536
stk_ptr: dq 0
