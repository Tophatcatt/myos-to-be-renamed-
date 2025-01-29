#include "../libs/tty.c"
#include <stdbool.h>
#include <stdint.h>
////
////
typedef struct {
  uint16_t address_low;
  // uint16_t kernel_cs;
  uint16_t selector;
  uint8_t reserved;
  uint8_t flags;
  uint16_t address_high;
} __attribute__((packed)) idt_entry_t;
typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idtr_t;
idt_entry_t idt[32];
////
////
///
///
struct __attribute__((packed)) cpu_status_t {
  uint32_t edi;
  uint32_t esi;
  uint32_t edx;
  uint32_t ecx;
  // other pushed registers
  uint32_t ebx;
  uint32_t eax;

  uint32_t vector_number;
  uint32_t error_code;

  uint32_t iret_eip;
  uint32_t iret_cs;
  uint32_t iret_flags;
  uint32_t iret_esp;
  uint32_t iret_ss;
};
//__attribute__((noreturn)) void interrupt_handler(void); //might need idk
void interrupt_handler(struct cpu_status_t *context) {
  terminal_errorstring("interrupt triggered");
  uint32_t vec_num = context->vector_number;
  terminal_errorstring("exception vector tripped");
  return;
  switch (vec_num) {
  case 0:
    terminal_errorstring("divided by zero error");
    break;
  case 13:
    terminal_errorstring("general protection fault.");
    break;
  case 14:
    terminal_errorstring("page fault.");
    break;
  default:
    terminal_errorstring("unexpected interrupt.");
    break;
  }
  return;
  //__asm__ volatile("cli; hlt"); // Completely hangs the computer
}
//////////
///
///
///
inline uint16_t cs(void) {
  uint16_t val;
  asm volatile("mov %%cs, %0" : "=r"(val));
  return val;
}
static uint8_t dpl = 0;
void idt_set_descriptor(uint8_t vector, void *handler, uint8_t flags) {
  idt_entry_t *entry = &idt[vector];
  // entry->kernel_cs = cs();
  entry->reserved = 0;
  entry->address_low = (uint32_t)handler & 0xffff;
  entry->address_high = (uint32_t)handler >> 16;
  // your code selector may be different!
  entry->selector = 0x08;
  // trap gate + present + DPL
  entry->flags = 0x8e; // 0b1110 | ((dpl & 0b11) << 5) | (1 << 7);
  // ist disabled
}
static uint16_t IDT_MAX_DESCRIPTORS = 32;

extern void *vector_0_handler;
static bool vectors[32];

// extern void *isr_stub_table[];
void idt_init(void);
void idt_init() {
  idtr_t idtr;
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
  for (uint8_t vector = 0; vector < 32; vector++) {
    void *handler = vector_0_handler + (vector * 16);
    // void *handler = isr_stub_table[vector];
    idt_set_descriptor(vector, handler, 0x8);
    vectors[vector] = true;
  }

  asm volatile("lidt %0" : : "m"(idtr)); // load the new IDT
  asm volatile("sti");                   // set the interrupt flag
}
