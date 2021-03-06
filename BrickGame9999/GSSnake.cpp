#include "GSSnake.hpp"

#include <iostream>
using namespace std;

namespace GSSnake
{
	State::State(Device &dev, Variant variant) :
		foodTicker(20),
		snakeTicker(15),
		snake(dev)
	{
		currentVariant = variant;

		if (!dev.inGame)
		{
			dev.score = 0;
			if (variant == VARIANT_NORMAL)
			{
				dev.lives = 3;
			}
			else if (variant == VARIANT_INFINITE)
			{
				dev.lives = 1;
			}
		}

		if (variant == VARIANT_NORMAL)
		{
			dev.highScoreLetter = 'A';
			snakeLengthRequired = 20 + 3;
		}
		else if (variant == VARIANT_INFINITE)
		{
			dev.highScoreLetter = 'B';
			snakeLengthRequired = logicalScreen.w * logicalScreen.h;
		}

		dev.inGame = true;

		dev.screen.mainArray.clear();
		dev.screen.hintArray.clear();

		dev.screen.hintArray.setCount(dev.lives);

		defineLevels();

		reset(dev);
	}

	State::~State()
	{
	}

	void State::parseEvent(Device &dev, Key k, KeyState state)
	{
		snake.parseEvent(k, state);

		if (snake.isSpeeding())
		{
			snakeTicker.setLength(3);
		}
		else
		{
			snakeTicker.setLength(snake.getSpeed());
		}
	}

	void State::tick(Device& dev)
	{
		switch (stateSegment)
		{
		case 0:
			tickPause(dev);
			break;
		case 1:
			tickSnake(dev);
			break;
		case 2:
			tickExplosion(dev);
			break;
		}
	}

	void State::tickSnake(Device &dev)
	{
		/*******************************************   LOGIC   *********************************/
		bool crashed = false;
		// move snake

		if (snakeTicker.triggered())
		{
			snake.tick(dev);
			snakeTicker.reset();
		}
		else
		{
			snakeTicker.tick();
		}

		// check for collision with itself and walls
		coord head = snake.getHead();

		if ((levels[dev.getLevel() % levelCount][head.y * 10 + head.x] != ' '))
		{
			dev.lives--;
			crashed = true;
		}

		for (int i = 1; i < snake.getLength(); i++)
		{
			if (snake.getHead() == snake.getSegment(i))
			{
				dev.lives--;
				crashed = true;
				break;
			}
		}

		// check for collision with food
		if (snake.getHead() == food)
		{
			dev.speaker.playSound(Sound::SND_BOUNCE);
			dev.increaseScore(10);

			genFood(dev.getLevel() % levelCount);
			snake.setLength(snake.getLength() + 1);

			if (snake.getLength() == snakeLengthRequired)
			{
				dev.increaseScore(50);
				dev.setLevel(dev.getLevel() + 1);
				dev.setSpeed(dev.getSpeed() + 1);
				reset(dev);
			}
		}

		if (crashed)
		{
			dev.speaker.playSound(Sound::SND_EXPLODE);
			explosion.setCoord(snake.getHead());
			gameTick = 0;
			stateSegment++;
			dev.pauseable = false;
		}
	}

	void State::tickPause(Device &dev)
	{
		if (gameTick >= gameStartPauseDurationTicks)
			stateSegment++;

		gameTick++;
	}

	void State::tickExplosion(Device &dev)
	{
		if (gameTick >= explosionDurationTicks)
		{
			if (dev.lives == 0)
			{
				nextState = GS_GAMEOVER;
			}
			else
			{
				nextState = GS_GAMEOVER_TOCURRENT;
			}

			dev.pauseable = true;
		}

		gameTick++;
	}

	void State::reset(Device &dev)
	{
		snakeTicker.reset();
		snakeTicker.setLength(snake.setSpeedBySpeed(dev.getSpeed()));
		snake.reset(dev);
		genFood(dev.getLevel() % levelCount);

		gameTick = 0;
		stateSegment = 0;
	}

	void State::genFood(int level)
	{
		bool collides = true;
		while (collides)
		{
			collides = false;

			food.x = rand() % 10;
			food.y = rand() % 20;

			collides = collides | snake.doesCollide(food);

			if (levels[level % levels.size()][food.y * 10 + food.x] != ' ')
			{
				collides = collides | true;
			}
		}

		foodTicker.setState(true, 2);
	}

	void State::render(Device &dev)
	{
		dev.screen.mainArray.clear();
		dev.screen.score.setNumber(dev.score);
		dev.screen.highScore.setNumber(dev.highScore[dev.highScoreLetter]);

		dev.screen.hintArray.clear();
		dev.screen.hintArray.setCount(dev.lives);

		dev.screen.speed.setNumber(dev.getSpeed());
		dev.screen.level.setNumber(dev.getLevel());

		switch (stateSegment)
		{
		case 0:
			renderSnake(dev);
			break;
		case 1:
			renderSnake(dev);
			break;
		case 2:
			renderSnake(dev);
			explosion.render(dev);
			break;
		}
	}

	void State::renderSnake(Device &dev)
	{
		dev.screen.mainArray.copyString(0, 0, levels[dev.getLevel() % levelCount], 10, 20);

		snake.render(dev);

		foodTicker.tick();
		// draw food
		if (foodTicker.triggered())
		{
			foodTicker.reset();
		}

		if (foodTicker.getState(2) == true)
		{
			dev.screen.mainArray.setPixel(food, PXARRAY_ON);
		}

	}
}

