#include "CubeGroup.h"

#define HIDE_CUBE_NUMBER	9		// ���صķ�������
#define PREPARE_CUBE_NUMBER	3		// �������Ӧ��Ϸ�ٶȵĳ�ʼ��������

CubeGroup::CubeGroup(float side_length)
	: side_length_(side_length)
	, tail_(nullptr)
{
}

void CubeGroup::InitCubes(int length)
{
	KGE_LOG();

	hide_faces_.clear();
	cube_map_.Clear();
	this->RemoveAllChildren();

	// ������һ������
	std::vector<Direction> choices = { Direction::LeftUp, Direction::LeftDown, Direction::RightUp, Direction::RightDown };
	int choice = math::Random(0, int(choices.size() - 1));

	tail_ = AppendCubeFace({ FaceType::Top, choices[choice] });

	// ����������ͬ���͵ķ��飬������ڸտ�ʼ��Ϸʱ��Ӧ��Ϸ�ٶ�
	for (int i = 0; i < PREPARE_CUBE_NUMBER; i++)
		AppendCubeFace({ FaceType::Top, choices[choice] });

	// ������ɺ���ķ���
	for (int i = 0; i < length + HIDE_CUBE_NUMBER - PREPARE_CUBE_NUMBER; i++)
		AddRandomFace();

	KGE_LOG();
}

void CubeGroup::AddRandomFace(int depth)
{
	if (depth >= 10)
	{
		// �ݹ���˵���㷨�쳣�����޷��������·���
		DumpCubes();
		throw Exception("AddRandomFace failed");
	}

	// ��ȡ����ķ�������
	auto choices = GetRandomChoices();
	if (!choices.empty())
	{
		// ���ѡ��һ�ֿ���
		int choice = math::Random(0, int(choices.size() - 1));

		// �����·���
		AppendCubeFace(choices[choice]);
	}
	else
	{
		// ��ȡ�������ʧ�ܣ���Ҫɾ��֮ǰ�ķ�����������
		int length = (int)hide_faces_.size();
		if (length >= 5)
		{
			int rebuild = std::min(length - 5, depth);
			for (int i = 0; i < rebuild; i++)
			{
				RemoveHeadFace();
			}

			for (int i = 0; i < rebuild + 1; i++)
			{
				AddRandomFace(depth + 1);
			}
		}
		else
		{
			// ����С��5ʱ��Ȼ��ͻ��˵���������㷨�쳣
			DumpCubes();
			throw Exception("Internal algorithm error #1");
		}
	}
}

CubeFace* CubeGroup::AppendCubeFace(FaceDesc desc)
{
	// ��ȡ�·���λ��
	auto pos = GetNewCubePos(desc);
	CubePtr cube = cube_map_.GetCubeFromMap(pos);
	if (!cube)
	{
		cube = cube_map_.CreateCube(pos, side_length_);
		this->AddChild(cube);
	}

	KGE_LOG("-new-", desc);

	// �����·����棬������Ϊ͸��
	auto face = cube->AddFace(desc);
	face->SetVisible(false);

	// ��ӷ��������Ӱ
	ActorPtr shadow = face->GetShadow();
	shadow->SetPosition(face->GetPosition() + cube->GetPosition());
	this->AddChild(shadow, -1);

	if (!hide_faces_.empty())
	{
		CubeFace* head = hide_faces_.front();
		head->SetNext(face);
	}
	hide_faces_.push_front(face);

	// �����صķ��������϶�ʱ����ʾ���һ��
	if (hide_faces_.size() > HIDE_CUBE_NUMBER)
	{
		hide_faces_.back()->Show();
		hide_faces_.pop_back();
	}
	return face;
}

void CubeGroup::RemoveFace(CubeFace* face)
{
	cube_map_.RemoveCubeFaceInMap(face);
}

CubeFace* CubeGroup::GetTail() const
{
	return tail_;
}

void CubeGroup::SetColor(ColorEnum color)
{
	cube_map_.SetColor(color);
}

void CubeGroup::RemoveTailFace()
{
	auto action = animation::ScaleTo(300_msec, Vec2(0, 0));
	// �����������Ƴ�������
	auto handler = AnimationEventHandler::HandleDone([=](Animation*, Actor* face) { this->RemoveFace((CubeFace*)face); });
	action.Handler(handler);
	tail_->StartAnimation(action);

	// ������Ӱ
	tail_->GetShadow()->StartAnimation(animation::ScaleTo(300_msec, Vec2(0, 0)));

	tail_ = tail_->GetNext();
}

void CubeGroup::RemoveHeadFace()
{
	if (hide_faces_.empty())
		throw std::out_of_range("call RemoveHeadFace on empty list");

	KGE_LOG("-remove- ", hide_faces_.front()->GetDesc());

	auto head = hide_faces_.front();
	cube_map_.RemoveCubeFaceInMap(head);
	hide_faces_.pop_front();
}

std::vector<FaceDesc> CubeGroup::GetRandomChoices()
{
	std::vector<FaceDesc> choices;

	CubeFace* head = hide_faces_.front();
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

	// ɸѡ���Ӿ��ϳ�ͻ�����
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

CubePos CubeGroup::GetNewCubePos(FaceDesc desc)
{
	if (hide_faces_.empty())
		return CubePos{ 0, 0, 0 };

	CubeFace* head = hide_faces_.front();

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

void CubeGroup::DumpCubes()
{
	KGE_LOG();
	KGE_LOG("==== dump cubes ====");
	for (auto iter = hide_faces_.rbegin(); iter != hide_faces_.rend(); iter++)
	{
		KGE_LOG((*iter)->GetDesc());
	}
	KGE_LOG("====================");
	KGE_LOG();
}
