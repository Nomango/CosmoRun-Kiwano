#pragma once
#include "Common.h"

class Interpolator
{
public:
	inline float Interpolate(float start, float end, float frac)
	{
		if (frac >= 1)
			return end;
		return start + (end - start) * frac;
	}

	inline Color Interpolate(const Color& start, const Color& end, float frac)
	{
		if (frac >= 1)
			return end;
		return Color::Rgba(
			Interpolate(start.r, end.r, frac),
			Interpolate(start.g, end.g, frac),
			Interpolate(start.b, end.b, frac),
			Interpolate(start.a, end.a, frac)
		);
	}

	inline Point Interpolate(const Point& start, const Point& end, float frac)
	{
		if (frac >= 1)
			return end;
		return Point(
			Interpolate(start.x, end.x, frac),
			Interpolate(start.y, end.y, frac)
		);
	}

	inline LinearGradientStyle Interpolate(const LinearGradientStyle& start, const LinearGradientStyle& end, float frac)
	{
		if (frac >= 1)
			return end;

		LinearGradientStyle style;
		style.begin = Interpolate(start.begin, end.begin, frac);
		style.end = Interpolate(start.end, end.end, frac);
		KGE_ASSERT(std::size(start.stops) == 2 && std::size(end.stops) == 2);
		style.stops.resize(2);
		style.stops[0].color = Interpolate(start.stops[0].color, end.stops[0].color, frac);
		style.stops[0].offset = Interpolate(start.stops[0].offset, end.stops[0].offset, frac);
		style.stops[1].color = Interpolate(start.stops[1].color, end.stops[1].color, frac);
		style.stops[1].offset = Interpolate(start.stops[1].offset, end.stops[1].offset, frac);
		return style;
	}
};
