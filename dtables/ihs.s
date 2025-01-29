.extern interrupt_handler
interrupt_stub:
pushl %eax
pushl %ebx
//push other registers here
pushl %ecx
pushl %edx
pushl %esi
pushl %edi
mov %esp, %edi
call interrupt_handler
mov %eax, %esp

popl %edi
popl %esi
popl %edx
popl %ecx
popl %ebx
popl %eax

//remove the vector number + error code
add $16, %esp

iret

//automate with macro later
.align 16
.global vector_0_handler
vector_0_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $0
jmp interrupt_stub


.align 16
vector_1_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $1
jmp interrupt_stub


.align 16
vector_2_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $2
jmp interrupt_stub


.align 16
vector_3_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $3
jmp interrupt_stub


.align 16
vector_4_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $4
jmp interrupt_stub

.align 16
vector_5_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $5
jmp interrupt_stub

.align 16
vector_6_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $6
jmp interrupt_stub

.align 16
vector_7_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $7
jmp interrupt_stub

.align 16
vector_8_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $8
jmp interrupt_stub

.align 16
vector_9_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $9
jmp interrupt_stub

.align 16
vector_10_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $10
jmp interrupt_stub

.align 16
vector_11_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $11
jmp interrupt_stub

  .align 16
vector_12_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $12
jmp interrupt_stub

.align 16
vector_13_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $13
jmp interrupt_stub

.align 16
vector_14_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $14
jmp interrupt_stub

.align 16
vector_15_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $15
jmp interrupt_stub

  .align 16
vector_16_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $16
jmp interrupt_stub

.align 16
vector_17_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $17
jmp interrupt_stub

.align 16
vector_18_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $18
jmp interrupt_stub

.align 16
vector_19_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $19
jmp interrupt_stub

.align 16
vector_20_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $20
jmp interrupt_stub

.align 16
vector_21_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $21
jmp interrupt_stub

.align 16
vector_22_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $22
jmp interrupt_stub

.align 16
vector_23_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $23
jmp interrupt_stub

  .align 16
vector_24_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $24
jmp interrupt_stub

.align 16
vector_25_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $25
jmp interrupt_stub

.align 16
vector_26_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $26
jmp interrupt_stub

.align 16
vector_27_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $27
jmp interrupt_stub

.align 16
vector_28_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $28
jmp interrupt_stub

.align 16
vector_29_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $29
jmp interrupt_stub

.align 16
vector_30_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $30
jmp interrupt_stub

.align 16
vector_31_handler:
//vector 0 has no error code
pushl $0
//the vector number
pushl $31
jmp interrupt_stub


