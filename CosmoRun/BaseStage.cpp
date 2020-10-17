#include "BaseStage.h"

BaseStage::BaseStage()
{
	// 窗口大小变化监听
	AddListener<WindowResizedEvent>([=](Event* evt) {
		auto win_evt = evt->Cast<WindowResizedEvent>();
		Size win_size(float(win_evt->width), float(win_evt->height));
		this->SetSize(win_size);
		Config::SetWindowSize(win_size);
	});
}
