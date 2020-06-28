#include "Cubes.h"

CubesPtr Cubes::Create(ColorMode mode, float side_length, int depth)
{
	CubesPtr ptr = memory::New<Cubes>();
	if (ptr)
	{
		ptr->Init(mode, side_length, depth);
	}
	return ptr;
}

void Cubes::Init(ColorMode mode, float side_length, int depth)
{
	this->BuildFaces(mode, side_length, depth);
}

void Cubes::SetColor(ColorMode mode)
{
	RangeFaces([=](CubeFace* face)
		{
			face->SetColor(mode);
		});
}

void Cubes::BuildFaces(ColorMode mode, float side_length, int depth)
{
	Function<CubeFace* (CubeFace*, int, int)> recursive_build = [&](CubeFace* parent, int index, int depth) -> CubeFace*
	{
		if (depth <= 0)
			return nullptr;

		CubeFace::Type type = CubeFace::Type::Top;
		if (parent)
		{
			switch (parent->GetType())
			{
			case CubeFace::Type::Top:
			case CubeFace::Type::Left:
			case CubeFace::Type::Right:
			default:
				break;
			}
		}

		CubeFacePtr face = CubeFace::Create(mode, type, side_length);
		this->AddChild(face);

		for (int i = 0; i < 4; i++)
		{
			auto border = recursive_build(face.Get(), i, depth - 1);
			face->SetBorder(i, border);
		}
		return face.Get();
	};

	center_ = recursive_build(nullptr, 0, depth);
}

void Cubes::RangeFaces(Function<void(CubeFace*)> func)
{
	Function<void(CubeFace*)> recursive_func = [&](CubeFace* center)
	{
		if (center)
		{
			func(center);

			for (int i = 0; i < 4; i++)
			{
				recursive_func(center->GetBorder(0));
			}
		}
	};

	recursive_func(center_);
}
