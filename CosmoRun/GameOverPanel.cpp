#include "GameOverPanel.h"
#include "Lang.h"

GameOverPanel::GameOverPanel(Size size)
{
	// 创建背景
	bg_ = new RectActor(size);
	bg_->SetAnchor(0.5f, 0.5f);
	bg_->SetFillColor(Color::Rgba(40, 40, 40, 0.6f));
	this->AddChild(bg_);

	// 创建游戏结束标题文字
	title_ = new CustomText(Lang::Get("gameover", "title"), 60, false);
	title_->MoveBy(0, -size.y / 2 * 0.7f);
	this->AddChild(title_);

	// 创建得分文字
	score_ = new CustomText(" ", 60, false);
	score_->MoveTo(0, title_->GetPositionY() + title_->GetHeight());
	this->AddChild(score_);

	// 创建最高分文字
	best_score_ = new CustomText(" ", 48, false);
	best_score_->MoveTo(0, score_->GetPositionY() + score_->GetHeight());
	this->AddChild(best_score_);

	Resize(size);
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
	this->StartAnimation(animation::FadeIn(500_msec).Delay(600_msec));
}

void GameOverPanel::Hide()
{
	this->StartAnimation(animation::FadeOut(500_msec));
}

void GameOverPanel::Resize(Size size)
{
	bg_->SetRectSize(size);
	title_->MoveTo(0, -size.y / 2 * 0.7f);
	score_->MoveTo(0, title_->GetPositionY() + title_->GetHeight());
	best_score_->MoveTo(0, score_->GetPositionY() + score_->GetHeight());
}
