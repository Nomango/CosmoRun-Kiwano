#include "Cube.h"

Cube::Cube(const CubePos& pos, float side_length)
	: pos_{ pos }
	, color_{}
	, side_length_(side_length)
{
	// 根据xyz坐标计算方块位置
	float offset_x = side_length * math::Cos(30.0f);
	float offset_y = side_length * math::Sin(30.0f);
	this->SetPositionX(pos[0] * offset_x - pos[1] * offset_x);
	this->SetPositionY(pos[0] * offset_y + pos[1] * offset_y - pos[2] * side_length);
}

const std::array<int, 3>& Cube::GetPos() const
{
	return pos_;
}

int Cube::GetFacesCount() const
{
	return int(faces_.size());
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

CubeFace* Cube::AddFace(CubeDesc desc)
{
	CubeFacePtr face = new CubeFace(desc.type, desc.direction, side_length_);

	face->SetCube(this);
	face->SetColor(color_);
	face->SetAnchor(0.5f, 0.5f);

	// 计算方块面相对于方块位置的偏移
	float offset_x = side_length_ * math::Cos(30.0f);
	switch (desc.type)
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

	faces_.push_back(face.Get());
	return face.Get();
}

void Cube::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		for (auto face : faces_)
		{
			face->SetColor(color);
		}
	}
}

CubePtr CubeMap::CreateCube(const CubePos& pos, float side_length)
{
	CubePtr cube = new Cube(pos, side_length);

	int key = pos[0] + (pos[1] << 8) + (pos[2] << 16);
	cube_map_.insert(std::make_pair(key, cube.Get()));
	return cube;
}

Cube* CubeMap::GetCubeFromMap(const CubePos& pos)
{
	int key = pos[0] + (pos[1] << 8) + (pos[2] << 16);
	auto iter = cube_map_.find(key);
	if (iter != cube_map_.end())
		return iter->second;
	return nullptr;
}

void CubeMap::RemoveCubeInMap(const CubePos& pos)
{
	int key = pos[0] + (pos[1] << 8) + (pos[2] << 16);
	cube_map_.erase(key);
}

void CubeMap::SetColor(ColorEnum color)
{
	for (auto pair : cube_map_)
	{
		pair.second->SetColor(color);
	}
}

void CubeMap::Clear()
{
	cube_map_.clear();
}
