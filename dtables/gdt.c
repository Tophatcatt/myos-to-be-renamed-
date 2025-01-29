#include <stdint.h>
struct __attribute__((__packed__)) GDTseg {
  uint16_t base;
  uint32_t limit;
  uint8_t access_byte;
  uint8_t flags;
};
struct __attribute__((__packed__)) GDTbounds {
  uint16_t limit;
  uint32_t base;
};
static struct GDTseg segments[5];
static struct GDTbounds bounds;
uint8_t getflags(struct GDTseg src) { return src.flags & 15; }
void encodeGdtEntry(uint8_t *target, struct GDTseg source) {
  // Check the limit to make sure that it can be encoded
  if (source.limit > 0xFFFFF) {
    // kerror("GDT cannot encode limits larger than 0xFFFFF");
    return;
  }

  // Encode the limit
  target[0] = source.limit & 0xFF;
  target[1] = (source.limit >> 8) & 0xFF;
  target[6] = (source.limit >> 16) & 0x0F;

  // Encode the base
  target[2] = source.base & 0xFF;
  target[3] = (source.base >> 8) & 0xFF;
  target[4] = (source.base >> 16) & 0xFF;
  target[7] = (source.base >> 24) & 0xFF;

  // Encode the access byte
  target[5] = source.access_byte;

  // Encode the flags
  target[6] |= (source.flags << 4);
}
void setGDT(uint8_t idx, uint32_t lim, uint32_t bas, uint8_t flag,
            uint8_t access) {
  segments[idx].limit = lim;
  segments[idx].base = bas;
  segments[idx].flags = flag;
  segments[idx].access_byte = access;
}
void installGDT() {
  setGDT(0, 0x0, 0x0, 0x0, 0x0);
  setGDT(1, 0xfffff, 0x0, 0xc, 0x9A);
  setGDT(2, 0xfffff, 0x0, 0xc, 0x92);
  setGDT(3, 0xfffff, 0x0, 0xc, 0xfa);
  setGDT(4, 0xfffff, 0x0, 0xc, 0xf2);
  // setGDT(5, sizeof(struct GDTseg) - 1, (uint32_t) & (segments[5]), 0x0,
  // 0x89);
  bounds.base = (uint32_t)segments + 0xC0000000;
  bounds.limit = (sizeof(struct GDTseg) * 5) - 1;
  asm("lgdtl (%0)" : : "m"(bounds));
  return;
}
void flush_gdt() {
  asm volatile("mov $0x10, %ax \n\t"
               "mov %ax, %ds \n\t"
               "mov %ax, %es \n\t"
               "mov %ax, %fs \n\t"
               "mov %ax, %gs \n\t"
               "mov %ax, %ss \n\t"
               "ljmp $0x08, $1f\n"
               "1:\n\t"
               "pop %edi \n\t"
               "push $0x8 \n\t"
               "push %edi \n\t"
               "ret");
}
