#include <stdint.h>

// Define the GDT structure
struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity; // this is both the flags and limit_high part of the gdt
                       // table
  uint8_t base_high;
};
// this is the same as in the global descriptor table page on osdev website
// just for the love of god, read the thing from 0 to 63 and not the opposite

struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

// Define the GDT entries
struct gdt_entry gdt_entries[4];
struct gdt_ptr gdt_ptr;

void set_gdt_entry(struct gdt_entry *entry, uint32_t base, uint32_t limit,
                   uint8_t access, uint8_t granularity) {
  entry->base_low = (base & 0xFFFF);
  entry->base_middle = (base >> 16) & 0xFF;
  entry->base_high = (base >> 24) & 0xFF;

  entry->limit_low = (limit & 0xFFFF);
  entry->granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

  entry->access = access;
}

void init_gdt() {
  // Null descriptor
  set_gdt_entry(&gdt_entries[0], 0, 0, 0, 0);

  // Code segment descriptor
  set_gdt_entry(&gdt_entries[1], 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // Data segment descriptor
  set_gdt_entry(&gdt_entries[2], 0, 0xFFFFFFFF, 0x92, 0xCF);
  set_gdt_entry(&gdt_entries[3], (uint32_t)(&gdt_entries[3]),
                sizeof(struct gdt_entry) - 1, 0x98, 0x00);

  // these two segments are kernel code segments, not user.
  // Set up the GDT pointer
  gdt_ptr.limit = sizeof(struct gdt_entry) * 3 - 1;
  gdt_ptr.base = (uint32_t)&gdt_entries;

  // Load the GDT
  asm volatile("lgdt (%0)" ::"r"(&gdt_ptr));

  // Load the segment registers
}
void flush_gdt() {
  asm volatile("mov $0x10, %ax \n\t"
               "mov %ax, %ds \n\t"
               "mov %ax, %es \n\t"
               "mov %ax, %fs \n\t"
               "mov %ax, %gs \n\t"
               "mov %ax, %ss \n\t");
}
