#include "CubeFace.h"

CubeFace::CubeFace(Type type, Direction d, float side_length)
	: type_(type)
	, direction_(d)
	, side_length_(side_length)
{
	CreateVertices();
}

CubeFace::Type CubeFace::GetType() const
{
	return type_;
}

Direction CubeFace::GetDirection() const
{
	return direction_;
}

void CubeFace::SetColor(ColorEnum color)
{
	// ���뵭��ʽ�л���ɫ
	auto action = Tween::Group({
		Tween::FadeOut(150_msec).DoneCallback([=](Actor*) { this->ResetBrush(color); }),
		Tween::FadeIn(150_msec)
	});

	this->StopAllActions();
	this->AddAction(action);
}

void CubeFace::RemoveSelf()
{
}

Cube* CubeFace::GetCube() const
{
	return parent_;
}

void CubeFace::SetCube(Cube* cube)
{
	parent_ = cube;
}

void CubeFace::CreateVertices()
{
	// �������������ɶ���
	float width = side_length_ * math::Cos(30.0f);
	float height = side_length_ * math::Sin(30.0f);

	switch (type_)
	{
	case CubeFace::Type::Top:
		this->SetVertices({
			Point(width, 0),
			Point(0, height),
			Point(width, 2 * height),
			Point(2 * width, height)
		});
		break;
	case CubeFace::Type::Left:
		this->SetVertices({
			Point(0, 0),
			Point(0, side_length_),
			Point(width, height + side_length_),
			Point(width, height)
		});
		break;
	case CubeFace::Type::Right:
		this->SetVertices({
			Point(width, 0),
			Point(0, height),
			Point(0, height + side_length_),
			Point(width, side_length_)
		});
		break;
	}
}

void CubeFace::ResetBrush(ColorEnum color)
{
	BrushPtr fill = GetFillBrush(color);
	BrushPtr stroke = GetStrokeBrush(color);

	this->SetFillBrush(fill);
	this->SetStrokeBrush(stroke);

	float stroke_width = side_length_ / 50;
	this->SetStrokeStyle(StrokeStyle::Create(stroke_width));
}

BrushPtr CubeFace::GetFillBrush(ColorEnum color)
{
	String id = strings::Format("cube_face_fill_brush_%d_%d_%d", int(color), int(type_), int(direction_));

	// ���һ������Ƿ��л�ˢ
	if (BrushPtr brush = ResourceCache::GetInstance().Get<Brush>(id))
	{
		return brush;
	}

	BrushPtr brush;
	if (type_ == CubeFace::Type::Left)
	{
		switch (color)
		{
		case ColorEnum::Blue:
			brush = Brush::Create(Color::Rgb(0, 160, 83));
			break;
		case ColorEnum::Purple:
			brush = Brush::Create(Color::Rgb(56, 134, 195));
			break;
		case ColorEnum::Gold:
			brush = Brush::Create(Color::Rgb(186, 109, 46));
			break;
		}
	}
	else if (type_ == CubeFace::Type::Right)
	{
		switch (color)
		{
		case ColorEnum::Blue:
			brush = Brush::Create(Color::Rgb(0, 121, 62));
			break;
		case ColorEnum::Purple:
			brush = Brush::Create(Color::Rgb(0, 91, 144));
			break;
		case ColorEnum::Gold:
			brush = Brush::Create(Color::Rgb(151, 88, 37));
			break;
		}
	}
	else
	{
		switch (color)
		{
		case ColorEnum::Blue:
			brush = GetTopFillBrush(Color::Rgb(0, 241, 128), Color::Rgb(0, 210, 100));
			break;
		case ColorEnum::Purple:
			brush = GetTopFillBrush(Color::Rgb(0, 190, 254), Color::Rgb(0, 160, 235));
			break;
		case ColorEnum::Gold:
			brush = GetTopFillBrush(Color::Rgb(253, 161, 70), Color::Rgb(220, 133, 50));
			break;
		}
	}

	ResourceCache::GetInstance().AddObject(id, brush);
	return brush;
}

BrushPtr CubeFace::GetTopFillBrush(Color light, Color dark)
{
	float width = side_length_ * math::Cos(30.0f);
	float height = side_length_ * math::Sin(30.0f);

	Point start, end;
	switch (direction_)
	{
	case Direction::LeftUp:
		start = Point{ width / 2, height / 2 };
		end = Point{ width *3 / 2, height * 3 / 2 };
		break;
	case Direction::LeftDown:
		start = Point{ width / 2, height * 3 / 2 };
		end = Point{ width * 3 / 2, height / 2 };
		break;
	case Direction::RightUp:
		start = Point{ width * 3 / 2, height / 2 };
		end = Point{ width / 2, height * 3 / 2 };
		break;
	case Direction::RightDown:
		start = Point{ width * 3 / 2, height * 3 / 2 };
		end = Point{ width / 2, height / 2 };
		break;
	}

	auto style = LinearGradientStyle(start, end, { GradientStop(0, light), GradientStop(1, dark) });
	return Brush::Create(style);
}

BrushPtr CubeFace::GetStrokeBrush(ColorEnum color)
{
	String id = strings::Format("cube_face_stroke_brush_%d_%d", int(color), int(type_));

	// ���һ������Ƿ��л�ˢ
	if (BrushPtr brush = ResourceCache::GetInstance().Get<Brush>(id))
	{
		return brush;
	}

	BrushPtr brush;
	switch (color)
	{
	case ColorEnum::Blue:
		brush = GetStrokeBrush(Color::Rgb(0, 140, 70), Color::Rgb(0, 91, 46));
		break;
	case ColorEnum::Purple:
		brush = GetStrokeBrush(Color::Rgb(0, 114, 181), Color::Rgb(0, 69, 109));
		break;
	case ColorEnum::Gold:
		brush = GetStrokeBrush(Color::Rgb(173, 96, 33), Color::Rgb(111, 65, 27));
		break;
	}

	ResourceCache::GetInstance().AddObject(id, brush);
	return brush;
}

BrushPtr CubeFace::GetStrokeBrush(Color light, Color dark)
{
	if (type_ == Type::Right)
	{
		// �Ҳ������ɫ����
		return Brush::Create(dark);
	}
	return Brush::Create(light);
}