#pragma once
#include "Common.h"

KGE_DECLARE_SMART_PTR(GameOverPanel);

class GameOverPanel : public Actor
{
public:
    GameOverPanel(Size size);

    void SetScore(int score, int best_score, bool is_best);

    void Show();

    void Hide();

private:
    TextActorPtr score_;
    TextActorPtr best_score_;
};
