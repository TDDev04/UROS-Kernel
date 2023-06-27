#pragma once
#include "typedefs.h"

void outb(uint16_t port, char data);
char inb(uint16_t port);
short inw(uint16_t port);