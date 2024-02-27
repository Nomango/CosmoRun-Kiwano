#pragma once
#include "Common.h"
#include "CubeFace.h"

class Ball : public Actor, public SizeSensor
{
public:
	Ball();

	float GetRadius() const;

	Vec2 GetVelocityAngle() const;

	Direction GetDirection() const;

	CubeFace* GetOn() const;

	bool IsSafe() const;

	void SetRadius(float radius);

	void SetOn(CubeFace* face);

	void SetDirection(Direction d);

	void ToNext();

	void Turn();

	void Die();

	void Reborn();

	void MoveParticles(Vec2 trans);

	void ResetParticles();

	void OnUnitChanged(float unit) override;

private:
	bool IsDirectionSame(FaceDesc curr, FaceDesc next) const;

	Direction GetNextDirection(FaceDesc curr, FaceDesc next) const;

	void RandomParticleType();

	void SpawnParticles(Task* task, Duration dt);

	void AddParticle(RefPtr<Actor> particle);

private:
	float radius_;
	float v_angle_;
	Direction direction_;
	CubeFace* where_ = nullptr;
	RefPtr<Actor> body_;

	// Á£×ÓÏà¹Ø
	int particle_type_;
	RefPtr<Actor> particles_;
	RefPtr<Brush> particle_brush_;
};
