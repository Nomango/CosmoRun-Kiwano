#include "Ball.h"

Ball::Ball(float radius)
	: radius_(radius)
	, direction_(Direction::Up)
{
	CircleActorPtr ball1 = new CircleActor(radius);
	CircleActorPtr ball2 = new CircleActor(radius * 0.8f);
	ball1->SetFillColor(Color::Rgba(Color::LightBlue, 0.5f));
	ball2->SetFillColor(Color::Rgba(Color::White, 1.0f));
	ball1->SetAnchor(0.5f, 0.5f);
	ball2->SetAnchor(0.5f, 0.5f);

	body_ = new Actor();
	body_->AddChild(ball1);
	body_->AddChild(ball2);

	this->AddChild(body_, 1);
	this->SetSize(Size(radius * 2, radius * 2));

	particles_ = new Actor();
	this->AddChild(particles_);

	particle_brush_ = new Brush(Color::Rgba(Color::White, 0.9f));

	// 生成动态小方块粒子
	TaskPtr task = new Task(Closure(this, &Ball::SpawnParticles), 50_msec);
	AddTask(task);
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
		FaceDesc curr_desc = where_->GetDesc();
		while (next)
		{
			if (next->GetType() == curr_desc.type)
			{
				// 面的类型相同，方向却不同，直接转向新的方向
				if (next->GetDirection() != curr_desc.direction)
				{
					direction_ = next->GetDirection();
					break;
				}
			}
			else
			{
				// 面的类型不同，需要用特殊的办法判断方向是否相同
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
	body_->AddAction(ActionGroup({
		ActionScaleTo(300_msec, 1.3f, 1.3f),
		ActionScaleTo(300_msec, 0, 0),
		}));

	StopAllTasks();
}

void Ball::Reborn()
{
	body_->SetScale(0, 0);
	body_->AddAction(ActionGroup({
		ActionScaleTo(300_msec, 1.3f, 1.3f),
		ActionScaleTo(300_msec, 1.0f, 1.0f),
		}));

	StartAllTasks();
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
	KGE_ERROR("curr:", curr, ", next:", next);
	throw Exception("Internal algorithm error #2");
	return Direction{};
}

void Ball::SpawnParticles(Task* task, Duration dt)
{
	float side = math::Random(radius_ * 0.7f, radius_ * 1.5f);
	float pos_radius = math::Random(0.0f, radius_ * 0.8f);
	float pos_angle = math::Random(0.0f, 360.0f);
	Duration dur = math::Random(1000, 1500);

	RectActorPtr rect = new RectActor(Size(side, side));
	rect->SetFillBrush(particle_brush_);
	rect->SetPositionX(pos_radius * math::Cos(pos_angle));
	rect->SetPositionY(pos_radius * math::Sin(pos_angle));
	rect->MoveBy(-particles_->GetPosition());
	rect->SetAnchor(0.5f, 0.5f);
	rect->AddAction(ActionFadeOut(dur).RemoveTargetWhenDone());
	rect->AddAction(ActionRotateBy(1_sec, math::Random(-120.0f, 120.0f)).Loops(-1));
	particles_->AddChild(rect);
}
