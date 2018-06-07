#ifndef _CHALLENGERESULT_LAYER_H_
#define _CHALLENGERESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhServerDataSwap.h"
#include "JhGlobalData.h"
USING_NS_CC;
class JhPlayerChallengeResultLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhPlayerChallengeResultLayer();
	~JhPlayerChallengeResultLayer();

	bool init(RankData* fightPlayerData, int win);
	static JhPlayerChallengeResultLayer* create(RankData* fightPlayerData, int win);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onSuccess();
	void onErr(int errcode);
	void showRank(float dt);
	void delayShowRank(float dt);
private:
	Node* m_csbnode;
	float _myrank;
	float _visualmyrank;
	cocos2d::ui::Text* ranknum;
};
#endif