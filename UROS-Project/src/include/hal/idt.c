#include "idt.h"

extern void irq_stub_0(void),
            irq_stub_1(void),
            irq_stub_nothing(void);

// https://wiki.osdev.org/Interrupts_tutorial#Entries
// we are just setting up an idt for x86 processors
typedef struct {
    uint16_t offset_low;    // The lower 16 bits of the ISR's address
    uint16_t selector;      // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t zero;           // Set to zero
    uint8_t flags;          // Flags
    uint16_t offset_high;   // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t; // the entry of our IDT

struct {
	uint16_t	limit;
	void*	    base;
} __attribute__((packed)) idt_r; // basically our IDT pointer

// generate an array with 256 descriptors

static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

// remap the PIC (https://github.com/bnoordhuis/marnix/blob/master/pic.c)

// after looking at this, i wont suggest it to run it on a real hardware because
// if ther is a single issue with the interrupts causing an unhandled exception
// there is a chance that it might brick the hardware.

void initialize_idt()
{
    //load the idt
    idt_r.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_r.base = idt;
    
    // remap the pic
    uint8_t mask[2];

    // leave at least some ms before the pic is remaped
    outb(0x80, 0x00);

    outb(0x20, 0x11);
    outb(0xa0, 0x11);

    outb(0x21, 0x20); // map to interrupt range 0x20-0x27
    outb(0xa1, 0x28); // map to interrupt range 0x28-0x2f

    outb(0x21, 4);    // have slave at IRQ2
    outb(0xa1, 2);    // ping master through IRQ2

    outb(0x21, 1);    // 8086 mode
    outb(0xa1, 1);    // ditto

    outb(0x21, mask[0]);
    outb(0xa1, mask[1]);

    // remap is done.
    
    // configure the channel 0, stands for PIT
    idt[32].offset_low  = (uint32_t)irq_stub_0 >>  0 & 0xFFFF;
	idt[32].offset_high = (uint32_t)irq_stub_0 >> 16 & 0xFFFF;
	idt[32].selector    = 8;
	idt[32].flags       = 0x8E;

    // configure channel 1, stands for the keyboard input
    idt[33].offset_low  = (uint32_t)irq_stub_1 >>  0 & 0xFFFF;
	idt[33].offset_high = (uint32_t)irq_stub_1 >> 16 & 0xFFFF;
	idt[33].selector    = 8;
	idt[33].flags       = 0x8E;
    
    extern void load_idt(void);
	load_idt();
	
	// Set PIT frequency
	uint16_t freq = 7159092 / (6 * FREQUENCY);
	outb(0x43, 0x34); // Channel 0, rate generator
	outb(0x40, freq & 0xFF);
	outb(0x40, freq >> 8);
	
	asm("sti");
}