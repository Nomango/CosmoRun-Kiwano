#include "Music.h"
#include "resource.h"

Music::Music()
{
	bg_id_ = player_.Load(Resource(IDR_MP31, "MP3"));
	die_id_ = player_.Load(Resource(IDR_MP32, "MP3"));
}

void Music::PlayBackground()
{
	player_.Play(bg_id_, -1);
}

void Music::PlayDie()
{
	player_.Play(die_id_);
}

AnimationPtr Music::LowVolume()
{
	return animation::Custom(500_msec, [=](Actor*, float frac) {
		player_.SetVolume(1.0f - 0.7f * frac);
	}).Delay(500_msec);
}

AnimationPtr Music::HighVolume()
{
	return animation::Custom(500_msec, [=](Actor*, float frac) {
		player_.SetVolume(0.3f + 0.7f * frac);
	});
}
