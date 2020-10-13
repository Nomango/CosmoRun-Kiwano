#include "CubeFace.h"
#include "Cube.h"

FaceDesc Face::Top_LeftDown = { FaceType::Top, Direction::LeftDown };
FaceDesc Face::Top_LeftUp = { FaceType::Top, Direction::LeftUp };
FaceDesc Face::Top_RightDown = { FaceType::Top, Direction::RightDown };
FaceDesc Face::Top_RightUp = { FaceType::Top, Direction::RightUp };
FaceDesc Face::Left_Up = { FaceType::Left, Direction::Up };
FaceDesc Face::Left_Down = { FaceType::Left, Direction::Down };
FaceDesc Face::Left_LeftUp = { FaceType::Left, Direction::LeftUp };
FaceDesc Face::Left_RightDown = { FaceType::Left, Direction::RightDown };
FaceDesc Face::Right_Up = { FaceType::Right, Direction::Up };
FaceDesc Face::Right_Down = { FaceType::Right, Direction::Down };
FaceDesc Face::Right_LeftDown = { FaceType::Right, Direction::LeftDown };
FaceDesc Face::Right_RightUp = { FaceType::Right, Direction::RightUp };

std::map<FaceType, String> type_map = {
	{ FaceType::Top, "[top]   " },
	{ FaceType::Left, "[left]  " },
	{ FaceType::Right, "[right] " },
};

std::map<Direction, String> d_map = {
	{ Direction::Up, "up" },
	{ Direction::Down, "down" },
	{ Direction::LeftDown, "left down" },
	{ Direction::LeftUp, "left up" },
	{ Direction::RightDown, "right down" },
	{ Direction::RightUp, "right up" },
};

std::ostream& operator<<(std::ostream& out, const FaceDesc& desc)
{
	out << type_map[desc.type] << d_map[desc.direction];
	return out;
}

CubeFace::CubeFace(FaceType type, Direction d)
	: desc_{ type, d }
{
	CreateVertices();
}

CubeFace::~CubeFace()
{
	if (shadow_)
	{
		shadow_->RemoveFromParent();
	}
}

ActorPtr CubeFace::GetShadow()
{
	return shadow_;
}

FaceType CubeFace::GetType() const
{
	return desc_.type;
}

Direction CubeFace::GetDirection() const
{
	return desc_.direction;
}

FaceDesc CubeFace::GetDesc() const
{
	return desc_;
}

bool CubeFace::IsIn(const std::initializer_list<FaceType>& list)
{
	for (auto type : list)
	{
		if (type == desc_.type)
		{
			return true;
		}
	}
	return false;
}

void CubeFace::SetColor(ColorEnum color)
{
	// 淡入淡出式切换颜色
	auto switch_bg = AnimationEventHandler::HandleDone([=](Animation*, Actor*) { this->ResetBrush(color); });
	auto action = animation::Group({
		animation::FadeOut(150_msec).Handler(switch_bg),
		animation::FadeIn(150_msec)
	});

	this->StopAllAnimations();
	this->StartAnimation(action);
}

