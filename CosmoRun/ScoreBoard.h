#pragma once
#include "Common.h"
#include "CustomText.h"

KGE_DECLARE_SMART_PTR(ScoreBoard);

class ScoreBoard : public Actor
{
public:
    ScoreBoard(Size size);

    void SetScore(int score, int best_score, bool is_best);

    void Show();

    void Hide();

    void Resize(Size size);

private:
    RectActorPtr bg_;
    CustomTextPtr title_;
    CustomTextPtr score_;
    CustomTextPtr best_score_;
};
