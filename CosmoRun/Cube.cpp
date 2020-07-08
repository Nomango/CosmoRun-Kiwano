#include "Cube.h"

Cube::Cube(int x, int y, int z, float side_length)
	: pos_{ x, y, z }
	, face_{}
	, next_{}
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

Cube* Cube::GetNext() const
{
	return next_;
}

void Cube::SetNext(Cube* next)
{
	next_ = next;
}

CubeFace* Cube::GetFace() const
{
	return face_;
}

CubeFace* Cube::SetFace(CubeFace::Type type, Direction d)
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

	this->AddChild(face);

	face_ = face.Get();
	return face_;
}

void Cube::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		face_->SetColor(color);
	}
}