void CubeFace::Show()
{
	SetVisible(true);
	SetOpacity(0);
	StartAnimation(animation::FadeIn(500_msec));

	shadow_->SetVisible(true);
	shadow_->SetOpacity(0);
	shadow_->StartAnimation(animation::FadeIn(500_msec));
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
	float unit = Config::Unit();
	// 根据面类型生成顶点
	float width = unit * math::Cos(30.0f);
	float height = unit * math::Sin(30.0f);

	Vector<Point> vertices;
	switch (desc_.type)
	{
	case FaceType::Top:
		vertices = {
			Point(width, 0),
			Point(width * 2, height),
			Point(width, height * 2),
			Point(0, height),
		};
		this->MoveTo(Point(0, -height));
		break;
	case FaceType::Left:
		vertices = {
			Point(0, 0),
			Point(width, height),
			Point(width, height * 3),
			Point(0, height * 2),
		};
		this->MoveTo(Point(-width / 2, height / 2));
		break;
	case FaceType::Right:
		vertices = {
			Point(width, 0),
			Point(width, height * 2),
			Point(0, height * 3),
			Point(0, height),
		};
		this->MoveTo(Point(width / 2, height / 2));
		break;
	}
	this->SetVertices(vertices);
	this->SetAnchor(0.5f, 0.5f);

	// 创建阴影
	PolygonActorPtr shadow = new PolygonActor;
	shadow->SetAnchor(0.5f, 0.5f);
	shadow->SetFillColor(Color::Black);
	shadow->SetVisible(false);

	float shadow_offset = 5;
	switch (desc_.type)
	{
	case FaceType::Top:
		shadow->SetVertices({
			Point(width, 0),
			Point(width * 2, height),
			Point(width * 2, height + shadow_offset),
			Point(width, height * 2 + shadow_offset),
			Point(0, height + shadow_offset),
			Point(0, height),
			});
		break;
	case FaceType::Left:
	{
		float offset_x = shadow_offset * math::Cos(30.0f);
		float offset_y = shadow_offset * math::Sin(30.0f);
		shadow->SetVertices({
			Point(0, 0),
			Point(offset_x, -offset_y),
			Point(width + offset_x, height - offset_y),
			Point(width + offset_x, height * 3 - offset_y),
			Point(width, height * 3),
			Point(0, height * 2),
			});
		break;
	}
	case FaceType::Right:
	{
		float offset_x = shadow_offset * math::Cos(30.0f);
		float offset_y = shadow_offset * math::Sin(30.0f);
		shadow->SetVertices({
			Point(width, 0),
			Point(width, height * 2),
			Point(0, height * 3),
			Point(-offset_x, height * 3 - offset_y),
			Point(-offset_x, height - offset_y),
			Point(width - offset_x,  -offset_y),
			});
		break;
	}
	}
	shadow->SetSize(this->GetSize());
	this->shadow_ = shadow;
}

void CubeFace::ResetBrush(ColorEnum color)
{
	BrushPtr fill = GetFillBrush(color);
	BrushPtr stroke = GetStrokeBrush(color);

	this->SetFillBrush(fill);
	this->SetStrokeBrush(stroke);

	float unit = Config::Unit();
	float stroke_width = unit / 50;
	this->SetStrokeStyle(new StrokeStyle(stroke_width));
}

BrushPtr CubeFace::GetFillBrush(ColorEnum color)
{
	String id = strings::Format("cube_face_fill_brush_%d_%d_%d", int(color), int(desc_.type), int(desc_.direction));

	// 查找缓存中是否有画刷
	if (BrushPtr brush = brush_cache_.Get<Brush>(id))
	{
		return brush;
	}

	BrushPtr brush;
	if (desc_.type == FaceType::Left)
	{
		switch (color)
		{
		case ColorEnum::Blue:
			brush = new Brush(Color::Rgb(0, 160, 83));
			break;
		case ColorEnum::Purple:
			brush = new Brush(Color::Rgb(56, 134, 195));
			break;
		case ColorEnum::Gold:
			brush = new Brush(Color::Rgb(186, 109, 46));
			break;
		}
	}
	else if (desc_.type == FaceType::Right)
	{
		switch (color)
		{
		case ColorEnum::Blue:
			brush = new Brush(Color::Rgb(0, 121, 62));
			break;
		case ColorEnum::Purple:
			brush = new Brush(Color::Rgb(0, 91, 144));
			break;
		case ColorEnum::Gold:
			brush = new Brush(Color::Rgb(151, 88, 37));
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

	brush_cache_.AddObject(id, brush);
	return brush;
}

BrushPtr CubeFace::GetTopFillBrush(Color light, Color dark)
{
	float unit = Config::Unit();
	float width = unit * math::Cos(30.0f);
	float height = unit * math::Sin(30.0f);

	LinearGradientStyle style;
	style.stops = { GradientStop(0, light), GradientStop(1, dark) };

	switch (desc_.direction)
	{
	case Direction::LeftUp:
		style.begin = Point{ width / 2, height / 2 };
		style.end = Point{ width *3 / 2, height * 3 / 2 };
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
	return new Brush(style);
}

BrushPtr CubeFace::GetStrokeBrush(ColorEnum color)
{
	String id = strings::Format("cube_face_stroke_brush_%d_%d", int(color), int(desc_.type));

	// 查找缓存中是否有画刷
	if (BrushPtr brush = brush_cache_.Get<Brush>(id))
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

	brush_cache_.AddObject(id, brush);
	return brush;
}

BrushPtr CubeFace::GetStrokeBrush(Color light, Color dark)
{
	if (desc_.type == FaceType::Right)
	{
		// 右侧的面颜色较深
		return new Brush(dark);
	}
	return new Brush(light);
}
