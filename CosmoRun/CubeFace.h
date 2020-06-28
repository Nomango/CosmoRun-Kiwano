#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(CubeFace);

class CubeFace : public PolygonActor
{
public:
	enum class Type
	{
		Top,
		Left,
		Right,
	};

	static CubeFacePtr Create(ColorMode mode, Type type, float side_length);

	void Init(ColorMode mode, Type type, float side_length);

	Type GetType() const;

	void SetColor(ColorMode mode);

	void SetBorder(int index, CubeFace* face);

	CubeFace* GetBorder(int index) const;

private:
	Type type_;
	CubeFace* borders_[4] = { 0 };
};
