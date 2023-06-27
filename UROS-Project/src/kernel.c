#include "include/gdi/graphics.h"
#include "include/io/mem.h"
#include "include/io/typedefs.h"
#include "include/io/math.h"

void kmain()
{
    InitMath();
    FillScreen(0x4287F5); // 0x009999 or 4287F5
	DrawString("\nExecuted /kernel.sys", 0, 0, 0x00FF00);
	DrawString("\n\nFirst clock init:", 0, 0, 0xFFA500);
    Exception_Handle();
    CreateWindow(100, 80, 500, 300, "Window Test");
    halt();
}
