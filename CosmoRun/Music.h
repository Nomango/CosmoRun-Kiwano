#pragma once
#include "Common.h"

class Music : public Singleton<Music>
{
public:
	void Load();

	void Destroy();

	void PlayBackground();

	void PlayDie();

	AnimationPtr LowVolume();

	AnimationPtr HighVolume();

private:
	size_t bg_id_ = 0;
	size_t die_id_ = 0;
	SoundPlayer player_;
};