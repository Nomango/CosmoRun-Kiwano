#pragma once
#include "Interpolator.h"

class BrushAnimation : public CustomAnimation
{
public:
	BrushAnimation(Duration dur, BrushPtr brush, Color start, Color end)
		: CustomAnimation(dur)
		, brush_(brush)
		, start_(start)
		, end_(end)
	{
	}

	void OnAnimationUpdate(Actor*, float frac) override
	{
		brush_->SetColor(interpolator_.Interpolate(start_, end_, frac));
	}

private:
	Interpolator interpolator_;
	BrushPtr brush_;
	Color start_;
	Color end_;
};

class BrushStyleAnimation : public CustomAnimation
{
public:
	BrushStyleAnimation(Duration dur, BrushPtr brush, LinearGradientStyle start, LinearGradientStyle end)
		: CustomAnimation(dur)
		, brush_(brush)
		, start_(start)
		, end_(end)
	{
	}

	void OnAnimationUpdate(Actor*, float frac) override
	{
		brush_->SetStyle(interpolator_.Interpolate(start_, end_, frac));
	}

private:
	Interpolator interpolator_;
	BrushPtr brush_;
	LinearGradientStyle start_;
	LinearGradientStyle end_;
};
