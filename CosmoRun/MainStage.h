#pragma once
#include "BaseStage.h"
#include "CustomText.h"
#include "Buttons.h"
#include "Cube.h"
#include "Ball.h"
#include "CubeGroup.h"
#include "Background.h"
#include "ScoreBoard.h"
#include "Music.h"

class MainStage : public BaseStage
{
public:
	MainStage();

	void InitGame();

	void StartGame();

	void GameOver();

	void Restart();

	void SetBestScore(int best_score);

	void Move(Vec2 trans);

	void SetColor(ColorEnum color);

	void OnKeyDown(Event* evt);

	void OnEnter() override;

	void OnUpdate(Duration dt) override;

	void OnUnitChanged(float unit) override;

private:
	enum class GameStatus
	{
		Ready,
		Running,
		Gameover,
		Restarting,
	};

	// 计分相关
	int score_;
	int best_score_;

	// 状态相关
	float speed_scale_;
	GameStatus status_;

	RefPtr<Actor> wrapper_;
	RefPtr<Background> background_;
	RefPtr<CustomText> title_;
	RefPtr<Ball> ball_;
	RefPtr<CubeGroup> cube_group_;
	RefPtr<CustomText> score_text_;
	RefPtr<CustomText> best_score_text_;
	RefPtr<PlayButton> play_button_;
	RefPtr<TryAgainButton> try_again_button_;
	RefPtr<ScoreBoard> score_board_;
};
