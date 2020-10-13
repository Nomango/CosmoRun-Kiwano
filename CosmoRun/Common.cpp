#include "Common.h"

namespace
{
	float global_unit_length = 0.0f;
	ColorEnum global_color = ColorEnum::Blue;
}

float Config::Unit()
{
	return global_unit_length;
}

void Config::SetWindowSize(uint32_t width, uint32_t height)
{
	global_unit_length = float(width) * 0.08f;
}

ColorEnum Config::Color()
{
	return global_color;
}

void Config::Color(ColorEnum color)
{
	global_color = color;
}
