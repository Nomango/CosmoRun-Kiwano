#pragma once
#include "Common.h"
#include "CustomText.h"

class ScoreBoard : public Actor
{
public:
    ScoreBoard(Size size);

    void SetScore(int score, int best_score, bool is_best);

    void Show();

    void Hide();

    void Resize(Size size);

private:
    RefPtr<RectActor> bg_;
    RefPtr<CustomText> title_;
    RefPtr<CustomText> score_;
    RefPtr<CustomText> best_score_;
};
