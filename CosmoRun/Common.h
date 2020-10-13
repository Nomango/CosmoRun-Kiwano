#pragma once
#include <kiwano/kiwano.h>

using namespace kiwano;

// 颜色模式
enum class ColorEnum
{
	Blue,		// 蓝色
	Purple,		// 紫色
	Gold,		// 金色
};

struct Config
{
	static float Unit();

	static void SetWindowSize(uint32_t width, uint32_t height);

	static ColorEnum Color();

	static void Color(ColorEnum color);
};
