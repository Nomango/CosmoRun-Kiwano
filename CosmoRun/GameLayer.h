#pragma once
#include "Common.h"
#include "Buttons.h"
#include "Cube.h"
#include "Ball.h"
#include "CubeGroup.h"
#include "Background.h"
#include "GameOverPanel.h"

KGE_DECLARE_SMART_PTR(GameLayer);

class GameLayer : public Actor
{
public:
	GameLayer(Size size);

	void InitGame();

	void StartGame();

	void GameOver();

	void Restart();

	void Move(Vec2 trans);

	void SetColor(ColorEnum color);

private:
	void OnUpdate(Duration dt) override;

private:
	enum class GameStatus
	{
		Ready,
		Running,
		Gameover,
	};

	GameStatus status_;

	// 计分相关
	int score_;
	int best_score_;
	float speed_scale_;

	// 结束面板
	GameOverPanelPtr gameover_panel_;

	RefPtr<PlayButton> play_button_;
	RefPtr<TryAgainButton> try_again_button_;

	BackgroundPtr background_;
	BallPtr ball_;
	CubeGroupPtr cube_group_;
};
