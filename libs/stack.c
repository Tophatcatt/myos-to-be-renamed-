#include <stdint.h>
// void stackpush(uint32_t val) {
//   asm volatile("movl %0, %%eax"
//                "pushl %%eax\n\t" // Push the value of x onto the stack
//                :                 // output vars go here
//                : "r"(val)        // input vars go here
//                : "eax"           // clobbered or manipulated go here
//   );
// }
// uint32_t stackpop() {
//   uint32_t out;
//   asm volatile("popl %%eax\n\t"
//                "movl %%eax, %0"
//                : "=r"(out)
//                :
//                : "eax");
//   return out;
// }
void stackpush(uint32_t val) {
  asm volatile("pushl %0\n\t" // Push the value of x onto the stack
               :              // output vars go here
               : "r"(val)     // input vars go here
               : "eax"        // clobbered or manipulated go here
  );
}
uint32_t stackpop() {
  uint32_t out;
  asm volatile("popl %0\n\t" : "=r"(out) : : "eax");
  return out;
}
