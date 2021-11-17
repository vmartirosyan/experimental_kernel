#ifndef REGISTERS_H
#define REGISTERS_H

struct registers
{
    unsigned int gs, fs, es, ds;      
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 
    unsigned int int_number, err_code;   
    unsigned int eip, cs, eflags, useresp, ss;  
};

#endif 
