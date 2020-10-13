#pragma once
#include "Common.h"

class HexagonButton : public PolygonActor
{
public:
	HexagonButton(float width);

	void Hide();

	void Show();

	void Enable();

	void Disable();

	void SetCallback(Function<void()> click);

protected:
	enum class Status
	{
		Normal,
		Hover,
		Pressed,
	};

	virtual void OnStatusChanged(Status status) = 0;

	void OnButtonEvent(Button* btn, Button::Event evt);

	Vector<Point> GetHexagonVertices(float width, float side, Point offset);

private:
	Status status_;
	ButtonPtr button_;
	Function<void()> click_;
};

class PlayButton : public HexagonButton
{
public:
	PlayButton();

	void OnStatusChanged(Status status) override;
};

class SpecialHexButton : public HexagonButton
{
public:
	SpecialHexButton();

	void OnStatusChanged(Status status) override;

private:
	PolygonActorPtr inner_polygon_;
};

class TryAgainButton : public SpecialHexButton
{
public:
	TryAgainButton();
};
