

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
