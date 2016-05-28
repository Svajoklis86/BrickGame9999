#include "GSArkanoid.h"

string GSArkanoid::currentLevel;
int GSArkanoid::currentCount;

GSArkanoid::GSArkanoid(Device &dev, GSArkanoidVariant variant)
{
	pauseTicker.setLength(60);
	paddleTicker.setLength(10);
	ballTicker.setLength(5);

	switch(variant)
	{
	case GSARKANOID_NORMAL:
		highScoreLetter = 'C';
		break;
	case GSARKANOID_DOUBLE:
		highScoreLetter = 'D';
		break;
	default: break;
	}

	defineLevels();

	if (!dev.inGame)
	{
		if (dev.level >= levelCount)
			dev.level = levelCount;

		dev.score = 0;
		dev.lives = 3;
	}

	dev.screen.level.setLink(&dev.level);
	dev.screen.speed.setLink(&dev.speed);
	dev.screen.score.setLink(&dev.score);
	dev.screen.highScore.setLink(&dev.highScore[highScoreLetter]);

	explosion.setCoord({ 5, 10 });

	reset(dev);

	if (!dev.inGame)
		resetLevel(dev);

	dev.inGame = true;
}

void GSArkanoid::reset(Device &dev)
{
	paddleX = 3;
	paddleDX = 0;
	ballX = 5;
	ballY = 18;
	ballDX = 1;
	ballDY = -1;
	slid = false;

	stateSegment = 0;

	pauseTicker.reset();
	paddleTicker.reset();
	ballTicker.reset();
}

void GSArkanoid::resetLevel(Device &dev)
{
	currentLevel = levels[dev.level].data;
	currentCount = levels[dev.level].count;
}

void GSArkanoid::tick(Device& dev)
{
	switch (stateSegment)
	{
	case SEG_PAUSE:
		tickPause(dev);
		break;

	case SEG_GAME:
		tickGame(dev);
		break;

	case SEG_EXPLOSION:
		tickExplosion(dev);
		break;
	}
}

void GSArkanoid::tickPause(Device &dev)
{
	if (pauseTicker.triggered())
	{
		pauseTicker.reset();
		stateSegment = SEG_GAME;
		return;
	}

	pauseTicker.tick();
}

void GSArkanoid::tickGame(Device &dev)
{
	if (paddleTicker.triggered())
	{
		paddleTicker.reset();

		if (paddleDX != 0 && ballDY == 1 && ballY == 18 && (ballX >= paddleX && ballX <= paddleX + paddleW - 1) && !slid)
		{
			ballX += paddleDX;
			if (ballX < 0)
				ballX = 0;
			else if (ballX >= 10)
				ballX = 9;

			ballDY = -ballDY;

			slid = true;
		}

		paddleX += paddleDX;
		if (paddleX < 0)
			paddleX = 0;
		if (paddleX > 10 - paddleW)
			paddleX = 10 - paddleW;

	}

	if (ballTicker.triggered())
	{
		ballTicker.reset();
		if (paddleDX != 0 && ballDY == 1 && ballY == 18 && (ballX >= paddleX && ballX <= paddleX + paddleW - 1) && !slid)
		{
			ballX += paddleDX;
			if (ballX < 0)
				ballX = 0;
			else if (ballX >= 10)
				ballX = 9;

			ballDY = -ballDY;

			slid = true;
		}
		else
		{
			if (ballX == 9 || ballX == 0)
				ballDX = -ballDX;
			if (ballY == 0 || ballY == 19)
				ballDY = -ballDY;

			// hit detection with paddle
			// block below
			if (ballY == 18 && (ballX >= paddleX && ballX <= paddleX + paddleW - 1) && (ballDY == 1))
			{
				ballDY = -ballDY;
			}
			else // diagonal from left
				if (ballY == 18 && (ballX == paddleX - 1) && (ballDX == 1) && (ballDY == 1) && !slid)
				{
					ballDY = -ballDY;
					ballDX = -ballDX;
				}
				else // diagonal from right
					if (ballY == 18 && (ballX == paddleX + paddleW) && (ballDX == -1) && (ballDY == 1) && !slid)
					{
						ballDY = -ballDY;
						ballDX = -ballDX;
					}

			int newBallDX = ballDX;
			int newBallDY = ballDY;

			do
			{
				ballDX = newBallDX;
				ballDY = newBallDY;

				// collision with level
				bool collidedWithLevel = false;
				// on y

				if (currentLevel[(ballY + ballDY) * 10 + ballX] != ' ')
				{
					currentLevel[(ballY + ballDY) * 10 + ballX] = ' ';
					currentCount--;
					dev.increaseScore(10, highScoreLetter);
					newBallDY = -ballDY;
					collidedWithLevel = true;
				}

				if (currentLevel[(ballY)* 10 + ballX + ballDX] != ' ')
				{
					currentLevel[(ballY)* 10 + ballX + ballDX] = ' ';
					currentCount--;
					dev.increaseScore(10, highScoreLetter);
					newBallDX = -ballDX;
					collidedWithLevel = true;
				}

				// only collide diagonally if clear on vertical/horizontal

				if (!collidedWithLevel && currentLevel[(ballY + ballDY) * 10 + ballX + ballDX] != ' ')
				{
					currentLevel[(ballY + ballDY) * 10 + ballX + ballDX] = ' ';
					currentCount--;
					dev.increaseScore(10, highScoreLetter);
					newBallDY = -ballDY;
					newBallDX = -ballDX;
				}

			} while (newBallDX != ballDX || newBallDY != ballDY);

			ballDX = newBallDX;
			ballDY = newBallDY;

			if (ballX + ballDX < 0 || ballX + ballDX >= 10)
				ballDX = -ballDX;

			if (ballY + ballDY < 0 || ballY + ballDY >= 20)
				ballDY = -ballDY;

			ballX += ballDX;
			ballY += ballDY;

			slid = false;

			if (currentCount == 0)
			{
				dev.level++;
				resetLevel(dev);
				reset(dev);
			}

			if (ballY == 19)
			{
				dev.lives--;
				explosion.setCoord({ ballX, ballY });
				stateSegment = SEG_EXPLOSION;
			}
		}

	}

	paddleTicker.tick();
	ballTicker.tick();
}

