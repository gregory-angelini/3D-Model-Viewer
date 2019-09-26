#include "RGBA.h"

RGBA::RGBA()
{
	red = green = blue = alpha = 1.0f;
}
	

RGBA::RGBA(int _red, int _green, int _blue, int _alpha)
{
	SetRGBA8(_red, _green, _blue, _alpha);
}


void RGBA::SetRGBA8(int _red, int _green, int _blue, int _alpha)
{
	red = (float)_red / 255;
	green = (float)_green / 255;
	blue = (float)_blue / 255;
	alpha = (float)_alpha / 255;
}