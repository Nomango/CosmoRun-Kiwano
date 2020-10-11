#include "Background.h"

KGE_DECLARE_SMART_PTR(Triangle);

class Triangle : public PolygonActor
{
public:
	Triangle(float side_length, BrushPtr brush)
	{
		// 设置三角形的三个顶点
		float h = side_length * math::Sin(60.f);
		SetVertices({
			Point(),
			Point(side_length, 0),
			Point(side_length * 0.5f, h),
		});

		// 设置填充颜色
		SetFillBrush(brush);

		// 添加动画：旋转 + 缩放 + 淡出 + 位移
		Duration dt = 1_msec * math::Random(1000, 2500);
		float angle_start = math::Random(-180.0f, 180.0f);
		float angle_end = math::Random(-480.0f, 480.0f);
		float scale_to = math::Random(1.6f, 2.2f);
		float move_x = math::Random(-2 * side_length, 2 * side_length);
		float move_y = math::Random(-2 * side_length, 2 * side_length);

		this->SetRotation(angle_start);
		this->SetScale(Vec2(0, 0));
		this->SetAnchor(0.5f, 0.5f);

		auto action = animation::Group({
			animation::RotateTo(dt, angle_end),
			animation::ScaleTo(dt, Vec2(scale_to, scale_to)),
			animation::FadeOut(dt),
			animation::MoveBy(dt, Vec2(move_x, move_y)),
		}, true).RemoveTargetWhenDone();  // 动画结束时自动移除三角形
		this->StartAnimation(action);
	}
};

Background::Background(ColorEnum color, Size size)
	: color_(color)
{
	// 背景色
	bg_rect_ = new RectActor(size);
	AddChild(bg_rect_);

	// 动态三角形图层
	dynamic_layer_ = new Actor();
	AddChild(dynamic_layer_);

	// 背景阴影
	bg_shadow_ = new RectActor(size);
	AddChild(bg_shadow_);

	// 添加任务：每隔130ms生成一个三角形
	TaskPtr task = new Task(Closure(this, &Background::SpawnTriangles), 130_msec);
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

		// 淡入淡出式切换背景色
		auto switch_bg = AnimationEventHandler::HandleDone([=](Animation*, Actor*) { this->bg_rect_->SetFillBrush(GetCurrentBrush()); });
		auto action = animation::Group({
			animation::FadeOut(150_msec).Handler(switch_bg),
			animation::FadeIn(150_msec)
		});
		bg_rect_->StopAllAnimations();
		bg_rect_->StartAnimation(action);
	}
}

void Background::MoveTriangles(Vec2 trans)
{
	// 只移动动态层
	dynamic_layer_->MoveBy(trans);
}

void Background::ResetTriangles()
{
	for (auto triangles : dynamic_layer_->GetAllChildren())
	{
		triangles->MoveBy(-dynamic_layer_->GetPosition());
	}
	dynamic_layer_->MoveTo(Point(0, 0));
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
	// 背景画刷，梯度渐变色
	float height = this->GetHeight();
	LinearGradientStyle style = LinearGradientStyle(
		Point{ 0, 0 },
		Point{ 0, height },
		{
			GradientStop(0, top),
			GradientStop(1, bottom),
		}
		);
	BrushPtr brush = new Brush(style);
	return brush;
}

BrushPtr Background::GetShadowBrush()
{
	// 背景阴影画刷
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
	return new Brush(style);
}

void Background::SpawnTriangles(Task* task, Duration dt)
{
	// 创建三角形的画刷
	if (!triangle_brush_)
	{
		triangle_brush_ = new Brush(Color(Color::White, 0.2f));
	}

	TrianglePtr t = new Triangle(GetWidth() * 0.05f, triangle_brush_);
	// 随机设置三角形的位置
	t->SetPositionX(GetWidth() * math::Random(0.0f, 1.0f));
	t->SetPositionY(GetHeight() * math::Random(0.0f, 1.0f));
	t->MoveBy(-dynamic_layer_->GetPosition());
	dynamic_layer_->AddChild(t);
}
