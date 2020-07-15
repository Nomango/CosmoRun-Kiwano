#include "Background.h"

KGE_DECLARE_SMART_PTR(Triangle);

class Triangle : public PolygonActor
{
public:
	Triangle(float side_length, BrushPtr brush)
	{
		// ���������ε���������
		float h = side_length * math::Sin(60.f);
		SetVertices({
			Point(),
			Point(side_length, 0),
			Point(side_length * 0.5f, h),
		});

		// ���������ɫ
		SetFillBrush(brush);

		// ��Ӷ�������ת + ���� + ���� + λ��
		Duration dt = 1_msec * math::Random(1000, 2500);
		float angle_start = math::Random(-180.0f, 180.0f);
		float angle_end = math::Random(-480.0f, 480.0f);
		float scale_to = math::Random(1.6f, 2.2f);
		float move_x = math::Random(-2 * side_length, 2 * side_length);
		float move_y = math::Random(-2 * side_length, 2 * side_length);

		this->SetRotation(angle_start);
		this->SetScale(Vec2(0, 0));
		this->SetAnchor(0.5f, 0.5f);

		auto action = Tween::Group({
			Tween::RotateTo(dt, angle_end),
			Tween::ScaleTo(dt, scale_to, scale_to),
			Tween::FadeOut(dt),
			Tween::MoveBy(dt, Vec2(move_x, move_y)),
		}, true).RemoveTargetWhenDone();  // ��������ʱ�Զ��Ƴ�������
		this->AddAction(action);
	}
};

Background::Background(ColorEnum color, Size size)
	: color_(color)
{
	// ����ɫ
	bg_rect_ = RectActor::Create(size);
	AddChild(bg_rect_);

	// ��̬������ͼ��
	dynamic_layer_ = Actor::Create();
	AddChild(dynamic_layer_);

	// ������Ӱ
	bg_shadow_ = RectActor::Create(size);
	AddChild(bg_shadow_);

	// �������ÿ��130ms����һ��������
	TaskPtr task = Task::Create(Closure(this, &Background::SpawnTriangles), 130_msec);
	AddTask(task);

	SetPosition(-size / 2);
	Resize(size);
}

void Background::Resize(Size size)
{
	SetSize(size);

	bg_rect_->SetRectSize(size);
	bg_rect_->SetFillBrush(GetCurrentBrush());

	bg_shadow_->SetRectSize(size);
	bg_shadow_->SetFillBrush(GetShadowBrush());
}

void Background::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		// ���뵭��ʽ�л�����ɫ
		auto action = Tween::Group({
			Tween::FadeOut(150_msec).DoneCallback([=](Actor*) { this->bg_rect_->SetFillBrush(GetCurrentBrush()); }),
			Tween::FadeIn(150_msec)
		});
		bg_rect_->StopAllActions();
		bg_rect_->AddAction(action);
	}
}

void Background::MoveTriangles(Vec2 trans)
{
	// ֻ�ƶ���̬��
	dynamic_layer_->Move(trans);
}

void Background::ResetTriangles()
{
	dynamic_layer_->AddAction(Tween::MoveTo(1_sec, Point(0, 0)));
}

BrushPtr Background::GetCurrentBrush()
{
	switch (color_)
	{
	case ColorEnum::Blue:
		return GetBackgroundBrush(Color::Rgb(8, 39, 110), Color::Rgb(6, 37, 38));
	case ColorEnum::Purple:
		return GetBackgroundBrush(Color::Rgb(118, 40, 78), Color::Rgb(45, 31, 66));
	case ColorEnum::Gold:
		return GetBackgroundBrush(Color::Rgb(7, 35, 82), Color::Rgb(55, 26, 19));
	default:
		break;
	}
	return BrushPtr();
}

BrushPtr Background::GetBackgroundBrush(Color top, Color bottom)
{
	// ������ˢ���ݶȽ���ɫ
	float height = this->GetHeight();
	LinearGradientStyle style = LinearGradientStyle(
		Point{ 0, 0 },
		Point{ 0, height },
		{
			GradientStop(0, top),
			GradientStop(1, bottom),
		}
		);
	BrushPtr brush = Brush::Create(style);
	return brush;
}

BrushPtr Background::GetShadowBrush()
{
	// ������Ӱ��ˢ
	Size size = this->GetSize();
	RadialGradientStyle style = RadialGradientStyle(
		size / 2,
		Vec2{},
		Vec2{ size.x * 0.7f, size.y * 0.6f },
		{
			GradientStop(0.8f, Color::Rgba(Color::Black, 0.0f)),
			GradientStop(1.0f, Color::Rgba(Color::Black, 0.3f)),
		}
	);
	return Brush::Create(style);
}

void Background::SpawnTriangles(Task* task, Duration dt)
{
	// ���������εĻ�ˢ
	if (!triangle_brush_)
	{
		triangle_brush_ = Brush::Create(Color(Color::White, 0.2f));
	}

	TrianglePtr t = new Triangle(GetWidth() * 0.05f, triangle_brush_);
	// ������������ε�λ��
	t->SetPositionX(GetWidth() * math::Random(0.0f, 1.0f));
	t->SetPositionY(GetHeight() * math::Random(0.0f, 1.0f));
	t->Move(-dynamic_layer_->GetPosition());
	dynamic_layer_->AddChild(t);
}
