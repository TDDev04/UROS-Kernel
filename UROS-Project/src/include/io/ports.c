#include "typedefs.h"

void outb(uint16_t port, char data) {
    asm volatile("outb %0, %1" : : "a"( data ), "Nd"( port ));
}

char inb(uint16_t port) {
    char result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

short inw(uint16_t port) {
    short result;
    asm volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}