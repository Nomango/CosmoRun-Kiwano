#include "SplashStage.h"
#include "Music.h"
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
		// Debug ģʽ�»�򿪿���̨���������־���ļ���
		s.debug_mode = true;

		// �����־���ļ�
		auto provider = new FileLogProvider("log.txt", std::ios_base::app);
		provider->SetLevel(LogLevel::Info);
		Logger::GetInstance().AddProvider(provider);

		KGE_LOG("-=-=-=-=-= Start new game =-=-=-=-=-");
#endif
		SetSettings(s);

		Config::Color(Config::RandomColor());
		Config::SetWindowSize(Size(s.window.width, s.window.height));

		// ʹ����Ƶģ��
		Application::GetInstance().Use(AudioModule::GetInstance());

		// ���󴴽�ʧ��ʱ�׳�
		ObjectBase::SetObjectPolicy(ObjectPolicy::Exception());
	}

	void OnReady() override
	{
		// ���ô�����С��С
		GetWindow()->SetMinimumSize(600, 400);

		StagePtr stage = new SplashStage;
		TransitionPtr transition = new FadeTransition(1500_msec);
		Director::GetInstance().EnterStage(stage, transition);
	}

	void OnDestroy() override
	{
		Music::GetInstance().Destroy();
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
	}
	return 0;
}
