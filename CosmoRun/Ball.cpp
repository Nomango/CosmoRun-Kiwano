#include "Ball.h"

Ball::Ball(float radius)
	: radius_(radius)
	, direction_(Direction::Up)
{
	CircleActorPtr ball1 = CircleActor::Create(radius);
	CircleActorPtr ball2 = CircleActor::Create(radius * 0.7f);
	ball1->SetFillColor(Color::Rgba(Color::White, 0.7f));
	ball2->SetFillColor(Color::Rgba(Color::White, 0.9f));
	ball1->SetAnchor(0.5f, 0.5f);
	ball2->SetAnchor(0.5f, 0.5f);
	this->AddChild(ball1, 1);
	this->AddChild(ball2, 1);

	particles_ = Actor::Create();
	this->AddChild(particles_);

	particle_brush_ = Brush::Create(Color::Rgba(Color::White, 0.9f));

	// 生成动态小方块粒子
	TaskPtr task = Task::Create(Closure(this, &Ball::SpawnParticles), 200_msec);
	AddTask(task);
}

float Ball::GetRadius() const
{
	return radius_;
}

float Ball::GetVelocityAngle() const
{
	return 90.0f - float(direction_) * 60.0f;
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
	// 进入下一方块时，自动调整方向
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
		while (next)
		{
			if (next->GetType() == where_->GetType())
			{
				// 面的类型相同，方向却不同，直接转向新的方向
				if (next->GetDirection() != where_->GetDirection())
				{
					direction_ = next->GetDirection();
					break;
				}
			}
			else
			{
				// 面的类型不同，需要用特殊的办法判断方向是否相同
				if (!IsDirectionSame(where_->GetDesc(), next->GetDesc()))
				{
					direction_ = GetNextDirection(where_->GetDesc(), next->GetDesc());
					break;
				}
			}
			next = next->GetNext();
		}
	}
}

void Ball::MoveParticles(Vec2 trans)
{
	particles_->Move(trans);
}

void Ball::ResetParticles()
{
	particles_->AddAction(Tween::MoveTo(1_sec, Point(0, 0)));
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
	// 执行到此处说明算法有问题
	throw Exception("Internal algorithm error #2");
	return Direction{};
}

void Ball::SpawnParticles(Task* task, Duration dt)
{
	// 离小球中心越近，存留的时间越长，这样可以形成粒子束收窄的效果
	AddParticle(2000_msec, 0, radius_ * 0.4f);
	AddParticle(1500_msec, radius_ * 0.4f, radius_ * 0.8f);
	AddParticle(1000_msec, radius_ * 0.8f, radius_ * 1.2f);
}

void Ball::AddParticle(Duration t, float pos_range_min, float pos_range_max)
{
	float side = math::Random(radius_ * 0.3f, radius_ * 0.7f);
	float pos_radius = math::Random(pos_range_min, pos_range_max);
	float pos_angle = math::Random(0.0f, 360.0f);

	RectActorPtr rect = RectActor::Create(Size(side, side));
	rect->SetFillBrush(particle_brush_);
	rect->SetPositionX(pos_radius * math::Cos(pos_angle));
	rect->SetPositionY(pos_radius * math::Sin(pos_angle));
	rect->Move(-particles_->GetPosition());
	rect->SetAnchor(0.5f, 0.5f);
	rect->SetOpacity(0);
	rect->AddAction(Tween::Group({ Tween::FadeIn(300_msec), Tween::FadeOut(t) }).RemoveTargetWhenDone());
	particles_->AddChild(rect);
}
