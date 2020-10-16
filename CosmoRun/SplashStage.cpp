#include "SplashStage.h"
#include "MainStage.h"
#include "Music.h"
#include "Lang.h"
#include "resource.h"
#include <thread>

SplashStage::SplashStage()
{
	SpritePtr logo = new Sprite(Resource(IDB_PNG1, "PNG"));
	logo->SetAnchor(0.5f, 0.5f);
	logo->SetPosition(GetSize() / 2);
	AddChild(logo);
}

void SplashStage::OnEnter()
{
	std::thread(Closure(this, &SplashStage::LoadResources)).detach();
}

void SplashStage::LoadResources()
{
	FileSystem::GetInstance().AddSearchPath("resource/");

	// ��������
	Font::Preload(Resource(IDR_FONT1, "TTF"));

	// һ���ȽϺ�ʱ����Դ����
	Music::GetInstance().Load();

	// ���ض���������
	Lang::Switch(Lang::Type::EN);

	Application::GetInstance().PreformInMainThread([]() {
		StagePtr stage = new MainStage;
		TransitionPtr transition = new FadeTransition(1500_msec);
		Director::GetInstance().EnterStage(stage, transition);
	});
}