void GSArkanoid::tickExplosion(Device &dev)
{
	if (pauseTicker.triggered())
	{
		pauseTicker.reset();
		if (dev.lives > 0)
		{
			nextState = GS_GAMEOVER_TOCURRENT;
			stateSegment = 0;
		}
		else
		{
			nextState = GS_GAMEOVER;
		}
		
		return;
	}

	pauseTicker.tick();
}

void GSArkanoid::parseEvent(Device& dev, Key k, KeyState state)
{
	if (state == STATE_DOWN)
	{
		switch (k)
		{
		case KEY_LEFT:
			paddleTicker.forceTrigger();
			paddleDX = -1;
			break;
		case KEY_RIGHT:
			paddleTicker.forceTrigger();
			paddleDX = 1;
			break;
		}
	}

	if (state == STATE_UP)
	{
		switch (k)
		{
		case KEY_LEFT:
		case KEY_RIGHT:
			paddleDX = 0;
			break;
		}
	}
}

void GSArkanoid::render(Device& dev)
{
	dev.screen.mainArray.clear();
	dev.screen.hintArray.setCount(dev.lives);

	dev.screen.level.setLinked();
	dev.screen.speed.setLinked();

	dev.screen.score.setLinked();
	dev.screen.highScore.setLinked();
	
	switch (stateSegment)
	{
	case SEG_PAUSE:
		renderGame(dev);
		break;
	case SEG_GAME:
		renderGame(dev);
		break;
	case SEG_EXPLOSION:
		renderGame(dev);
		explosion.render(dev);
		break;
	}
}

void GSArkanoid::renderGame(Device& dev)
{
	string paddle = "**********";

	dev.screen.mainArray.copyString(paddleX, 20 - 1, paddle, paddleW, 1);

	dev.screen.mainArray.setPixel(ballX, ballY, ON);

	dev.screen.mainArray.copyString(0, 0, currentLevel, 10, 20);
}


GSArkanoid::~GSArkanoid()
{
	
}

int countStarsInData(string data)
{
	int count = 0;
	for (auto it = data.begin(); it != data.end(); it++)
	{
		if (*it == '*') count++;
	}

	return count;
}

void GSArkanoid::defineLevels()
{
	levels[levelCount].data =
		"          "
		"          "
		"          "
		" ******** "
		"  ******  "
		"  ******  "
		" ******** "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          ";
	levels[levelCount].count = countStarsInData(levels[levelCount].data);
	levelCount++;

	levels[levelCount].data =
		"  *   *  *"
		"  * *  *  "
		"*   * *   "
		" ******** "
		"  ******  "
		"  ******  "
		" ******** "
		"   * * *  "
		"     * *  "
		"  **   *  "
		"    **    "
		"      *   "
		" *        "
		"  *    *  "
		"   *      "
		"      *   "
		"          "
		"          "
		"          "
		"          ";
	levels[levelCount].count = countStarsInData(levels[levelCount].data);
	levelCount++;

}
