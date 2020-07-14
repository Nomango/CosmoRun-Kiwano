#include "Ball.h"

Ball::Ball(float radius)
	: radius_(radius)
{
	CircleActorPtr ball1 = CircleActor::Create(radius);
	CircleActorPtr ball2 = CircleActor::Create(radius * 0.7f);
	ball1->SetFillColor(Color::Rgba(Color::White, 0.7f));
	ball2->SetFillColor(Color::Rgba(Color::White, 0.9f));
	ball1->SetAnchor(0.5f, 0.5f);
	ball2->SetAnchor(0.5f, 0.5f);
	this->AddChild(ball1);
	this->AddChild(ball2);

	particles_ = Actor::Create();
	this->AddChild(particles_);

	// 生成动态小方块
	TaskPtr task = Task::Create(Closure(this, &Ball::SpawnParticles), 50_msec);
	AddTask(task);
}

float Ball::GetRadius() const
{
	return radius_;
}

CubeFace* Ball::GetOn() const
{
	return where_;
}

void Ball::SetOn(CubeFace* face)
{
	where_ = face;
}

void Ball::ToNext()
{
	where_ = where_->GetNext();
}

void Ball::Move(Vec2 trans)
{
	particles_->Move(trans);
}

void Ball::SpawnParticles(Task* task, Duration dt)
{
	if (!particle_brush_)
	{
		particle_brush_ = Brush::Create(Color::Rgba(Color::White, 0.75f));
	}

	AddParticle(1500_msec, radius_ * 1.2f);
	AddParticle(500_msec, radius_ * 0.8f);
}

void Ball::AddParticle(Duration t, float pos_range)
{
	float side = math::Random(radius_ * 0.3f, radius_ * 0.7f);

	RectActorPtr rect = RectActor::Create(Size(side, side));
	rect->SetFillBrush(particle_brush_);
	rect->SetPositionX(math::Random(-pos_range, pos_range));
	rect->SetPositionY(math::Random(-pos_range, pos_range));
	rect->Move(-particles_->GetPosition());
	rect->SetAnchor(0.5f, 0.5f);
	rect->AddAction(Tween::FadeOut(t).RemoveTargetWhenDone());
	particles_->AddChild(rect);
}
