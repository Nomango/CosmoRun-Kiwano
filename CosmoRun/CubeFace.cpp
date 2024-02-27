#include "CubeFace.h"
#include "Cube.h"

CubeFace::CubeFace(Cube* cube, FaceType type, Direction d, BrushCreator* brush_creator)
	: cube_(cube)
	, desc_{ type, d }
	, brush_creator_(brush_creator)
{
	RefPtr<Brush> fill = brush_creator_->GetFillBrush(desc_);
	RefPtr<Brush> stroke = brush_creator_->GetStrokeBrush(desc_.type);

	this->SetFillBrush(fill);
	this->SetStrokeBrush(stroke);

	Recreate(Config::Unit());
}

CubeFace::~CubeFace()
{
	if (shadow_)
	{
		shadow_->RemoveFromParent();
	}
}

RefPtr<Actor> CubeFace::GetShadow()
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
	SetScale(0, 0);
	StartAnimation(animation::ScaleTo(300_msec, Vec2(1, 1)));

	shadow_->SetVisible(true);
	shadow_->SetScale(0, 0);
	shadow_->StartAnimation(animation::ScaleTo(300_msec, Vec2(1, 1)));
}

void CubeFace::Hide(RefPtr<AnimationEventHandler> handler)
{
	auto action = animation::ScaleTo(300_msec, Vec2(0, 0));
	action.Handler(handler);
	StartAnimation(action);

	// 处理阴影
	GetShadow()->StartAnimation(animation::ScaleTo(300_msec, Vec2(0, 0)));
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

void CubeFace::Recreate(float unit)
{
	CreateVertices();
	CreateShadow();

	float stroke_width = unit / 50;
	this->SetStrokeStyle(new StrokeStyle(stroke_width));

	shadow_->SetPosition(this->GetPosition() + cube_->GetPosition());
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
}

void CubeFace::CreateShadow()
{
	float unit = Config::Unit();
	// 根据面类型生成顶点
	float width = unit * math::Cos(30.0f);
	float height = unit * math::Sin(30.0f);

	// 创建阴影
	if (!shadow_)
	{
		shadow_ = new PolygonActor;
		shadow_->SetAnchor(0.5f, 0.5f);
		shadow_->SetFillColor(Color::Black);
		shadow_->SetVisible(false);
	}

	float shadow_offset = 5 * Config::NormalizeUnit();
	switch (desc_.type)
	{
	case FaceType::Top:
	{
		float shadow_height = height * 1.5f;
		shadow_->SetVertices({
			Point(width, 0),
			Point(width * 2, height),
			Point(width * 2, height + shadow_offset),

			Point(width * (2 - 0.3f), height + shadow_offset + shadow_height * 0.3f),
			Point(width * (2 - 0.8f), height + shadow_offset + shadow_height * 0.9f),
			Point(width, height + shadow_offset + shadow_height),
			Point(width * 0.8f, height + shadow_offset + shadow_height * 0.9f),
			Point(width * 0.3f, height + shadow_offset + shadow_height * 0.3f),

			Point(0, height + shadow_offset),
			Point(0, height),
			});
		break;
	}
	case FaceType::Left:
	{
		float offset_x = shadow_offset * math::Cos(30.0f);
		float offset_y = shadow_offset * math::Sin(30.0f);
		shadow_->SetVertices({
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
		shadow_->SetVertices({
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
	shadow_->SetSize(this->GetSize());
}
