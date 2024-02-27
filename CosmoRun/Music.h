#pragma once
#include "Common.h"

class Music : public Singleton<Music>
{
public:
	enum ID
	{
		Bg,
		Die,
		Click,
	};

	void Load();

	void Destroy();

	void Play(ID id, int loop = 0);

	Resource GetResource(ID id);

	RefPtr<Animation> LowVolume();

	RefPtr<Animation> HighVolume();

private:
	RefPtr<audio::SoundPlayer> player_;
};
