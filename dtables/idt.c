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
//__attribute__((noreturn)) void interrupt_handler(void); //might need idk
uint32_t getecx() {
  uint32_t val;
  asm volatile("movl %%ecx, %0" : "=r"(val));
  return val;
}
void interrupt_handler(uint32_t vecnum) {
  // terminal_errorstring("interrupt triggered\n");
  // terminal_errorstring("exception vector tripped\n");
  // uint32_t vecnum = getecx();
  switch (vecnum) {
  case 0:
    terminal_writeint(vecnum);
    terminal_errorstring("divided by zero error\n");
    break;
  case 13:
    terminal_errorstring("general protection fault.\n");
    break;
  case 14:
    terminal_errorstring("page fault.\n");
    break;
  default:
    terminal_errorstring("unexpected interrupt.\n");
    terminal_writeint(vecnum);
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

// extern void *vector_0_handler;
static bool vectors[32];

extern void *isr_stub_table[];
void idt_init(void);
void idt_init() {
  idtr_t idtr;
  idtr.base = (uintptr_t)&idt[0];
  idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
  for (uint8_t vector = 0; vector < 32; vector++) {
    // void *handler = vector_0_handler + (vector * 16);
    void *handler = isr_stub_table[vector];
    idt_set_descriptor(vector, handler, 0x8);
    vectors[vector] = true;
  }

  asm volatile("lidt %0" : : "m"(idtr)); // load the new IDT
  asm volatile("sti");                   // set the interrupt flag
}
