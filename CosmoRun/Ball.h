#pragma once
#include "Common.h"
#include "CubeFace.h"

KGE_DECLARE_SMART_PTR(Ball);

class Ball : public Actor
{
public:
	Ball(float radius);

	float GetRadius() const;

	CubeFace* GetOn() const;

	void SetOn(CubeFace* face);

	void ToNext();

	void Move(Vec2 trans);

private:
	void SpawnParticles(Task* task, Duration dt);

	void AddParticle(Duration t, float pos_range);

private:
	float radius_;
	CubeFace* where_ = nullptr;
	ActorPtr particles_;
	BrushPtr particle_brush_;
};
