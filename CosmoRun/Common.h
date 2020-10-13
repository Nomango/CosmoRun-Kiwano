#pragma once
#include <kiwano/kiwano.h>

using namespace kiwano;

// ��ɫģʽ
enum class ColorEnum
{
	Blue,		// ��ɫ
	Purple,		// ��ɫ
	Gold,		// ��ɫ
};

struct Config
{
	static float Unit();

	static void SetWindowSize(uint32_t width, uint32_t height);

	static ColorEnum Color();

	static void Color(ColorEnum color);
};
