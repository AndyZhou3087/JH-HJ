#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <cocos2d.h>
#include "global.h"

class GameManager
{
public:
	static GameManager * getInstance(); 
	void start();
	void pause();
	void resume();
	void over();
	void overWithoutNotice();
	void restart();
	void relive();
	void next();
	void ready(int level);

	bool isStarted();
	bool isPaused();
	bool isOver();

	void addScore(int score);

	void selectCross(bool selected);
	inline bool isCrossSelected() { return m_bSelectCross; }

private:
	GameManager();
	~GameManager();
	bool init();

private:
	bool	m_bGameStarted;
	bool	m_bGamePaused;
	bool	m_bGameOver;

	bool m_bSelectCross;
};

#endif

