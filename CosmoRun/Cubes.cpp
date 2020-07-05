#include "Cubes.h"

Cubes::Cubes(ColorEnum color, float side_length)
	: color_(color)
	, side_length_(side_length)
{
	this->BuildCubes(CUBE_QUEUE_LENGTH);
}

void Cubes::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		for (auto face : cubes_)
		{
			face->SetColor(color);
		}
	}
}

void Cubes::BuildCubes(int length)
{
	cubes_.clear();
	for (int i = 0; i < length; i++)
	{
		if (i == 0)
		{
			// 构建头节点
			CubeFacePtr face = new CubeFace(color_, CubeFace::Type::Top, side_length_);
			AddCube(face);
		}
		else
		{
			CreateRandomCube();
		}
	}
}

void Cubes::CreateRandomCube()
{
	CubeFace::Type type = GetRandomType();

	CubeFacePtr face = new CubeFace(color_, type, side_length_);
	face->SetParent(Head());
	AddCube(face);
}

CubeFace::Type Cubes::GetRandomType()
{
	CubeFace::Type type = CubeFace::Type::Top;
	return type;
}

void Cubes::AddCube(CubeFacePtr face)
{
	// 新方块添加到list尾部
	this->AddChild(face);
	cubes_.push_back(face.Get());
}

void Cubes::RemoveTail()
{
	// 路径的尾巴其实是list的头
	cubes_.front()->RemoveSelf();
	cubes_.erase(cubes_.begin());
}

CubeFace* Cubes::Head() const
{
	return nullptr;
}
