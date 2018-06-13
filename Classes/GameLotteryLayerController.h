#ifndef __game_lottery_layer_controller_h__
#define __game_lottery_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

enum EnumCardAward {
	Card_Cross,
	Card_Gift,
	Card_time1,
	Card_time3,
};

enum EnumUiState {
	State_Free,
	State_Openall,
	State_GetAll,
	State_BuyedAll,
};

class UBigAwardLayerController;

class GameLotteryLayerController:public LayerController {
public:
	GameLotteryLayerController();
	~GameLotteryLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void regBgiAwardLayer(UBigAwardLayerController *l) { m_bigAwardLayer = l; }
	virtual void onPayAllCardOpenComplete(bool success) override;
	virtual void onPayFetchAllComplete(bool success) override;

private:
	void initFreeUi();
	void initOpenallUi();
	void initGetallUi();
	void initBuyedallUi();

	void enterFree();
	void enterOpenall();
	void enterGetall();
	void enterBuyedAll();


	void initAllCard();
	void initCard(cocos2d::Sprite *card, EnumCardAward award);
	void randomCard();
	
	void recoverFrontCard(cocos2d::Node *card);
	void recoverBackCard(cocos2d::Node *card);

	void startOrbitAction(int cardIndex, bool enableSwap = false, bool enableSwapTime3 = false);
	void selectCard(int cardIndex);
	static const char * getIconByAward(EnumCardAward award);

	//换指定卡到指定位置
	void swapGiftCard(EnumCardAward award, int index);
	void swapCard(int src, int dst);
	void checkCard(int index);
	void openAll();
	bool getAward();
	bool getAward(EnumCardAward);


private:
	cocos2d::Sprite *m_card1_front;
	Button *m_card1_back;

	cocos2d::Sprite *m_card2_front;
	Button *m_card2_back;

	cocos2d::Sprite *m_card3_front;
	Button *m_card3_back;

	cocos2d::Sprite *m_card4_front;
	Button *m_card4_back;

	Button *m_close;
	Button *m_get;
	Button *m_openall;
	Node *m_disfreeTip;

	bool m_bIsCard1ActionDone;
	bool m_bIsCard2ActionDone;
	bool m_bIsCard3ActionDone;
	bool m_bIsCard4ActionDone;

	std::vector<EnumCardAward> m_award;

	bool m_selected;
	bool m_card1Selected;
	bool m_card2Selected;
	bool m_card3Selected;
	bool m_card4Selected;

	EnumUiState m_state;

	UBigAwardLayerController *m_bigAwardLayer;
};
#endif