#include "MainStage.h"
#include "Lang.h"
#include "resource.h"

KGE_DECLARE_SMART_PTR(DemoRunner);
class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		Settings s;
		s.window.title = "CosmoRun";
		s.window.width = 800;
		s.window.height = 600;
		s.window.resizable = true;
		s.window.icon = Icon(IDI_ICON1);

#ifdef KGE_DEBUG
		s.debug_mode = true;

		// 输出日志到文件
		auto provider = new FileLogProvider("log.txt", std::ios_base::app);
		provider->SetLevel(LogLevel::Info);
		Logger::GetInstance().AddProvider(provider);

		KGE_LOG("-=-=-=-=-= Start new game =-=-=-=-=-");
#endif
		SetSettings(s);

		Config::Color(Config::RandomColor());
		Config::SetWindowSize(Size(s.window.width, s.window.height));

		// 使用音频模块
		Application::GetInstance().Use(AudioModule::GetInstance());

		// 对象创建失败时抛出
		ObjectBase::SetObjectPolicy(ObjectPolicy::Exception());
	}

	void OnReady() override
	{
		// 加载资源
		LoadResources();

		// 设置窗口最小大小
		GetWindow()->SetMinimumSize(600, 400);

		MainStagePtr stage = new MainStage;
		Director::GetInstance().EnterStage(stage);
	}

	void LoadResources()
	{
		FileSystem::GetInstance().AddSearchPath("resource/");

		Font::Preload(Resource(IDR_FONT1, "TTF"));

		// 加载多语言配置
		Lang::Switch(Lang::Type::EN);
	}
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
#if defined(_WIN32)
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	try
	{
		DemoRunnerPtr runner = new DemoRunner;
		Application::GetInstance().Run(runner);
	}
	catch (std::exception& e)
	{
		KGE_ERROR(e.what());
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
