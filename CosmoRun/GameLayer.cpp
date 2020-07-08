#include "GameLayer.h"

#define MAX_CUBE_NUMBER 12
#define PREPARE_CUBE_NUMBER 2

GameLayer::GameLayer(ColorEnum color, Size size)
{
	color_ = color;
	SetSize(size);

	BuildCubes(CUBE_QUEUE_LENGTH);
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		for (auto cube : cubes_)
		{
			cube->SetColor(color);
		}
	}
}

void GameLayer::StartGame()
{
	BuildCubes(CUBE_QUEUE_LENGTH);
}

void GameLayer::BuildCubes(int length)
{
	cubes_.clear();
	this->RemoveAllChildren();

	// 创建第一个方块
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	this->AddCube(0, 0, 0, CubeFace::Type::Top, choices[choice]);

	// 创建几个相同类型的方块，让玩家在刚开始游戏时适应游戏速度
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
	{
		CreateCube(CubeFace::Type::Top, choices[choice]);
	}

	// 随机生成后面的方块
	for (int i = 0; i < MAX_CUBE_NUMBER - PREPARE_CUBE_NUMBER; i++)
	{
		this->CreateRandomCube();
	}
}

void GameLayer::CreateRandomCube()
{
	// 随机下一个方块的属性
	CubeDesc next = GetRandomCubeDesc();

	// 创建方块
	CreateCube(next.type, next.direction);
}

CubeDesc GameLayer::GetRandomCubeDesc() const
{
	Cube* head = cubes_.back();
	CubeFace* head_face = head->GetFace();

	// 找出下一个方块的可能类型和方向
	std::vector<CubeDesc> choices;
	switch (head_face->GetType())
	{
	case CubeFace::Type::Top:

		switch (head_face->GetDirection())
		{
		case Direction::LeftUp:
			choices.assign({
				{ CubeFace::Type::Top, Direction::LeftUp },
				{ CubeFace::Type::Top, Direction::LeftDown },
				{ CubeFace::Type::Top, Direction::RightUp },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Right, Direction::LeftDown },
				});
			break;
		case Direction::LeftDown:
			choices.assign({
				{ CubeFace::Type::Top, Direction::LeftUp },
				{ CubeFace::Type::Top, Direction::LeftDown },
				{ CubeFace::Type::Top, Direction::RightDown },
				{ CubeFace::Type::Left, Direction::Down },
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::RightDown },
				});
			break;
		case Direction::RightUp:
			choices.assign({
				{ CubeFace::Type::Top, Direction::LeftUp },
				{ CubeFace::Type::Top, Direction::RightUp },
				{ CubeFace::Type::Top, Direction::RightDown },
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::RightDown },
				});
			break;
		case Direction::RightDown:
			choices.assign({
				{ CubeFace::Type::Top, Direction::LeftDown },
				{ CubeFace::Type::Top, Direction::RightUp },
				{ CubeFace::Type::Top, Direction::RightDown },
				{ CubeFace::Type::Right, Direction::Down },
				{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Right, Direction::LeftDown },
				});
			break;
		}
		break;

	case CubeFace::Type::Left:

		switch (head_face->GetDirection())
		{
		case Direction::Up:
			choices.assign({
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::RightDown },
				{ CubeFace::Type::Top, Direction::RightUp },
				{ CubeFace::Type::Top, Direction::RightDown },
				{ CubeFace::Type::Top, Direction::LeftUp },
				});
			break;
		case Direction::Down:
			choices.assign({
				{ CubeFace::Type::Left, Direction::Down },
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::RightDown },
				{ CubeFace::Type::Top, Direction::RightDown },
				{ CubeFace::Type::Top, Direction::LeftUp },
				{ CubeFace::Type::Top, Direction::LeftDown },
				});
			break;
		case Direction::LeftUp:
			choices.assign({
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::Down },
				{ CubeFace::Type::Right, Direction::LeftDown },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::Down },
				});
			break;
		case Direction::RightDown:
			choices.assign({
				{ CubeFace::Type::Left, Direction::RightDown },
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::Down },
				// 应该不存在下面的情况，注释掉
				/*{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::Down },*/
				});
			break;
		}
		break;

	case CubeFace::Type::Right:

		switch (head_face->GetDirection())
		{
		case Direction::Up:
			choices.assign({
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::LeftDown },
				{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Top, Direction::LeftUp },
				{ CubeFace::Type::Top, Direction::LeftDown },
				{ CubeFace::Type::Top, Direction::RightUp },
				});
			break;
		case Direction::Down:
			choices.assign({
				{ CubeFace::Type::Right, Direction::Down },
				{ CubeFace::Type::Right, Direction::LeftDown },
				{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Top, Direction::LeftDown },
				{ CubeFace::Type::Top, Direction::RightDown },
				{ CubeFace::Type::Top, Direction::RightUp },
				});
			break;
		case Direction::RightUp:
			choices.assign({
				{ CubeFace::Type::Left, Direction::RightDown },
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::Down },
				{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::Down },
				});
			break;
		case Direction::LeftDown:
			choices.assign({
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::Down },
				// 应该不存在下面的情况，注释掉
				/*{ CubeFace::Type::Right, Direction::LeftDown },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::Down },*/
				});
			break;
		}
		break;
	}

	// 随机选择一种可能
	int choice = math::Random(0, int(choices.size() - 1));
	return choices[choice];
}

