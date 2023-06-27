#include "vbe.h"
#include "font.h"
#include "../io/math.h"

#define WIDTH 800
#define HEIGHT 600

const int CHAR_WIDTH = 8;
const int CHAR_HEIGHT = 16;

#define RGB(r, g, b) ((uint32_t)(((uint32_t)(uint8_t)(b) | (((uint32_t)(uint8_t)(g)) << 8)) | (((uint32_t)(uint8_t)(r)) << 16)))
#define RGB2UINT(rgb) (RGB(rgb.r, rgb.g, rgb.b))
#define UINT2RGB(uint) (static_cast<COLORRGB>(uint))

void SetPixel(int x, int y, int color)
{
    uint32_t *screen = (uint32_t *)gfx_mode->physical_base_pointer;
    if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT)
    {
        screen[y * WIDTH + x] = color;
    }
}

void FillScreen(int color)
{
    uint32_t *screen = (uint32_t *)gfx_mode->physical_base_pointer;
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        screen[i] = color;
    }
}

void DrawChar(int x, int y, char ch, int color)
{
    int beginX = max(x, 0);
    int beginY = max(y, 0);
    int endX = min(x+CHAR_WIDTH, WIDTH);
    int endY = min(y+CHAR_HEIGHT, HEIGHT);
    for(int cy = beginY; cy < endY; cy++)
    {
        for(int cx = beginX; cx < endX; cx++)
        {
            if(font[(int)ch * 16 + cy - y] & 1 << (7-(cx - x)))
            {
                SetPixel(cx, cy, color);
            }
        }
    }
}

void DrawString(const char *string, int x, int y, int color)
{
	int i = 0;
	while (string[i])
	{
		if (string[i] == '\n')
			y += CHAR_HEIGHT;
		else {
            DrawChar(x, y, string[i], color);
			x += CHAR_WIDTH;
		}
		i++;
	}
}

void IntToString(int number, OUT char* str) 
{
	uint32_t writtenOneDigit = FALSE;
	int writeIdx = 0;
	if (number == 0) {
		// Special case for when the number is 0
		str[0] = '0';
		str[1] = 0;
		return;
	}
	if (number < 0) {
		str[writeIdx++] = '-';
		number *= -1;
	}
	for (int div = 1000000000; div > 0; div /= 10) {
		int digit = number/div;
		if (digit != 0 || writtenOneDigit) {
			writtenOneDigit = TRUE;
			str[writeIdx++] = digit + 48; // convert number digit to string digit
			number -= digit * div;
		}
	}
	str[writeIdx] = 0;
}

void DrawInt(int num, int x, int y, int color) {
	char chars[12];
	IntToString(num, chars);
	DrawString(chars, x, y, color);
}


void CreateWindow(int x, int y, int w, int h, const char *Title)
{
    // create the main window
    for (int i = 0; i < w; i++) 
    {
        for (int j = 0; j < h; j++) 
        {
            SetPixel(x + i, y + j, 0xA0A0A0);
        }
    }

    for (int i = 0; i < w - 6; i++) 
    {
        for (int j = 0; j < h - 25; j++) 
        {
            SetPixel((x + 3) + i, (y + 20) + j, 0xE0E0E0);
        }
    }

    for (int i = 0; i < w -4; i++) 
    {
        for (int j = 0; j < 20; j++) 
        {
            SetPixel((x + 2) + i, (y + 2) + j, 0x000099);
        }
    }

    DrawString(Title, (x + 6), (y + 4), 0xFFFFFF);
}


void Exception_Handle()
{
    // create the main window
    for (int i = 0; i < WIDTH; i++) 
    {
        for (int j = 0; j < 20; j++) 
        {
            SetPixel(i, (HEIGHT - 18) + j, 0xFFFFFF);
        }
    }
    DrawString("cock", 1, (HEIGHT - 16), RGB(255, 0, 0));
}