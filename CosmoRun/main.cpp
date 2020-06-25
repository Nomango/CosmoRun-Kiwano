#include "MainStage.h"

KGE_DECLARE_SMART_PTR(DemoRunner);
class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		Logger::GetInstance().ShowConsole(true);

		Settings s;
		s.window.title = "CosmoRun";
		s.window.width = 800;
		s.window.height = 600;
		s.debug_mode = true;
		SetSettings(s);
	}

	void OnReady() override
	{
		MainStagePtr stage = MainStage::Create();
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
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
