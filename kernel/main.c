#include "../dtables/gdt.c"
#include "../dtables/idt.c"
#include <stdint.h>
// extern void reloadSegments();
void kernel_main(void) {
  /* Initialize terminal */
  terminal_initialize();
  // installGDT();
  // flush_gdt();
  idt_init();

  terminal_writestring("Hello, kernel World!\nNow with Newlines\n");
  terminal_errorstring("also an error message\n");
  terminal_writestring("back to normal\n");
  terminal_successstring("success message\n");
  terminal_writeint(69420);
  newline();
  terminal_writeint(80085);
  // terminal_writestring("\nlol\n");
  // // terminal_writeint(get_cursor_position());
  // newline();
  // // terminal_writeint(get_cursor_xcoord());
  // terminal_uproot();
  // uint16_t lol = 10 / erry;
  // asm("div %0" ::"r"(0)); //  terminal_writestring("hello");
  uint32_t zero = 0;

  // asm volatile("int $0" : : "m"(zero));
}
