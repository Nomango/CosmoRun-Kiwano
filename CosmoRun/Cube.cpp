#include "Cube.h"

Cube::Cube(int x, int y, int z, float side_length)
	: pos_{ x, y, z }
	, faces_{}
	, color_{}
	, side_length_(side_length)
{
	// 根据xyz坐标计算方块位置
	float offset_x = side_length * math::Cos(30.0f);
	float offset_y = side_length * math::Sin(30.0f);
	this->SetPositionX(x * offset_x - y * offset_x);
	this->SetPositionY(x * offset_y + y * offset_y - z * side_length);
}

const std::array<int, 3>& Cube::GetPos() const
{
	return pos_;
}

CubeFace* Cube::GetFace(CubeFace::Type type) const
{
	for (auto face : faces_)
	{
		if (face->GetType() == type)
		{
			return face;
		}
	}
	return nullptr;
}

void Cube::AddFace(CubeFace::Type type, Direction d)
{
	CubeFacePtr face = new CubeFace(type, d, side_length_);

	face->SetCube(this);
	face->SetColor(color_);
	face->SetAnchor(0.5f, 0.5f);

	// 计算方块面相对于方块位置的偏移
	float offset_x = side_length_ * math::Cos(30.0f);
	switch (type)
	{
	case CubeFace::Type::Top:
		face->SetPosition(0, 0);
		break;
	case CubeFace::Type::Left:
		face->SetPosition(-offset_x / 2, side_length_ * 3 / 4);
		break;
	case CubeFace::Type::Right:
		face->SetPosition(offset_x / 2, side_length_ * 3 / 4);
		break;
	}

	faces_.push_back(face.Get());
	this->AddChild(face);
}

void Cube::SetColor(ColorEnum color)
{
	color_ = color;
	for (auto face : faces_)
	{
		face->SetColor(color_);
	}
}
