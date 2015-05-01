#pragma once

#include "ScreenNumber.h"

enum pixelState
{
	ON = true,
	OFF = false
};

class Screen
{
public:
	Screen();
	~Screen();

	class PixelArray
	{
	public:
		PixelArray(int sizeX, int sizeY);
		~PixelArray();

		void setPixel(int x, int y, pixelState state);
		bool getPixel(int x, int y);
		int xyToIndex(int x, int y);
		void clear();

		int getSizeX(){ return sizeX; };
		int getSizeY(){ return sizeY; };
	private:
		int sizeX, sizeY;
		bool *pixels;
	};

	bool music = false;

	ScreenNumber score;
	ScreenNumber highScore;
	ScreenNumber level;
	ScreenNumber speed;

	PixelArray mainArray;
	PixelArray hintArray;

private:
};