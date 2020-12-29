#include <Include/PSF2Font.h>
#include <Include/Xeneva.h>
#include <Include/Stdlib/Stdint.h>
#include <Include/File.h>
#include <Include/Memory.h>
#include <Include/Console.h>

static uint16_t *unicode;
static uint8_t *data;

/**
 *PSF Font with bugs
 */
void PSFInit (CHAR16* filename)
{
	uint16_t glyph=0;

	data = OpenFile (filename);
}

/**
 * Put a character on the Screen using PSF Font
 *
 * @param c - Character
 * @param x - X value on the Screen
 * @param y - Y value on the Screen
 * @param bg - Background Color
 * @param fg - Foreground Color
 */
void PSFPutC (char c, int x, int y, uint32_t bg, uint32_t fg)
{
	PSFFONT *font = (PSFFONT*)data;

	int Y = font->Height;
	int X = font->Width;
	unsigned GlyphBytes = Y * ((X + 7) / 8);
	uint8_t* const Data = static_cast<uint8_t* const>(data + font->HeaderSize);

	unsigned BWidth = (X + 7) / 8;
	uint8_t const *FontData = Data + (c * GlyphBytes);

	for (int dy = 0; dy < Y; ++dy)
	{
		for (int dx = 0; dx < BWidth; ++dx)
		{
			uint8_t byte = FontData[dy * BWidth + dx];
			for (int i =0; i < 8; ++i)
			{
				if (dx*8 + i >= X) continue;
				const uint8_t mask = 1 << (7-i);
				uint32_t u = (byte & mask) ? fg : bg;
				DrawPixel (x + dx * 8 + i, y + dy, u);
			}
		}
	}
}