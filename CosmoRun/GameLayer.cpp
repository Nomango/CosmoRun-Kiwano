#include "GameLayer.h"

#define MAX_CUBE_NUMBER 12
#define HIDE_CUBE_NUMBER 9
#define PREPARE_CUBE_NUMBER 3

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
	, current_face_(nullptr)
{
	SetSize(size);
}

void GameLayer::StartGame()
{
	RemoveAllTasks();

	InitCubes(MAX_CUBE_NUMBER);

	TaskPtr task = Task::Create(Closure(this, &GameLayer::TickTest), 500_msec);
	AddTask(task);
}

void GameLayer::SetColor(ColorEnum color)
{
	if (color_ != color)
	{
		color_ = color;

		cube_map_.SetColor(color);
	}
}

void GameLayer::RemoveFace(Actor* face)
{
	cube_map_.RemoveCubeFaceInMap((CubeFace*)face);
}

void GameLayer::InitCubes(int length)
{
	KGE_LOG();

	hide_faces_.clear();
	cube_map_.Clear();
	this->RemoveAllChildren();

	// 创建第一个方块
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	current_face_ = AppendCubeFace({ FaceType::Top, choices[choice] });

	// 创建几个相同类型的方块，让玩家在刚开始游戏时适应游戏速度
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
		AppendCubeFace({ FaceType::Top, choices[choice] });

	// 随机生成后面的方块
	for (int i = 0; i < length + HIDE_CUBE_NUMBER - PREPARE_CUBE_NUMBER; i++)
		AddRandomFace();

	KGE_LOG();
}

void GameLayer::TickTest(Task* task, Duration dt)
{
	AddRandomFace();
	RemoveTailFace();

	auto face = hide_faces_.front();
	AddAction(Tween::MoveTo(400_msec, GetSize() / 2 - face->GetCube()->GetPosition()));
}

void GameLayer::AddRandomFace()
{
	// 获取随机的方块类型
	auto choices = GetRandomChoices();
	if (!choices.empty())
	{
		// 随机选择一种可能
		int choice = math::Random(0, int(choices.size() - 1));

		// 创建新方块
		AppendCubeFace(choices[choice]);
	}
	else
	{
		// 获取随机方块失败，需要删掉之前的方块重新生成
		if (hide_faces_.size() < 5)
		{
			KGE_LOG();
			KGE_LOG("====");
			for (auto face : hide_faces_)
			{
				KGE_LOG(type_map[face->GetType()], d_map[face->GetDirection()]);
			}
			KGE_LOG("====");
			KGE_LOG();

			throw Exception("Internal algorithm error");
		}
		else if (hide_faces_.size() == 5)
		{
			RemoveHeadFace();

			AddRandomFace();
			AddRandomFace();
		}
		else
		{
			// 方块数量较多时，可能存在圈形路径，需要多删几个
			RemoveHeadFace();
			RemoveHeadFace();

			AddRandomFace();
			AddRandomFace();
			AddRandomFace();
		}
	}
}

CubeFace* GameLayer::AppendCubeFace(FaceDesc desc)
{
	// 获取新方块位置
	auto pos = GetNewCubePos(desc);
	CubePtr cube = cube_map_.GetCubeFromMap(pos);
	if (!cube)
	{
		float side_length = GetWidth() * 0.08f;
		cube = cube_map_.CreateCube(pos, side_length);
		this->AddChild(cube);
	}

	KGE_LOG("-new-", type_map[desc.type], d_map[desc.direction]);

	// 创建新方块面，并设置为透明
	auto face = cube->AddFace(desc);
	face->SetVisible(false);

	if (!hide_faces_.empty())
	{
		CubeFace* head = hide_faces_.back();
		head->SetNext(face);
	}
	hide_faces_.push_back(face);

	// 当隐藏的方块数量较多时，显示最后一个
	if (hide_faces_.size() > HIDE_CUBE_NUMBER)
	{
		hide_faces_.front()->Show();
		hide_faces_.pop_front();
	}
	return face;
}

void GameLayer::RemoveTailFace()
{
	auto action = Tween::FadeOut(500_msec).DoneCallback(Closure(this, &GameLayer::RemoveFace));
	current_face_->AddAction(action);
	current_face_ = current_face_->GetNext();
}

void GameLayer::RemoveHeadFace()
{
	KGE_LOG("-remove- ", type_map[hide_faces_.back()->GetType()], d_map[hide_faces_.back()->GetDirection()]);

	auto head = hide_faces_.back();
	cube_map_.RemoveCubeFaceInMap(head);
	hide_faces_.pop_back();
}

std::vector<FaceDesc> GameLayer::GetRandomChoices()
{
	std::vector<FaceDesc> choices;

	CubeFace* head = hide_faces_.back();
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
			break;
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

	// 筛选掉视觉上冲突的情况
	for (auto iter = choices.begin(); iter != choices.end();)
	{
		auto pos = GetNewCubePos(*iter);

		if (cube_map_.IsCollidedWith(pos, *iter, head))
			iter = choices.erase(iter);
		else
			++iter;
	}

	// KGE_LOG("choices", choices.size());
	return choices;
}

CubePos GameLayer::GetNewCubePos(FaceDesc desc)
{
	if (hide_faces_.empty())
		return CubePos{ 0, 0, 0 };

	CubeFace* head = hide_faces_.back();

	// 计算相对位置
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

	// 计算新方块的坐标
	auto pos = head->GetCube()->GetPos();
	for (int i = 0; i < 3; i++)
	{
		pos[i] += offset[i];
	}
	return pos;
}
