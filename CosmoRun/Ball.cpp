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
