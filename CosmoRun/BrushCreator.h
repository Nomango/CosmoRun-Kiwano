#pragma once
#include "Common.h"

// 方块面画刷生成器
class BrushCreator
{
public:
	void Init();

	RefPtr<Brush> GetFillBrush(FaceDesc desc);

	RefPtr<Brush> GetStrokeBrush(FaceType type);

	List<RefPtr<Animation>> CreateColorAnimation(Duration dur, ColorEnum old_color, ColorEnum new_color);

private:
	LinearGradientStyle GetTopFaceStyle(ColorEnum color, Direction d);

	Color GetLeftFaceColor(ColorEnum color);

	Color GetRightFaceColor(ColorEnum color);

	Color GetStrokeColor(ColorEnum color, FaceType type);

private:
	ResourceCache brush_cache_;
};
