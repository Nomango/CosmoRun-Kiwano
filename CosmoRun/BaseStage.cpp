#include "BaseStage.h"

BaseStage::BaseStage()
{
	// ���ڴ�С�仯����
	AddListener<WindowResizedEvent>([=](Event* evt) {
		auto win_evt = evt->Cast<WindowResizedEvent>();
		Size win_size(float(win_evt->width), float(win_evt->height));
		this->SetSize(win_size);
		Config::SetWindowSize(win_size);
	});
}
