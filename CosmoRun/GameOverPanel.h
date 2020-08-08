#pragma once
#include "Common.h"
#include "CustomText.h"

KGE_DECLARE_SMART_PTR(GameOverPanel);

class GameOverPanel : public Actor
{
public:
    GameOverPanel(Size size);

    void SetScore(int score, int best_score, bool is_best);

    void Show();

    void Hide();

private:
    CustomTextPtr score_;
    CustomTextPtr best_score_;
};
