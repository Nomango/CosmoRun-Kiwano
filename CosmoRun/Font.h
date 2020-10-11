#pragma once
#include "Common.h"

inline FontPtr GillSansFont(float size, bool bold)
{
	uint32_t weight = bold ? FontWeight::Bold : FontWeight::Normal;
	FontPtr font = new Font("Gill Sans MT", size, weight, FontPosture::Normal, FontStretch::Condensed);
	return font;
}
