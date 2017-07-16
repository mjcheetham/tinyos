#pragma once
#include "types.h"

typedef struct registers
{
    uint32 ds; // data segment selector
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
    uint32 int_no, err_code; // interrupt number and error code (if applicable)
    uint32 eip, cs, eflags, useresp, ss; // pushed by the processor on automatically (iret)
} registers_t;
