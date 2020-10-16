#include "Ball.h"

Ball::Ball()
	: radius_(0)
	, direction_(Direction::Up)
	, particle_type_(0)
{
	body_ = new Actor();
	this->AddChild(body_, 1);

	particles_ = new Actor();
	this->AddChild(particles_);

	particle_brush_ = new Brush(Color::White);

	// ���ɶ�̬С��������
	TaskPtr task = new Task(Closure(this, &Ball::SpawnParticles), 100_msec);
	AddTask(task);

	RandomParticleType();

	OnUnitChanged(Config::Unit());
}

float Ball::GetRadius() const
{
	return radius_;
}

Vec2 Ball::GetVelocityAngle() const
{
	float angle = 90.0f - float(direction_) * 60.0f;
	return Vec2(-math::Cos(angle), math::Sin(angle));
}

Direction Ball::GetDirection() const
{
	return direction_;
}

CubeFace* Ball::GetOn() const
{
	return where_;
}

bool Ball::IsSafe() const
{
	auto center = GetStage()->GetSize() / 2;
	return where_->ContainsPoint(center);
}

void Ball::SetRadius(float radius)
{
	radius_ = radius;

	CircleActorPtr ball1 = new CircleActor(radius);
	ball1->SetAnchor(0.5f, 0.5f);

	RadialGradientStyle style;
	style.center = Point(radius, radius);
	style.radius = style.center;
	style.stops = { { 0.6f, Color::SkyBlue }, { 1.0f, Color(Color::SkyBlue, 0.0f) } };
	BrushPtr ball1_brush = new Brush(style);
	ball1->SetFillBrush(ball1_brush);

	CircleActorPtr ball2 = new CircleActor(radius * 0.7f);
	ball2->SetFillColor(Color::Rgba(Color::White, 1.0f));
	ball2->SetAnchor(0.5f, 0.5f);

	body_->RemoveAllChildren();
	body_->AddChild(ball1);
	body_->AddChild(ball2);
	this->SetSize(Size(radius * 2, radius * 2));
}

void Ball::SetOn(CubeFace* face)
{
	where_ = face;
}

void Ball::SetDirection(Direction d)
{
	direction_ = d;
}

void Ball::ToNext()
{
	auto next = where_->GetNext();
	// ������һ����ʱ���Զ���������
	if (next->GetType() != where_->GetType())
		direction_ = next->GetDirection();
	where_ = next;
}

void Ball::Turn()
{
	if (direction_ != where_->GetDirection())
		direction_ = where_->GetDirection();
	else
	{
		CubeFace* next = where_->GetNext();
		FaceDesc curr_desc = where_->GetDesc();
		while (next)
		{
			if (next->GetType() == curr_desc.type)
			{
				// ���������ͬ������ȴ��ͬ��ֱ��ת���µķ���
				if (next->GetDirection() != curr_desc.direction)
				{
					direction_ = next->GetDirection();
					break;
				}
			}
			else
			{
				// ������Ͳ�ͬ����Ҫ������İ취�жϷ����Ƿ���ͬ
				if (!IsDirectionSame(curr_desc, next->GetDesc()))
				{
					direction_ = GetNextDirection(curr_desc, next->GetDesc());
					break;
				}
			}
			curr_desc = next->GetDesc();
			next = next->GetNext();
		}
	}
}

void Ball::Die()
{
	body_->StartAnimation(animation::Group({
		animation::ScaleTo(400_msec, Vec2(1.4f, 1.4f)),
		animation::ScaleTo(400_msec, Vec2(0, 0)),
		}));

	StopAllTasks();
}

void Ball::Reborn()
{
	body_->SetScale(0, 0);
	body_->StartAnimation(animation::Group({
		animation::ScaleTo(400_msec, Vec2(1.3f, 1.3f)),
		animation::ScaleTo(400_msec, Vec2(1.0f, 1.0f)),
		}));

	StartAllTasks();

	RandomParticleType();
}

void Ball::MoveParticles(Vec2 trans)
{
	particles_->MoveBy(trans);
}

void Ball::ResetParticles()
{
	particles_->MoveTo(Point(0, 0));
	Reborn();
}

void Ball::OnUnitChanged(float unit)
{
	SetRadius(unit * 0.2f);
}

