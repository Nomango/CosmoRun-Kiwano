#include "GameLayer.h"

#define MAX_CUBE_NUMBER 12
#define PREPARE_CUBE_NUMBER 2

std::map<FaceType, String> type_map = {
		{ FaceType::Top, "top   " },
		{ FaceType::Left, "left  " },
		{ FaceType::Right, "right " },
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

	cube_faces_.clear();
	cube_map_.Clear();
	this->RemoveAllChildren();

	CircleActorPtr c = CircleActor::Create(3);
	c->SetFillColor(Color::Red);
	this->AddChild(c, 1);

	// ������һ������
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	AppendCubeFace({ FaceType::Top, choices[choice] });

	// ����������ͬ���͵ķ��飬������ڸտ�ʼ��Ϸʱ��Ӧ��Ϸ�ٶ�
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
	{
		AppendCubeFace({ FaceType::Top, choices[choice] });
	}

	// ������ɺ���ķ���
	for (int i = 0; i < length - PREPARE_CUBE_NUMBER; i++)
	{
		// �����������
		AddRandomFace();
	}

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

FaceDesc GameLayer::GetRandomChoice()
{
	auto choices = GetRandomChoices();
	if (choices.empty())
	{
		throw Exception("choices is empty");
	}

	// ���ѡ��һ�ֿ���
	int choice = math::Random(0, int(choices.size() - 1));
	return choices[choice];
}

std::vector<FaceDesc> GameLayer::GetRandomChoices()
{
	std::vector<FaceDesc> choices;

	CubeFace* head = cube_faces_.back();
	switch (head->GetType())
	{
	case FaceType::Top:

		switch (head->GetDirection())
		{
		case Direction::LeftUp:
			choices = {
				Face::Top_LeftUp,
				Face::Top_LeftDown,
				Face::Top_RightUp,
				Face::Left_Up,
				Face::Left_Down,
				Face::Right_Up,
			};
			break;
		case Direction::LeftDown:
			choices = {
				Face::Top_LeftUp,
				Face::Top_LeftDown,
				Face::Top_RightDown,
				Face::Right_Up,
				Face::Right_Down,
				Face::Left_Down,
			};
			break;
		case Direction::RightUp:
			choices = {
				Face::Top_LeftUp,
				Face::Top_RightDown,
				Face::Top_RightUp,
				Face::Right_Up,
				Face::Right_Down,
				Face::Left_Up,
			};
			break;
		case Direction::RightDown:
			choices = {
				Face::Top_LeftDown,
				Face::Top_RightDown,
				Face::Top_RightUp,
				Face::Left_Up,
				Face::Left_Down,
				Face::Right_Down,
			};
			break;
		}
		break;

	case FaceType::Left:

		switch (head->GetDirection())
		{
		case Direction::Up:
			choices = {
				Face::Left_Up,
				Face::Left_LeftUp,
				Face::Left_RightDown,
				Face::Right_LeftDown,
				Face::Right_RightUp,
				Face::Top_RightUp,
			};
			break;
		case Direction::Down:
			choices = {
				Face::Left_Down,
				Face::Left_LeftUp,
				Face::Left_RightDown,
				Face::Right_LeftDown,
				Face::Right_RightUp,
				Face::Top_LeftDown,
			};
			break;
		case Direction::LeftUp:
			choices = {
				Face::Left_Up,
				Face::Left_Down,
				Face::Left_LeftUp,
				Face::Right_LeftDown,
				Face::Top_LeftDown,
				Face::Top_RightUp,
			};
			break;
		case Direction::RightDown:
			choices = {
				Face::Left_Up,
				Face::Left_Down,
				Face::Left_RightDown,
				Face::Right_RightUp,
				Face::Top_LeftDown,
				Face::Top_RightUp,
			};
			break;
		}
		break;

	case FaceType::Right:

		switch (head->GetDirection())
		{
		case Direction::Up:
			choices = {
				Face::Right_Up,
				Face::Right_LeftDown,
				Face::Right_RightUp,
				Face::Left_RightDown,
				Face::Left_LeftUp,
				Face::Top_LeftUp,
			};
			break;
		case Direction::Down:
			choices = {
				Face::Right_Down,
				Face::Right_LeftDown,
				Face::Right_RightUp,
				Face::Left_RightDown,
				Face::Left_LeftUp,
				Face::Top_RightDown,
			};
		case Direction::RightUp:
			choices = {
				Face::Right_Up,
				Face::Right_Down,
				Face::Right_RightUp,
				Face::Left_RightDown,
				Face::Top_RightDown,
				Face::Top_LeftUp,
			};
			break;
		case Direction::LeftDown:
			choices = {
				Face::Right_Up,
				Face::Right_Down,
				Face::Right_LeftDown,
				Face::Left_LeftUp,
				Face::Top_RightDown,
				Face::Top_LeftUp,
			};
			break;
		}
		break;
	}

	// ɸѡ���������ѡ��
	FilterChoices(choices);
	// KGE_LOG("choices", choices.size());
	return choices;
}

void GameLayer::FilterChoices(std::vector<FaceDesc>& choices)
{
	CubeFace* head = cube_faces_.back();

	for (auto iter = choices.begin(); iter != choices.end();)
	{
		auto pos = GetNewCubePos(*iter);

		if (cube_map_.IsCollidedWith(pos, *iter, head))
			iter = choices.erase(iter);
		else
			++iter;
	}
}

CubePos GameLayer::GetNewCubePos(FaceDesc desc)
{
	if (cube_faces_.empty())
		return CubePos{ 0, 0, 0 };

	CubeFace* head = cube_faces_.back();

	// �������λ��
	CubePos offset = { 0 };
	switch (head->GetType())
	{
	case FaceType::Top:

		if (desc == Face::Top_LeftDown)
			offset = { 0, 1, 0 };
		else if (desc == Face::Top_LeftUp)
			offset = { -1, 0, 0 };
		else if (desc == Face::Top_RightDown)
			offset = { 1, 0, 0 };
		else if (desc == Face::Top_RightUp)
			offset = { 0, -1, 0 };
		else if (desc == Face::Left_Up)
			offset = { 0, -1, 1 };
		else if (desc == Face::Left_Down)
			offset = { 0, 0, 0 };
		else if (desc == Face::Right_Up)
			offset = { -1, 0, 1 };
		else if (desc == Face::Right_Down)
			offset = { 0, 0, 0 };
		else
			assert(0);
		break;

	case FaceType::Left:

		if (desc == Face::Left_Down)
			offset = { 0, 0, -1 };
		else if (desc == Face::Left_Up)
			offset = { 0, 0, 1 };
		else if (desc == Face::Left_LeftUp)
			offset = { -1, 0, 0 };
		else if (desc == Face::Left_RightDown)
			offset = { 1, 0, 0 };
		else if (desc == Face::Right_LeftDown)
			offset = { -1, 1, 0 };
		else if (desc == Face::Right_RightUp)
			offset = { 0, 0, 0 };
		else if (desc == Face::Top_RightUp)
			offset = { 0, 0, 0 };
		else if (desc == Face::Top_LeftDown)
			offset = { 0, 1, -1 };
		else
			assert(0);
		break;

	case FaceType::Right:

		if (desc == Face::Right_Down)
			offset = { 0, 0, -1 };
		else if (desc == Face::Right_Up)
			offset = { 0, 0, 1 };
		else if (desc == Face::Right_LeftDown)
			offset = { 0, 1, 0 };
		else if (desc == Face::Right_RightUp)
			offset = { 0, -1, 0 };
		else if (desc == Face::Left_LeftUp)
			offset = { 0, 0, 0 };
		else if (desc == Face::Left_RightDown)
			offset = { 1, -1, 0 };
		else if (desc == Face::Top_RightDown)
			offset = { 1, 0, -1 };
		else if (desc == Face::Top_LeftUp)
			offset = { 0, 0, 0 };
		else
			assert(0);
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

CubeFace* GameLayer::AppendCubeFace(FaceDesc desc)
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

	KGE_LOG("-new-", type_map[desc.type], d_map[desc.direction]);

	auto face = cube->AddFace(desc);

	if (!cube_faces_.empty())
	{
		CubeFace* head = cube_faces_.back();
		head->SetNext(face);
	}
	cube_faces_.push_back(face);
	return face;
}
