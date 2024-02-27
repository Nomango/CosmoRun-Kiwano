#include "BrushCreator.h"
#include "BrushAnimation.h"

void BrushCreator::Init()
{
	auto list = {
		Face::Top_LeftDown,
		Face::Top_LeftUp,
		Face::Top_RightDown,
		Face::Top_RightUp,
		Face::Left_Up,
		Face::Left_Down,
		Face::Left_LeftUp,
		Face::Left_RightDown,
		Face::Right_Up,
		Face::Right_Down,
		Face::Right_LeftDown,
		Face::Right_RightUp,
	};
	for (auto& desc : list)
		(void)GetFillBrush(desc);

	for (auto& type : { FaceType::Left, FaceType::Right, FaceType::Top })
		(void)GetStrokeBrush(type);
}

RefPtr<Brush> BrushCreator::GetFillBrush(FaceDesc desc)
{
	String id = strings::Format("fill_brush_%d_%d", int(desc.type), int(desc.direction));

	// 查找缓存中是否有画刷
	if (RefPtr<Brush> brush = brush_cache_.Get<Brush>(id))
		return brush;

	RefPtr<Brush> brush;
	auto color = Config::Color();
	if (desc.type == FaceType::Left)
	{
		brush = new Brush(GetLeftFaceColor(color));
	}
	else if (desc.type == FaceType::Right)
	{
		brush = new Brush(GetRightFaceColor(color));
	}
	else
	{
		brush = new Brush(GetTopFaceStyle(color, desc.direction));
	}
	brush_cache_.AddObject(id, brush);
	return brush;
}

RefPtr<Brush> BrushCreator::GetStrokeBrush(FaceType type)
{
	String id = strings::Format("stroke_brush_%d", int(type));

	// 查找缓存中是否有画刷
	if (RefPtr<Brush> brush = brush_cache_.Get<Brush>(id))
		return brush;

	auto color = Config::Color();
	RefPtr<Brush> brush = new Brush(GetStrokeColor(color, type));
	brush_cache_.AddObject(id, brush);
	return brush;
}

List<RefPtr<Animation>> BrushCreator::CreateColorAnimation(Duration dur, ColorEnum old_color, ColorEnum new_color)
{
	List<RefPtr<Animation>> list;
	for (auto& desc : { Face::Top_LeftDown, Face::Top_LeftUp, Face::Top_RightDown, Face::Top_RightUp })
	{
		auto old_style = GetTopFaceStyle(old_color, desc.direction);
		auto new_style = GetTopFaceStyle(new_color, desc.direction);
		RefPtr<Animation> animation = new BrushStyleAnimation(dur, GetFillBrush(desc), old_style, new_style);
		list.push_back(animation);
	}

	for (auto& desc : { Face::Left_Up, Face::Left_Down, Face::Left_LeftUp, Face::Left_RightDown })
	{
		RefPtr<Animation> animation = new BrushAnimation(dur, GetFillBrush(desc), GetLeftFaceColor(old_color), GetLeftFaceColor(new_color));
		list.push_back(animation);
	}

	for (auto& desc : { Face::Right_Up, Face::Right_Down, Face::Right_LeftDown, Face::Right_RightUp })
	{
		RefPtr<Animation> animation = new BrushAnimation(dur, GetFillBrush(desc), GetRightFaceColor(old_color), GetRightFaceColor(new_color));
		list.push_back(animation);
	}

	for (auto& type : { FaceType::Left, FaceType::Right, FaceType::Top })
	{
		RefPtr<Animation> animation = new BrushAnimation(dur, GetStrokeBrush(type), GetStrokeColor(old_color, type), GetStrokeColor(new_color, type));
		list.push_back(animation);
	}
	return list;
}

LinearGradientStyle BrushCreator::GetTopFaceStyle(ColorEnum color, Direction d)
{
	auto GetTopFillBrushStyle = [=](Color light, Color dark)
	{
		float unit = Config::Unit();
		float width = unit * math::Cos(30.0f);
		float height = unit * math::Sin(30.0f);

		LinearGradientStyle style;
		style.stops = { GradientStop(0, light), GradientStop(1, dark) };

		switch (d)
		{
		case Direction::LeftUp:
			style.begin = Point{ width / 2, height / 2 };
			style.end = Point{ width * 3 / 2, height * 3 / 2 };
			break;
		case Direction::LeftDown:
			style.begin = Point{ width / 2, height * 3 / 2 };
			style.end = Point{ width * 3 / 2, height / 2 };
			break;
		case Direction::RightUp:
			style.begin = Point{ width * 3 / 2, height / 2 };
			style.end = Point{ width / 2, height * 3 / 2 };
			break;
		case Direction::RightDown:
			style.begin = Point{ width * 3 / 2, height * 3 / 2 };
			style.end = Point{ width / 2, height / 2 };
			break;
		}
		return style;
	};
	switch (color)
	{
	case ColorEnum::Blue:
		return GetTopFillBrushStyle(Color::Rgb(0, 241, 128), Color::Rgb(0, 210, 100));
	case ColorEnum::Purple:
		return GetTopFillBrushStyle(Color::Rgb(0, 190, 254), Color::Rgb(0, 160, 235));
	case ColorEnum::Gold:
		return GetTopFillBrushStyle(Color::Rgb(253, 161, 70), Color::Rgb(220, 133, 50));
	}
	return {};
}

Color BrushCreator::GetLeftFaceColor(ColorEnum color)
{
	switch (color)
	{
	case ColorEnum::Blue:
		return Color::Rgb(0, 160, 83);
	case ColorEnum::Purple:
		return Color::Rgb(56, 134, 195);
	case ColorEnum::Gold:
		return Color::Rgb(186, 109, 46);
	}
	return {};
}

Color BrushCreator::GetRightFaceColor(ColorEnum color)
{
	switch (color)
	{
	case ColorEnum::Blue:
		return Color::Rgb(0, 121, 62);
	case ColorEnum::Purple:
		return Color::Rgb(0, 91, 144);
	case ColorEnum::Gold:
		return Color::Rgb(151, 88, 37);
	}
	return {};
}

Color BrushCreator::GetStrokeColor(ColorEnum color, FaceType type)
{
	auto GetStrokeBrushColor = [=](Color light, Color dark)
	{
		if (type == FaceType::Right)
		{
			// 右侧的面颜色较深
			return dark;
		}
		return light;
	};
	switch (color)
	{
	case ColorEnum::Blue:
		return GetStrokeBrushColor(Color::Rgb(0, 140, 70), Color::Rgb(0, 91, 46));
	case ColorEnum::Purple:
		return GetStrokeBrushColor(Color::Rgb(0, 114, 181), Color::Rgb(0, 69, 109));
	case ColorEnum::Gold:
		return GetStrokeBrushColor(Color::Rgb(173, 96, 33), Color::Rgb(111, 65, 27));
	}
	return {};
}
