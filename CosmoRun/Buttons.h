#pragma once
#include "CustomText.h"

class HexagonButton : public PolygonActor, public SizeSensor
{
public:
	HexagonButton(float width);

	void Hide();

	void Show();

	void Enable();

	void Disable();

	void SetCallback(Function<void()> click);

	void OnUnitChanged(float unit) override;

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

protected:
	float width_;
	Status status_;
	RefPtr<Button> button_;
	Function<void()> click_;
};

class PlayButton : public HexagonButton
{
public:
	PlayButton();

	void OnStatusChanged(Status status) override;

	void OnUnitChanged(float unit) override;

private:
	RefPtr<CustomText> text_;
};

class SpecialHexButton : public HexagonButton
{
public:
	SpecialHexButton(float width);

	void ClearChildren();

	void OnStatusChanged(Status status) override;

	void OnUnitChanged(float unit) override;

private:
	RefPtr<PolygonActor> inner_polygon_;
};

class TryAgainButton : public SpecialHexButton
{
public:
	TryAgainButton();

	void OnUnitChanged(float unit) override;
};
