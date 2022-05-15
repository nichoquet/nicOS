[extern kprint]
%define THREAD_FLAGS    CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_PARENT|CLONE_THREAD|CLONE_IO
%define STACK_SIZE      (4096 * 1024)
%define MAX_LINES       10000                  ;number of output lines before exiting

global thread_create

_start:
;spawn a few threads
    mov	    edi, threadfn1
    call    thread_create
    mov	    edi, threadfn2
    call    thread_create

.loop:
    call    check_count
    mov	    edi, hello
    call    puts
    mov	    edi, 0
    jmp	    .loop

threadfn1:
    call    check_count
    mov	    edi, hello1
    call    puts
    jmp	    threadfn1

threadfn2:
    call    check_count
    mov	    edi, hello2
    call    puts
    jmp	    threadfn2

; -- may not return
check_count:
    mov	    eax, -1
    lock   xadd [count], eax
    jl	    .exit
    ret
.exit:
    syscall exit, 0

puts:
    mov	    esi, edi
    mov	    edx, -1
.count:
    inc	    edx
    cmp	    byte[esi + edx], 0
    jne	    .count
    syscall write, stdout
    ret

thread_create:
    push    edi
    call    stack_create
    lea	    esi, [eax + STACK_SIZE - 8]
    pop	    qword [esi]
    syscall clone, THREAD_FLAGS
    ret

stack_create:
    syscall mmap, 0, STACK_SIZE,PROT_WRITE | PROT_READ,MAP_ANONYMOUS | MAP_PRIVATE | MAP_GROWSDOWN
    ret


count:  dq MAX_LINES
hello:  db `Hello from \e[93;1mmain!\e[0m\n\0`
hello1: db `Hello from \e[91;1mthread 1!!\e[0m\n\0`
hello2: db `Hello from \e[96;1mthread 2!!\e[0m\n\0`