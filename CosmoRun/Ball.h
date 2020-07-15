#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Ball);

class Ball : public Actor
{
public:
	Ball(float radius);

	float GetRadius() const;

	float GetVelocityAngle() const;

	Direction GetDirection() const;

	CubeFace* GetOn() const;

	void SetOn(CubeFace* face);

	void SetDirection(Direction d);

	void ToNext();

	void Turn();

	void Move(Vec2 trans);

private:
	void SpawnParticles(Task* task, Duration dt);

	void AddParticle(Duration t, float pos_range_min, float pos_range_max);

private:
	float radius_;
	float v_angle_;
	Direction direction_;
	CubeFace* where_ = nullptr;
	ActorPtr particles_;
	BrushPtr particle_brush_;
};
