#include "../dtables/gdt.c"
#include "../dtables/idt.c"
#include <stdint.h>
void kernel_main(void) {
  terminal_initialize();
  idt_init();
  terminal_writestring("Hello, kernel World!\nNow with Newlines\n");
  terminal_writestring("back to normal \n");
  // asm("int $0");
  terminal_successstring("success message \n");
  terminal_writeint(69420);
  newline();
  terminal_writeint(80085);
  newline();
  // asm("int $0");
  terminal_uproot();
  newline();
  terminal_writeint(68);
  newline();
  asm volatile("hlt");
}
