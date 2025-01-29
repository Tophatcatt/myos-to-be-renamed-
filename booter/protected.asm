extern init_gdt
extern flush_gdt
extern kernel_main
global protected_mode
protected_mode:
cli            ; disable interrupts
call init_gdt    ; load GDT register with start address of Global Descriptor Table
mov eax, cr0 
or al, 1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
mov cr0, eax
call flush_gdt
jmp 08h:Pmodemain

Pmodemain:
call kernel_main

