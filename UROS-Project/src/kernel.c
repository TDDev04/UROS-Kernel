#include "include/gdi/graphics.h"
#include "include/mem.h"
#include "include/test.h"
#include "include/typedefs.h"

void kmain()
{
    //FillScreen(0x4287F5); // 0x009999 or 4287F5

	DrawString("euscalva", 0, 0, 0x00FF00);
	DrawString("\nExecuted /kernel.sys", 0, 0, 0x00FF00);
	DrawString("\n\nFirst clock init:", 0, 0, 0xFFA500);
    //CreateWindow(100, 80, 500, 300, "GOP/VBE Window test 1");
    //DrawString(test, 107, 105, 0);
}
