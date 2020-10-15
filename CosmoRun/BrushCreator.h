#pragma once
#include "Common.h"

// 方块面画刷生成器
class BrushCreator
{
public:
	void Init();

	BrushPtr GetFillBrush(FaceDesc desc);

	BrushPtr GetStrokeBrush(FaceType type);

	List<AnimationPtr> CreateColorAnimation(Duration dur, ColorEnum old_color, ColorEnum new_color);

private:
	LinearGradientStyle GetTopFaceStyle(ColorEnum color, Direction d);

	Color GetLeftFaceColor(ColorEnum color);

	Color GetRightFaceColor(ColorEnum color);

	Color GetStrokeColor(ColorEnum color, FaceType type);

private:
	ResourceCache brush_cache_;
};
