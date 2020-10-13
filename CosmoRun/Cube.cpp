#include "Cube.h"

Cube::Cube(const CubePos& pos)
	: pos_{ pos }
{
	float unit = Config::Unit();
	// 根据xyz坐标计算方块位置
	float width = unit * math::Cos(30.0f);
	float height = unit * math::Sin(30.0f);
	this->SetPositionX(pos[0] * width - pos[1] * width);
	this->SetPositionY(pos[0] * height + pos[1] * height - pos[2] * unit);
}

const CubePos& Cube::GetPos() const
{
	return pos_;
}

int Cube::GetFacesCount() const
{
	return int(faces_.size());
}

std::vector<CubeFace*> Cube::GetFaces() const
{
	return faces_;
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
	ColorEnum color = Config::Color();
	CubeFacePtr face = new CubeFace(desc.type, desc.direction);
	face->SetCube(this);
	face->SetColor(color);

	this->AddChild(face);

	faces_.push_back(face.Get());
	return face.Get();
}

void Cube::SetColor(ColorEnum color)
{
	for (auto face : faces_)
	{
		face->SetColor(color);
	}
}

void Cube::RemoveFace(Actor* face)
{
	for (auto iter = faces_.begin(); iter != faces_.end();)
	{
		if (*iter == face)
		{
			this->RemoveChild(face);
			iter = faces_.erase(iter);
		}
		else
			++iter;
	}
}

CubePtr CubeMap::CreateCube(const CubePos& pos)
{
	CubePtr cube = new Cube(pos);

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

void CubeMap::RemoveCubeInMap(Cube* cube)
{
	auto pos = cube->GetPos();
	int key = pos[0] + (pos[1] << 8) + (pos[2] << 16);
	cube_map_.erase(key);
}

void CubeMap::RemoveCubeFaceInMap(CubeFace* face)
{
	auto cube = face->GetCube();
	cube->RemoveFace(face);

	if (cube->GetFacesCount() == 0)
	{
		RemoveCubeInMap(cube);
		cube->RemoveFromParent();
	}
}

bool CubeMap::IsCollidedWith(const CubePos& pos, FaceDesc desc, CubeFace* head)
{
	for (auto& pair : cube_map_)
	{
		auto cube = pair.second;
		auto self_pos = cube->GetPos();
		int x1 = self_pos[0] - self_pos[2];
		int y1 = self_pos[1] - self_pos[2];
		int x2 = pos[0] - pos[2];
		int y2 = pos[1] - pos[2];
		int offset_x = (x1 - x2);
		int offset_y = (y1 - y2);

		for (auto face : cube->GetFaces())
		{
			if (face == head)
				continue;

			if (offset_x == 0 && offset_y == 0)
				return true;

			switch (desc.type)
			{
			case FaceType::Top:
				if (offset_x == -1 && offset_y == -1 && face->IsIn({ FaceType::Left, FaceType::Right }))
					return true;
				if (offset_x == -2 && offset_y == -2 && face->IsIn({ FaceType::Left, FaceType::Right }))
					return true;
				if (offset_x == -1 && offset_y == 0 && face->IsIn({ FaceType::Top, FaceType::Right }))
					return true;
				if (offset_x == 0 && offset_y == -1 && face->IsIn({ FaceType::Top, FaceType::Left }))
					return true;
				if (offset_x == 1 && offset_y == 0 && face->IsIn({ FaceType::Top }))
					return true;
				if (offset_x == 0 && offset_y == 1 && face->IsIn({ FaceType::Top }))
					return true;
				if (offset_x == -2 && offset_y == -1 && face->IsIn({ FaceType::Right }))
					return true;
				if (offset_x == -1 && offset_y == -2 && face->IsIn({ FaceType::Left }))
					return true;
				break;
			case FaceType::Left:
				if (offset_x == -1 && offset_y == 0 && face->IsIn({ FaceType::Left, FaceType::Right }))
					return true;
				if (offset_x == 0 && offset_y == 1 && face->IsIn({ FaceType::Top, FaceType::Right }))
					return true;
				if (offset_x == 1 && offset_y == 1 && face->IsIn({ FaceType::Top, FaceType::Left }))
					return true;
				if (offset_x == 1 && offset_y == 0 && face->IsIn({ FaceType::Left }))
					return true;
				if (offset_x == -1 && offset_y == -1 && face->IsIn({ FaceType::Left }))
					return true;
				if (offset_x == -1 && offset_y == 1 && face->IsIn({ FaceType::Right }))
					return true;
				if (offset_x == -2 && offset_y == -1 && face->IsIn({ FaceType::Right }))
					return true;
				if (offset_x == 1 && offset_y == 2 && face->IsIn({ FaceType::Right, FaceType::Top }))
					return true;
				if (offset_x == 2 && offset_y == 2 && face->IsIn({ FaceType::Top }))
					return true;
				if (offset_x == 2 && offset_y == 1 && face->IsIn({ FaceType::Top }))
					return true;
				break;
			case FaceType::Right:
				if (offset_x == 0 && offset_y == -1 && face->IsIn({ FaceType::Left, FaceType::Right }))
					return true;
				if (offset_x == 1 && offset_y == 0 && face->IsIn({ FaceType::Top, FaceType::Left }))
					return true;
				if (offset_x == 1 && offset_y == 1 && face->IsIn({ FaceType::Top, FaceType::Right }))
					return true;
				if (offset_x == 0 && offset_y == 1 && face->IsIn({ FaceType::Right }))
					return true;
				if (offset_x == -1 && offset_y == -1 && face->IsIn({ FaceType::Right }))
					return true;
				if (offset_x == 1 && offset_y == -1 && face->IsIn({ FaceType::Left }))
					return true;
				if (offset_x == -1 && offset_y == -2 && face->IsIn({ FaceType::Left }))
					return true;
				if (offset_x == 2 && offset_y == 1 && face->IsIn({ FaceType::Top, FaceType::Left }))
					return true;
				if (offset_x == 2 && offset_y == 2 && face->IsIn({ FaceType::Top }))
					return true;
				if (offset_x == 1 && offset_y == 2 && face->IsIn({ FaceType::Top }))
					return true;
				break;
			}
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
