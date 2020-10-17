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

	ActorPtr wrapper_;
	BackgroundPtr background_;
	CustomTextPtr title_;
	BallPtr ball_;
	CubeGroupPtr cube_group_;
	CustomTextPtr score_text_;
	CustomTextPtr best_score_text_;
	RefPtr<PlayButton> play_button_;
	RefPtr<TryAgainButton> try_again_button_;
	ScoreBoardPtr score_board_;
};
