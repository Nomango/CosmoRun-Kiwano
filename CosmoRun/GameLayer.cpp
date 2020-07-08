#include "GameLayer.h"

#define MAX_CUBE_NUMBER 12
#define PREPARE_CUBE_NUMBER 2

GameLayer::GameLayer(ColorEnum color, Size size)
	: color_(color)
{
	SetSize(size);
}

void GameLayer::StartGame()
{
	InitCubes(CUBE_QUEUE_LENGTH);
}

void GameLayer::Restart()
{
	// ������
	InitCubes(CUBE_QUEUE_LENGTH);
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		cube_map_.SetColor(color);
	}
}

void GameLayer::InitCubes(int length)
{
	cube_faces_.clear();
	cube_map_.Clear();
	this->RemoveAllChildren();

	// ������һ������
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	this->AddCubeFace(0, 0, 0, CubeFace::Type::Top, choices[choice]);

	// ����������ͬ���͵ķ��飬������ڸտ�ʼ��Ϸʱ��Ӧ��Ϸ�ٶ�
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
	{
		CreateCubeFace(CubeFace::Type::Top, choices[choice]);
	}

	// ������ɺ���ķ���
	for (int i = 0; i < MAX_CUBE_NUMBER - PREPARE_CUBE_NUMBER; i++)
	{
		this->CreateRandomCube();
	}
}

void GameLayer::CreateRandomCube()
{
	// �����һ�����������
	CubeDesc next = GetRandomCubeDesc();

	// ��������
	CreateCubeFace(next.type, next.direction);
}

CubeDesc GameLayer::GetRandomCubeDesc() const
{
	CubeFace* head = cube_faces_.back();

	// �ж�ͷ�����Ƿ��Ѿ���ռ����������
	bool must_new_cube = (head->GetCube()->GetFacesCount() == 2);

	// �ҳ���һ������Ŀ������ͺͷ���
	std::vector<CubeDesc> choices;
	switch (head->GetType())
	{
	case CubeFace::Type::Top:

		switch (head->GetDirection())
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
				});
			if (!must_new_cube)
			{
				choices.insert(choices.end(), {
					{ CubeFace::Type::Left, Direction::Down },
					{ CubeFace::Type::Left, Direction::LeftUp },
					{ CubeFace::Type::Left, Direction::RightDown },
					});
			}
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
				});
			if (!must_new_cube)
			{
				choices.insert(choices.end(), {
					{ CubeFace::Type::Right, Direction::Down },
					{ CubeFace::Type::Right, Direction::RightUp },
					{ CubeFace::Type::Right, Direction::LeftDown },
					});
			}
			break;
		}
		break;

	case CubeFace::Type::Left:

		switch (head->GetDirection())
		{
		case Direction::Up:
			choices.assign({
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::RightDown },
				});
			if (!must_new_cube)
			{
				choices.insert(choices.end(), {
					{ CubeFace::Type::Top, Direction::RightUp },
					{ CubeFace::Type::Top, Direction::RightDown },
					{ CubeFace::Type::Top, Direction::LeftUp },
					});
			}
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
				});

			// Ӧ�ò���������������ע�͵�
			/*if (!must_new_cube)
			{
				choices.insert(choices.end(), {
					{ CubeFace::Type::Right, Direction::RightUp },
					{ CubeFace::Type::Right, Direction::Up },
					{ CubeFace::Type::Right, Direction::Down },
					});
			}*/
			break;
		}
		break;

	case CubeFace::Type::Right:

		switch (head->GetDirection())
		{
		case Direction::Up:
			choices.assign({
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::LeftDown },
				{ CubeFace::Type::Right, Direction::RightUp },
				});
			if (!must_new_cube)
			{
				choices.insert(choices.end(), {
					{ CubeFace::Type::Top, Direction::LeftUp },
					{ CubeFace::Type::Top, Direction::LeftDown },
					{ CubeFace::Type::Top, Direction::RightUp },
					});
			}
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
				});

			// Ӧ�ò���������������ע�͵�
			/*if (!must_new_cube)
			{
				choices.insert(choices.end(), {
					{ CubeFace::Type::Right, Direction::LeftDown },
					{ CubeFace::Type::Right, Direction::Up },
					{ CubeFace::Type::Right, Direction::Down },
					});
			}*/
			break;
		}
		break;
	}

	// ��Ҫȥ�������Ӿ�����Ӱ������

	// ���ѡ��һ�ֿ���
	int choice = math::Random(0, int(choices.size() - 1));
	return choices[choice];
}

CubeFace* GameLayer::CreateCubeFace(CubeFace::Type type, Direction d)
{
	CubeFace* head = cube_faces_.back();

	// �������λ��
	std::array<int, 3> offset = { 0 };
	switch (head->GetType())
	{
	case CubeFace::Type::Top:

		switch (head->GetDirection())
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

		switch (head->GetDirection())
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

		switch (head->GetDirection())
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

	// �����·��������
	auto pos = head->GetCube()->GetPos();
	for (int i = 0; i < 3; i++)
	{
		pos[i] += offset[i];
	}

	// �����·���
	auto face = this->AddCubeFace(pos[0], pos[1], pos[2], type, d);
	head->SetNext(face);
	return face;
}

CubeFace* GameLayer::AddCubeFace(int x, int y, int z, CubeFace::Type type, Direction d)
{
	CubePtr cube = cube_map_.GetCubeFromMap(x, y, z);
	if (!cube)
	{
		float side_length = GetWidth() * 0.08f;
		cube = cube_map_.CreateCube(x, y, z, side_length);
		this->AddChild(cube);
	}

	auto face = cube->AddFace(type, d);
	cube_faces_.push_back(face);
	return face;
}
