; Defined in isr.c
[EXTERN isr_handler]

isr_common:
    pusha                ; pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds           ; lower 16 bits of eax = ds
    push eax             ; save the data segment descriptor

    mov ax, 0x10         ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop eax              ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                 ; pops edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8           ; clean up the pushed error code and pushed ISR number
    sti                  ; enable interrupts
    iret                 ; return from an interrupt..
                         ; ..pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


; Define macro for interrupt handler without an error code
%macro ISR_NOERROR 1
    [GLOBAL isr%1]
    isr%1:
        cli              ; disable interrupts
        push byte 0      ; push a dummy error code
        push byte %1     ; push the interrupt number
        jmp isr_common   ; jump to the common handler
%endmacro

; Define macro for interrupt handler with an error code
%macro ISR_ERROR 1
    [GLOBAL isr%1]
    isr%1:
        cli              ; disable interrupts
        push byte %1     ; push the interrupt number
        jmp isr_common   ; jump to the common handler
%endmacro

; Set up ISRs 0 to 31
ISR_NOERROR 0
ISR_NOERROR 1
ISR_NOERROR 2
ISR_NOERROR 3
ISR_NOERROR 4
ISR_NOERROR 5
ISR_NOERROR 6
ISR_NOERROR 7
ISR_ERROR   8
ISR_NOERROR 9
ISR_ERROR   10
ISR_ERROR   11
ISR_ERROR   12
ISR_ERROR   13
ISR_ERROR   14
ISR_NOERROR 15
ISR_NOERROR 16
ISR_NOERROR 17
ISR_NOERROR 18
ISR_NOERROR 19
ISR_NOERROR 20
ISR_NOERROR 21
ISR_NOERROR 22
ISR_NOERROR 23
ISR_NOERROR 24
ISR_NOERROR 25
ISR_NOERROR 26
ISR_NOERROR 27
ISR_NOERROR 28
ISR_NOERROR 29
ISR_NOERROR 30
ISR_NOERROR 31
