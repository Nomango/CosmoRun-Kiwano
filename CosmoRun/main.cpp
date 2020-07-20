#include "MainStage.h"

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

#ifdef KGE_DEBUG
		s.debug_mode = true;

		// 输出日志到文件
		auto provider = FileLogProvider::Create("log.txt", std::ios_base::app);
		provider->SetLevel(LogLevel::Info);
		Logger::GetInstance().AddProvider(provider);

		KGE_LOG("-=-=-=-=-= Start new game =-=-=-=-=-");
#endif
		SetSettings(s);
	}

	void OnReady() override
	{
		MainStagePtr stage = new MainStage;
		Director::GetInstance().EnterStage(stage);
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
