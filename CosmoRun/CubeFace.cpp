#include "CubeFace.h"
#include "Cube.h"

CubeFace::CubeFace(FaceType type, Direction d, float side_length)
	: type_(type)
	, direction_(d)
	, side_length_(side_length)
{
	CreateVertices();
}

FaceType CubeFace::GetType() const
{
	return type_;
}

Direction CubeFace::GetDirection() const
{
	return direction_;
}

void CubeFace::SetColor(ColorEnum color)
{
	// 淡入淡出式切换颜色
	auto action = Tween::Group({
		Tween::FadeOut(150_msec).DoneCallback([=](Actor*) { this->ResetBrush(color); }),
		Tween::FadeIn(150_msec)
	});

	this->StopAllActions();
	this->AddAction(action);
}

bool CubeFace::IsCollidedWith(const CubePos& pos, CubeDesc desc)
{
	auto self_pos = GetCube()->GetPos();
	int x1 = self_pos[0] - self_pos[2];
	int y1 = self_pos[1] - self_pos[2];
	int x2 = pos[0] - pos[2];
	int y2 = pos[1] - pos[2];
	int offset_x = (x2 - x1);
	int offset_y = (y2 - y1);
	if (offset_x == 0 && offset_y == 0)
		return true;

	if (offset_x <= 1 && offset_y <= 1)
	{
		return true;
	}
	return false;
}

void CubeFace::RemoveSelf()
{
	// 动画结束后移除自身
	auto action = Tween::FadeOut(2_sec).DoneCallback(Closure(GetCube(), &Cube::RemoveFace));
	AddAction(action);
}

CubeFace* CubeFace::GetNext() const
{
	return next_;
}

void CubeFace::SetNext(CubeFace* face)
{
	next_ = face;
}

Cube* CubeFace::GetCube() const
{
	return cube_;
}

void CubeFace::SetCube(Cube* cube)
{
	cube_ = cube;
}

void CubeFace::CreateVertices()
{
	// 根据面类型生成顶点
	float width = side_length_ * math::Cos(30.0f);
	float height = side_length_ * math::Sin(30.0f);

	switch (type_)
	{
	case FaceType::Top:
		this->SetVertices({
			Point(width, 0),
			Point(0, height),
			Point(width, 2 * height),
			Point(2 * width, height)
		});
		break;
	case FaceType::Left:
		this->SetVertices({
			Point(0, 0),
			Point(0, side_length_),
			Point(width, height + side_length_),
			Point(width, height)
		});
		break;
	case FaceType::Right:
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

	// 查找缓存中是否有画刷
	if (BrushPtr brush = ResourceCache::GetInstance().Get<Brush>(id))
	{
		return brush;
	}

	BrushPtr brush;
	if (type_ == FaceType::Left)
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
	else if (type_ == FaceType::Right)
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

	// 查找缓存中是否有画刷
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
	if (type_ == FaceType::Right)
	{
		// 右侧的面颜色较深
		return Brush::Create(dark);
	}
	return Brush::Create(light);
}
