#include "SplashStage.h"
#include "MainStage.h"
#include "Music.h"
#include "Lang.h"
#include "resource.h"
#include <thread>

SplashStage::SplashStage()
{
	logo_ = new Sprite(Resource(IDB_PNG1, "PNG"));
	logo_->SetAnchor(0.5f, 0.5f);
	AddChild(logo_);

	OnUnitChanged(Config::Unit());
}

void SplashStage::OnEnter()
{
	// �첽������Դ
	std::thread(Closure(this, &SplashStage::LoadResources)).detach();
}

void SplashStage::LoadResources()
{
	// ��������
	Font::Preload(Resource(IDR_FONT1, "TTF"));

	// һ���ȽϺ�ʱ����Դ����
	Music::GetInstance().Load();

	// ���ض���������
	Lang::Switch(Lang::Type::EN);

	// ��ʱ500����
	(500_msec).Sleep();

	Application::GetInstance().PreformInMainThread([]() {
		StagePtr stage = new MainStage;
		TransitionPtr transition = new FadeTransition(1500_msec);
		Director::GetInstance().EnterStage(stage, transition);
	});
}

void SplashStage::OnUnitChanged(float unit)
{
	float normal = Config::NormalizeUnit();
	logo_->SetScale(normal, normal);
	logo_->SetPosition(GetSize() / 2);
}
