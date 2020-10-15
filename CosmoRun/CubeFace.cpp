#include "CubeFace.h"
#include "Cube.h"

CubeFace::CubeFace(FaceType type, Direction d, BrushCreator* brush_creator)
	: desc_{ type, d }
	, brush_creator_(brush_creator)
{
	CreateVertices();

	BrushPtr fill = brush_creator_->GetFillBrush(desc_);
	BrushPtr stroke = brush_creator_->GetStrokeBrush(desc_.type);

	this->SetFillBrush(fill);
	this->SetStrokeBrush(stroke);

	float unit = Config::Unit();
	float stroke_width = unit / 50;
	this->SetStrokeStyle(new StrokeStyle(stroke_width));
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
