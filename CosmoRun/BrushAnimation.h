#pragma once
#include "Interpolator.h"

class BrushAnimation : public CustomAnimation
{
public:
	BrushAnimation(Duration dur, RefPtr<Brush> brush, Color start, Color end)
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
	RefPtr<Brush> brush_;
	Color start_;
	Color end_;
};

class BrushStyleAnimation : public CustomAnimation
{
public:
	BrushStyleAnimation(Duration dur, RefPtr<Brush> brush, LinearGradientStyle start, LinearGradientStyle end)
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
	RefPtr<Brush> brush_;
	LinearGradientStyle start_;
	LinearGradientStyle end_;
};