Cube* GameLayer::CreateCube(CubeFace::Type type, Direction d)
{
	Cube* head = cubes_.back();
	CubeFace* head_face = head->GetFace();

	// 计算相对位置
	std::array<int, 3> offset = { 0 };
	switch (head_face->GetType())
	{
	case CubeFace::Type::Top:

		switch (head_face->GetDirection())
		{
		case Direction::LeftUp:
			if (type == CubeFace::Type::Top)
				offset = { -1, 0, 0 };
			else
				offset = { -1, 0, 1 };
			break;
		case Direction::LeftDown:
			if (type == CubeFace::Type::Top)
				offset = { 0, 1, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		case Direction::RightUp:
			if (type == CubeFace::Type::Top)
				offset = { 0, -1, 0 };
			else
				offset = { 0, -1, 1 };
			break;
		case Direction::RightDown:
			if (type == CubeFace::Type::Top)
				offset = { 1, 0, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		}
		break;

	case CubeFace::Type::Left:

		switch (head_face->GetDirection())
		{
		case Direction::Up:
			if (type == CubeFace::Type::Top)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 0, 1 };
			break;
		case Direction::Down:
			if (type == CubeFace::Type::Top)
				offset = { 0, 1, -1 };
			else
				offset = { 0, 0, -1 };
			break;
		case Direction::LeftUp:
			if (type == CubeFace::Type::Left)
				offset = { -1, 0, 0 };
			else
				offset = { -1, 1, 0 };
			break;
		case Direction::RightDown:
			if (type == CubeFace::Type::Left)
				offset = { 1, 0, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		}
		break;

	case CubeFace::Type::Right:

		switch (head_face->GetDirection())
		{
		case Direction::Up:
			if (type == CubeFace::Type::Top)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 0, 1 };
			break;
		case Direction::Down:
			if (type == CubeFace::Type::Top)
				offset = { 1, 0, -1 };
			else
				offset = { 0, 0, -1 };
			break;
		case Direction::RightUp:
			if (type == CubeFace::Type::Left)
				offset = { 1, -1, 0 };
			else
				offset = { 0, -1, 0 };
			break;
		case Direction::LeftDown:
			if (type == CubeFace::Type::Left)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 1, 0 };
			break;
		}
		break;
	}

	// 计算新方块的坐标
	auto pos = head->GetPos();
	for (int i = 0; i < 3; i++)
	{
		pos[i] += offset[i];
	}

	// 创建新方块
	auto cube = this->AddCube(pos[0], pos[1], pos[2], type, d);
	head->SetNext(cube);
	return cube;
}

Cube* GameLayer::AddCube(int x, int y, int z, CubeFace::Type type, Direction d)
{
	float side_length = GetWidth() * 0.08f;

	CubePtr cube = new Cube(x, y, z, side_length);
	cube->SetFace(type, d);

	cubes_.push_back(cube.Get());
	this->AddChild(cube);

	return cube.Get();
}
