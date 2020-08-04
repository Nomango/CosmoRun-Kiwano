#include "GameOverPanel.h"
#include "CustomText.h"
#include "Lang.h"

GameOverPanel::GameOverPanel(Size size)
{
	FontPtr font = new Font("Gill Sans MT", 83, FontWeight::Bold);

	// 创建背景
	RectActorPtr bg = new RectActor(size);
	bg->SetAnchor(0.5f, 0.5f);
	bg->SetFillColor(Color::Rgba(40, 40, 40, 0.6f));
	this->AddChild(bg);

	// 创建游戏结束标题文字
	TextActorPtr title = new CustomText(Lang::Get("gameover", "title"), 60, false);
	title->MoveBy(0, -size.y / 2 * 0.7f);
	this->AddChild(title);

	// 创建得分文字
	score_ = new CustomText(" ", 60, false);
	score_->MoveTo(0, title->GetPositionY() + title->GetHeight());
	this->AddChild(score_);

	// 创建最高分文字
	best_score_ = new CustomText(" ", 48, false);
	best_score_->MoveTo(0, score_->GetPositionY() + score_->GetHeight());
	this->AddChild(best_score_);

	this->SetCascadeOpacityEnabled(true);
}

void GameOverPanel::SetScore(int score, int best_score, bool is_best)
{
	score_->SetText(std::to_string(score));
	if (is_best)
	{
		best_score_->SetText(Lang::Get("gameover", "new_best"));
	}
	else
	{
		best_score_->SetText(Lang::Get("gameover", "best_score") + strings::Format(" %3d", best_score));
	}
}

void GameOverPanel::Show()
{
	this->SetOpacity(0);
	this->SetVisible(true);
	this->AddAction(ActionFadeIn(500_msec).Delay(600_msec));
}

void GameOverPanel::Hide()
{
	this->AddAction(ActionFadeOut(500_msec));
}
