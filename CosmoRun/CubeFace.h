#pragma once
#include "BrushCreator.h"

class Cube;

class CubeFace : public PolygonActor
{
public:
	CubeFace(Cube* cube, FaceType type, Direction d, BrushCreator* brush_creator);

	~CubeFace();

	RefPtr<Actor> GetShadow();

	FaceType GetType() const;

	Direction GetDirection() const;

	FaceDesc GetDesc() const;

	bool IsIn(const std::initializer_list<FaceType>& list);

	void Show();

	void Hide(RefPtr<AnimationEventHandler> handler);

	CubeFace* GetNext() const;

	void SetNext(CubeFace* face);

	Cube* GetCube() const;

	void Recreate(float unit);

private:
	void CreateVertices();

	void CreateShadow();

private:
	FaceDesc desc_;
	CubeFace* next_ = nullptr;
	Cube* cube_ = nullptr;
	BrushCreator* brush_creator_ = nullptr;
	RefPtr<PolygonActor> shadow_;
};
