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
	// 异步加载资源
	std::thread(Closure(this, &SplashStage::LoadResources)).detach();
}

void SplashStage::LoadResources()
{
	// 加载字体
	Font::Preload(Resource(IDR_FONT1, "TTF"));

	// 一个比较耗时的资源加载
	Music::GetInstance().Load();

	// 加载多语言配置
	Lang::Switch(Lang::Type::EN);

	// 延时500毫秒
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