bool Ball::IsDirectionSame(FaceDesc curr, FaceDesc next) const
{
	switch (curr.type)
	{
	case FaceType::Top:
		switch (curr.direction)
		{
		case Direction::RightUp:
			return next == Face::Left_Up;
		case Direction::RightDown:
			return next == Face::Right_Down;
		case Direction::LeftDown:
			return next == Face::Left_Down;
		case Direction::LeftUp:
			return next == Face::Right_Up;
		}
		break;

	case FaceType::Left:
		switch (curr.direction)
		{
		case Direction::Up:
			return next == Face::Top_RightUp;
		case Direction::RightDown:
			return next == Face::Right_RightUp;
		case Direction::Down:
			return next == Face::Top_LeftDown;
		case Direction::LeftUp:
			return next == Face::Right_LeftDown;
		}
		break;

	case FaceType::Right:
		switch (curr.direction)
		{
		case Direction::Up:
			return next == Face::Top_LeftUp;
		case Direction::RightUp:
			return next == Face::Left_RightDown;
		case Direction::Down:
			return next == Face::Top_RightDown;
		case Direction::LeftDown:
			return next == Face::Left_LeftUp;
		}
		break;
	}
	return false;
}

Direction Ball::GetNextDirection(FaceDesc curr, FaceDesc next) const
{
	switch (curr.type)
	{
	case FaceType::Top:
		switch (curr.direction)
		{
		case Direction::RightUp:
		case Direction::LeftDown:
			if (next == Face::Right_Up)
				return Direction::LeftUp;
			else if (next == Face::Right_Down)
				return Direction::RightDown;
			break;
		case Direction::LeftUp:
		case Direction::RightDown:
			if (next == Face::Left_Up)
				return Direction::RightUp;
			else if (next == Face::Left_Down)
				return Direction::LeftDown;
			break;
		}
		break;

	case FaceType::Left:
		switch (curr.direction)
		{
		case Direction::Up:
		case Direction::Down:
			if (next == Face::Right_RightUp)
				return Direction::RightDown;
			else if (next == Face::Right_LeftDown)
				return Direction::LeftUp;
			break;
		case Direction::RightDown:
		case Direction::LeftUp:
			if (next == Face::Top_RightUp)
				return Direction::Up;
			else if (next == Face::Top_LeftDown)
				return Direction::Down;
			break;
		}
		break;

	case FaceType::Right:
		switch (curr.direction)
		{
		case Direction::Up:
		case Direction::Down:
			if (next == Face::Left_LeftUp)
				return Direction::LeftDown;
			else if (next == Face::Left_RightDown)
				return Direction::RightUp;
			break;
		case Direction::RightUp:
		case Direction::LeftDown:
			if (next == Face::Top_LeftUp)
				return Direction::Up;
			else if (next == Face::Top_RightDown)
				return Direction::Down;
			break;
		}
		break;
	}
	// ִ�е��˴�˵���㷨������
	KGE_ERROR("curr:", curr, ", next:", next);
	throw Exception("Internal algorithm error #2");
	return Direction{};
}

void Ball::RandomParticleType()
{
	particle_type_ = math::Random(1, 2);
}

void Ball::SpawnParticles(Task* task, Duration dt)
{
	ActorPtr particle;
	switch (particle_type_)
	{
	case 1:
	{
		float side = math::Random(radius_ * 0.4f, radius_ * 0.7f);
		CircleActorPtr circle = new CircleActor(side);
		circle->SetAnchor(0.5f, 0.5f);
		circle->SetFillBrush(particle_brush_);
		particle = circle;
		break;
	}
	case 2:
	{
		float side = math::Random(radius_ * 0.7f, radius_ * 1.3f);
		RectActorPtr rect = new RectActor(Size(side, side));
		rect->SetAnchor(0.5f, 0.5f);
		rect->SetFillBrush(particle_brush_);
		particle = rect;
		break;
	}
	}
	AddParticle(particle);
}

void Ball::AddParticle(ActorPtr particle)
{
	// ���Ӷ���
	float pos_radius = math::Random(0.0f, radius_ * 0.5f);
	float pos_angle = math::Random(0.0f, 360.0f);
	Duration dur = 1500_msec;

	particle->SetPositionX(pos_radius * math::Cos(pos_angle));
	particle->SetPositionY(pos_radius * math::Sin(pos_angle));
	particle->SetRotation(math::Random(0.0f, 360.f));
	particle->StartAnimation(animation::FadeOut(dur).RemoveTargetWhenDone());
	particle->StartAnimation(animation::RotateBy(dur, math::Random(30.0f, 60.0f) * math::Sign(math::Random(-1.0f, 1.0f))));
	particle->StartAnimation(animation::ScaleTo(dur, Vec2(0.3f, 0.3f)));

	particle->MoveBy(-particles_->GetPosition());
	particles_->AddChild(particle);
}
