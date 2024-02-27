#include "Music.h"
#include "resource.h"

void Music::Load()
{
	player_ = new audio::SoundPlayer;
	player_->Preload(Resource(IDR_MP31, "MP3"));
	player_->Preload(Resource(IDR_MP32, "MP3"));
	player_->Preload(Resource(IDR_MP33, "MP3"));
}

void Music::Destroy()
{
	player_ = nullptr;
}

void Music::Play(ID id, int loop)
{
	player_->Play(GetResource(id), loop);
}

Resource Music::GetResource(ID id)
{
	switch (id)
	{
	case Music::Bg:
		return Resource(IDR_MP31, "MP3");
	case Music::Die:
		return Resource(IDR_MP32, "MP3");
	case Music::Click:
		return Resource(IDR_MP33, "MP3");
	default:
		break;
	}
	return {};
}

RefPtr<Animation> Music::LowVolume()
{
	return animation::Custom(500_msec, [=](Actor*, float frac) {
		player_->SetVolume(1.0f - 0.7f * frac);
	}).Delay(500_msec);
}

RefPtr<Animation> Music::HighVolume()
{
	return animation::Custom(500_msec, [=](Actor*, float frac) {
		player_->SetVolume(0.3f + 0.7f * frac);
	});
}
