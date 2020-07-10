#include "GameLayer.h"

#define MAX_CUBE_NUMBER 12
#define PREPARE_CUBE_NUMBER 2

std::map<CubeFace::Type, String> type_map = {
		{ CubeFace::Type::Top, "top   " },
		{ CubeFace::Type::Left, "left  " },
		{ CubeFace::Type::Right, "right " },
};

std::map<Direction, String> d_map = {
	{ Direction::Up, "up" },
	{ Direction::Down, "down" },
	{ Direction::LeftDown, "left down" },
	{ Direction::LeftUp, "left up" },
	{ Direction::RightDown, "right down" },
	{ Direction::RightUp, "right up" },
};

GameLayer::GameLayer(ColorEnum color, Size size)
	: color_(color)
{
	SetSize(size);
}

void GameLayer::StartGame()
{
	InitCubes(MAX_CUBE_NUMBER);
}

void GameLayer::Restart()
{
	// ������
	InitCubes(MAX_CUBE_NUMBER);
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
	KGE_LOG();
	KGE_LOG();

	cube_faces_.clear();
	cube_map_.Clear();
	this->RemoveAllChildren();

	CircleActorPtr c = CircleActor::Create(3);
	c->SetFillColor(Color::Red);
	this->AddChild(c, 1);

	// ������һ������
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	this->AppendCubeFace({ CubeFace::Type::Top, choices[choice] });

	// ����������ͬ���͵ķ��飬������ڸտ�ʼ��Ϸʱ��Ӧ��Ϸ�ٶ�
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
	{
		AppendCubeFace({ CubeFace::Type::Top, choices[choice] });
	}

	// ������ɺ���ķ���
	for (int i = 0; i < length - PREPARE_CUBE_NUMBER; i++)
	{
		// �����������
		AddRandomFace();
	}

	KGE_LOG();
	KGE_LOG();
}

void GameLayer::AddRandomFace()
{
	try
	{
		// ��ȡ����ķ�������
		auto desc = GetRandomChoice();

		// �����·���
		auto face = AppendCubeFace(desc);

		// �ж��Ƿ��г�ͻ
		/*if (cube_faces_.size() > 3)
		{
			for (size_t i = 0; i < cube_faces_.size() - 2; i++)
			{
				if (cube_faces_[i]->IsCollidedWith(face))
				{
					RemoveHeadFace();
					throw Exception("new face collided");
				}
			}
		}*/
	}
	catch (Exception&)
	{
		// ��ȡ�������ʧ�ܣ���Ҫɾ��֮ǰ�ķ�����������
		if (cube_faces_.size() >= 3 && cube_faces_.size() <= 5)
		{
			RemoveHeadFace();

			AddRandomFace();
			AddRandomFace();
		}
		else
		{
			// ���������϶�ʱ�����ܴ���Ȧ��·������Ҫ��ɾ����
			RemoveHeadFace();
			RemoveHeadFace();

			AddRandomFace();
			AddRandomFace();
			AddRandomFace();
		}
	}
}

void GameLayer::RemoveHeadFace()
{
	KGE_LOG("-remove- ", type_map[cube_faces_.back()->GetType()], d_map[cube_faces_.back()->GetDirection()]);

	cube_faces_.back()->RemoveSelf();
	//auto head = cube_faces_.back();
	//head->GetCube()->RemoveFace(head);
	cube_faces_.pop_back();

	if (cube_faces_.empty())
		KGE_LOG("??");
}

CubeDesc GameLayer::GetRandomChoice()
{
	auto choices = GetRandomChoices();
	if (choices.empty())
	{
		throw Exception("choices is empty");
	}

	// ���ѡ��һ�ֿ���
	int choice = math::Random(0, int(choices.size() - 1));

	// �������Ӿ������ڵķ���
	auto pos = GetNewCubePos(choices[choice]);
	auto head = cube_faces_.back();
	for (auto face : cube_faces_)
	{
		if (face == head)
			continue;

		CubePos pos2 = face->GetCube()->GetPos();
		CubePos offset = { pos[0] - pos2[0], pos[1] - pos2[1], pos[2] - pos2[2] };

		if (face->GetType() == choices[choice].type)
		{
			if (offset == CubePos{ 0, 0, 0 }
				|| offset == CubePos{ 1, 0, 0 }
				|| offset == CubePos{ -1, 0, 0 }
				|| offset == CubePos{ 0, 1, 0 }
				|| offset == CubePos{ 0, -1, 0 }
				|| offset == CubePos{ 0, 0, 1 }
				|| offset == CubePos{ 0, 0, -1 })
			{
				throw Exception("choice is collided");
			}
		}
	}
	return choices[choice];
}

std::vector<CubeDesc> GameLayer::GetRandomChoices()
{
	CubeFace* head = cube_faces_.back();

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

		switch (head->GetDirection())
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
				// Ӧ�ò���������������ע�͵�
				/*{ CubeFace::Type::Right, Direction::RightUp },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::Down },*/
				});
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
				{ CubeFace::Type::Right, Direction::LeftDown },
				{ CubeFace::Type::Right, Direction::Up },
				{ CubeFace::Type::Right, Direction::Down },
				// Ӧ�ò���������������ע�͵�
				/*{ CubeFace::Type::Left, Direction::LeftUp },
				{ CubeFace::Type::Left, Direction::Up },
				{ CubeFace::Type::Left, Direction::Down },*/
				});
			break;
		}
		break;
	}

	// ɸѡ���������ѡ��
	FilterChoices(choices);
	// KGE_LOG("choices", choices.size());
	return choices;
}

