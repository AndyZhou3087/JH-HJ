#ifndef __game_top_layer_h__
#define __game_top_layer_h__

#include "LayerController.h"
#include "cocos2d.h"
#include "prefix.h"

class GameTopLayerController:public LayerController {
public:
	GameTopLayerController();
	~GameTopLayerController();

	virtual void initial();
	virtual void enter() override;
	virtual void exit() override;

	virtual void onGameStart() override;
	virtual void onGameRestart() override;
	virtual void onGameReady() override;

	void onScoreChange();
	void on1sTimer();
	void onTimeChange();

	inline void stopTime() { m_bStop = true; }
	inline void startTime() { m_bStop = false; }

private:
	void initTime();
	void updateTime();
	void updateInfo();
	void updateTimeBar();
	void initTimeBar();

private:
	TextAtlas *m_numScore;
	TextAtlas *m_numTarget;
	TextAtlas *m_numTime;
	cocos2d::Sprite *m_timeBar;
	bool m_bStop;
	
};

#endif