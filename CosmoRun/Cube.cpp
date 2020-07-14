#include "Cube.h"

Cube::Cube(const CubePos& pos, float side_length)
	: pos_{ pos }
	, color_{}
	, side_length_(side_length)
{
	// 根据xyz坐标计算方块位置
	float width = side_length * math::Cos(30.0f);
	float height = side_length * math::Sin(30.0f);
	this->SetPositionX(pos[0] * width - pos[1] * width);
	this->SetPositionY(pos[0] * height + pos[1] * height - pos[2] * side_length);
}

const CubePos& Cube::GetPos() const
{
	return pos_;
}

int Cube::GetFacesCount() const
{
	return int(faces_.size());
}

bool Cube::Has(std::initializer_list<FaceType> types) const
{
	for (auto face : faces_)
	{
		if (face->GetDesc().IsIn(types))
		{
			return true;
		}
	}
	return false;
}

CubeFace* Cube::GetFace(FaceType type) const
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

CubeFace* Cube::AddFace(FaceDesc desc)
{
	CubeFacePtr face = new CubeFace(desc.type, desc.direction, side_length_);
	face->SetCube(this);
	face->SetColor(color_);

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

void Cube::RemoveFace(Actor* face)
{
	this->RemoveChild(face);

	// 自动移除自身
	if (this->GetFacesCount() == 0)
		this->RemoveFromParent();
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

bool CubeMap::IsCollidedWith(const CubePos& pos, FaceDesc desc, CubeFace* head)
{
	for (auto& pair : cube_map_)
	{
		auto cube = pair.second;
		if (head->GetCube() == cube && cube->GetFacesCount() == 1)
			continue;

		auto self_pos = cube->GetPos();
		int x1 = self_pos[0] - self_pos[2];
		int y1 = self_pos[1] - self_pos[2];
		int x2 = pos[0] - pos[2];
		int y2 = pos[1] - pos[2];
		int offset_x = (x1 - x2);
		int offset_y = (y1 - y2);

		if (offset_x == 0 && offset_y == 0)
			return true;

		switch (desc.type)
		{
		case FaceType::Top:
			if (offset_x == -1 && offset_y == -1 && cube->Has({ FaceType::Left, FaceType::Right }))
				return true;
			if (offset_x == -1 && offset_y == 0 && cube->Has({ FaceType::Top, FaceType::Right }))
				return true;
			if (offset_x == 0 && offset_y == -1 && cube->Has({ FaceType::Top, FaceType::Left }))
				return true;
			if (offset_x == 1 && offset_y == 0 && cube->Has({ FaceType::Top }))
				return true;
			if (offset_x == 0 && offset_y == 1 && cube->Has({ FaceType::Top }))
				return true;
			if (offset_x == -2 && offset_y == -1 && cube->Has({ FaceType::Right }))
				return true;
			if (offset_x == -1 && offset_y == -2 && cube->Has({ FaceType::Left }))
				return true;
			break;
		case FaceType::Left:
			if (offset_x == -1 && offset_y == 0 && cube->Has({ FaceType::Left, FaceType::Right }))
				return true;
			if (offset_x == 0 && offset_y == 1 && cube->Has({ FaceType::Top, FaceType::Right }))
				return true;
			if (offset_x == 1 && offset_y == 1 && cube->Has({ FaceType::Top, FaceType::Left }))
				return true;
			if (offset_x == 1 && offset_y == 0 && cube->Has({ FaceType::Left }))
				return true;
			if (offset_x == -1 && offset_y == -1 && cube->Has({ FaceType::Left }))
				return true;
			if (offset_x == -1 && offset_y == 1 && cube->Has({ FaceType::Right }))
				return true;
			if (offset_x == 1 && offset_y == 2 && cube->Has({ FaceType::Top }))
				return true;
			break;
		case FaceType::Right:
			if (offset_x == 0 && offset_y == -1 && cube->Has({ FaceType::Left, FaceType::Right }))
				return true;
			if (offset_x == 1 && offset_y == 0 && cube->Has({ FaceType::Top, FaceType::Left }))
				return true;
			if (offset_x == 1 && offset_y == 1 && cube->Has({ FaceType::Top, FaceType::Right }))
				return true;
			if (offset_x == 0 && offset_y == 1 && cube->Has({ FaceType::Right }))
				return true;
			if (offset_x == -1 && offset_y == -1 && cube->Has({ FaceType::Right }))
				return true;
			if (offset_x == 1 && offset_y == -1 && cube->Has({ FaceType::Left }))
				return true;
			if (offset_x == 2 && offset_y == 1 && cube->Has({ FaceType::Top }))
				return true;
			break;
		}
	}
	return false;
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