void GameLayer::FilterChoices(std::vector<CubeDesc>& choices)
{
	std::vector<CubeDesc> to_be_removed;
	CubeFace* head = cube_faces_.back();

	// ��Ҫȥ�������Ӿ�����Ӱ������
	// 1. ͷ�����Ѿ���ռ���������棬��������ռ�����һ����
	bool must_new_cube = (head->GetCube()->GetFacesCount() == 2);
	switch (head->GetType())
	{
	case CubeFace::Type::Top:
		switch (head->GetDirection())
		{
		case Direction::LeftDown:
			if (must_new_cube)
			{
				to_be_removed = {
					{ CubeFace::Type::Left, Direction::Down },
					{ CubeFace::Type::Left, Direction::LeftUp },
					{ CubeFace::Type::Left, Direction::RightDown },
				};
			}
			break;
		case Direction::RightDown:
			if (must_new_cube)
			{
				to_be_removed = {
					{ CubeFace::Type::Right, Direction::Down },
					{ CubeFace::Type::Right, Direction::RightUp },
					{ CubeFace::Type::Right, Direction::LeftDown },
				};
			}
			break;
		}
		break;

	case CubeFace::Type::Left:
		switch (head->GetDirection())
		{
		case Direction::Up:
			if (must_new_cube)
			{
				to_be_removed = {
					{ CubeFace::Type::Top, Direction::RightUp },
					{ CubeFace::Type::Top, Direction::RightDown },
					{ CubeFace::Type::Top, Direction::LeftUp },
				};
			}
			break;
		}
		break;

	case CubeFace::Type::Right:
		switch (head->GetDirection())
		{
		case Direction::Up:
			if (must_new_cube)
			{
				to_be_removed = {
					{ CubeFace::Type::Top, Direction::LeftUp },
					{ CubeFace::Type::Top, Direction::LeftDown },
					{ CubeFace::Type::Top, Direction::RightUp },
				};
			}
			break;
		}
		break;
	}

	// 2. ������������������ռ��ͬһ����
	for (auto iter = choices.begin(); iter != choices.end();)
	{
		auto pos = GetNewCubePos(*iter);

		if (auto cube = cube_map_.GetCubeFromMap(pos))
		{
			if (head->GetCube() != cube)
			{
				iter = choices.erase(iter);
				continue;
			}
		}
		++iter;
	}

	// �Ƴ�����������
	if (!to_be_removed.empty())
	{
		auto need_to_remove = [&](const CubeDesc& desc)
		{
			auto iter = std::find(to_be_removed.begin(), to_be_removed.end(), desc);
			return iter != to_be_removed.end();
		};

		choices.erase(
			std::remove_if(choices.begin(), choices.end(), need_to_remove),
			choices.end()
		);
	}
}

CubePos GameLayer::GetNewCubePos(CubeDesc desc)
{
	if (cube_faces_.empty())
		return CubePos{ 0, 0, 0 };

	CubeFace* head = cube_faces_.back();

	// �������λ��
	CubePos offset = { 0 };
	switch (head->GetType())
	{
	case CubeFace::Type::Top:

		switch (head->GetDirection())
		{
		case Direction::LeftUp:
			if (desc.type == CubeFace::Type::Top)
				offset = { -1, 0, 0 };
			else
				offset = { -1, 0, 1 };
			break;
		case Direction::LeftDown:
			if (desc.type == CubeFace::Type::Top)
				offset = { 0, 1, 0 };
			else
				offset = { 0, 0, 0 };
			break;
		case Direction::RightUp:
			if (desc.type == CubeFace::Type::Top)
				offset = { 0, -1, 0 };
			else
				offset = { 0, -1, 1 };
			break;
		case Direction::RightDown:
			if (desc.type == CubeFace::Type::Top)
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
			if (desc.type == CubeFace::Type::Top)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 0, 1 };
			break;
		case Direction::Down:
			if (desc.type == CubeFace::Type::Top)
				offset = { 0, 1, -1 };
			else
				offset = { 0, 0, -1 };
			break;
		case Direction::LeftUp:
			if (desc.type == CubeFace::Type::Left)
				offset = { -1, 0, 0 };
			else
				offset = { -1, 1, 0 };
			break;
		case Direction::RightDown:
			if (desc.type == CubeFace::Type::Left)
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
			if (desc.type == CubeFace::Type::Top)
				offset = { 0, 0, 0 };
			else
				offset = { 0, 0, 1 };
			break;
		case Direction::Down:
			if (desc.type == CubeFace::Type::Top)
				offset = { 1, 0, -1 };
			else
				offset = { 0, 0, -1 };
			break;
		case Direction::RightUp:
			if (desc.type == CubeFace::Type::Left)
				offset = { 1, -1, 0 };
			else
				offset = { 0, -1, 0 };
			break;
		case Direction::LeftDown:
			if (desc.type == CubeFace::Type::Left)
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
	return pos;
}

CubeFace* GameLayer::AppendCubeFace(CubeDesc desc)
{
	// ��ȡ�·���λ��
	auto pos = GetNewCubePos(desc);
	CubePtr cube = cube_map_.GetCubeFromMap(pos);
	if (!cube)
	{
		float side_length = GetWidth() * 0.08f;
		cube = cube_map_.CreateCube(pos, side_length);
		this->AddChild(cube);
	}

	KGE_LOG(type_map[desc.type], d_map[desc.direction]);

	auto face = cube->AddFace(desc);

	if (!cube_faces_.empty())
	{
		CubeFace* head = cube_faces_.back();
		head->SetNext(face);
	}
	cube_faces_.push_back(face);
	return face;
}
