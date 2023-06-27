#pragma once
#include "../io/typedefs.h"

#define WIDTH 800
#define HEIGHT 600

#define RGB(r, g, b) ((uint32_t)(((uint32_t)(uint8_t)(b) | (((uint32_t)(uint8_t)(g)) << 8)) | (((uint32_t)(uint8_t)(r)) << 16)))
#define RGB2UINT(rgb) (RGB(rgb.r, rgb.g, rgb.b))
#define UINT2RGB(uint) (static_cast<COLORRGB>(uint))

void SetPixel(int x, int y, int color);
void FillScreen(int color);
void DrawChar(int x, int y, char ch, int color);
void DrawString(const char *string, int x, int y, int color);
void IntToString(int number, OUT char* str);
void DrawInt(int num, int x, int y, int color);
void CreateWindow(int x, int y, int w, int h, const char *Title);
void Exception_Handle();
