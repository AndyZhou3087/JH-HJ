#include "JhPlayerChallengeResultLayer.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhConst.h"
#include "JhSoundManager.h"
#include "JhGameDataSave.h"
#include "JhRankLayer.h"

JhPlayerChallengeResultLayer::JhPlayerChallengeResultLayer()
{
	JhGlobalData::isPlayerChallenging = false;
	JhGameDataSave::getInstance()->setPlayerChallengeData("");
}


JhPlayerChallengeResultLayer::~JhPlayerChallengeResultLayer()
{

}


JhPlayerChallengeResultLayer* JhPlayerChallengeResultLayer::create(RankData* fightPlayerData, int win)
{
	JhPlayerChallengeResultLayer *pRet = new JhPlayerChallengeResultLayer();
	if (pRet && pRet->init(fightPlayerData, win))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool JhPlayerChallengeResultLayer::init(RankData* fightPlayerData, int win)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhplayerChallengeResultLayer.csb");
	m_csbnode->setPositionY(100);
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhPlayerChallengeResultLayer::onBack, this));

	std::string str = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("heroimg");
	headimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/wintitle%d.png", win);
	cocos2d::ui::ImageView* wintitle = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintitle");
	wintitle->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	/*str = StringUtils::format("ui/winicon%d.png", win);
	cocos2d::ui::ImageView* winicon = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winicon");
	winicon->loadTexture(str, cocos2d::ui::TextureResType::PLIST);*/

	str = StringUtils::format("ui/winbox%d.png", win);
	cocos2d::ui::ImageView* winbox = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("winbox");
	winbox->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	str = StringUtils::format("ui/wintext%d.png", win);
	cocos2d::ui::ImageView* wintext = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("wintext");
	wintext->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	if (win == 1)
	{
		winbox->setPosition(Vec2(359, 382));
	}
	else{
		winbox->setPosition(Vec2(359, 307));
		wintext->setContentSize(Size(228, 56));
	}

	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("explbl");

	ranknum = (cocos2d::ui::Text*)m_csbnode->getChildByName("ranknum");
	str = StringUtils::format("%d", JhGlobalData::myrank);

	ranknum->setString(str);

	int lastrank = JhGlobalData::myrank;
	/*cocos2d::ui::Text* rankup = (cocos2d::ui::Text*)m_csbnode->getChildByName("rankup");
	rankup->setVisible(false);*/
	if (win == 1)
	{
		explbl->setString("+3");
		if (fightPlayerData->rank < JhGlobalData::myrank)
		{
			//str = StringUtils::format("%d", fightPlayerData->rank);
			//ranknum->setString(str);
			_myrank = fightPlayerData->rank;
			_visualmyrank = JhGlobalData::myrank;
			std::string upstr = StringUtils::format("%d", JhGlobalData::myrank - fightPlayerData->rank);
			/*rankup->setString(upstr);
			rankup->setVisible(true);*/
			JhGlobalData::myrank = fightPlayerData->rank;
			this->scheduleOnce(schedule_selector(JhPlayerChallengeResultLayer::delayShowRank), 1.0f);
		}
		JhGlobalData::myFihgtexp += 3;
	}
	else
	{
		explbl->setString("-1");
		JhGlobalData::myFihgtexp -= 1;
		//rankup->setVisible(false);
		//winicon->setVisible(false);
	}

	JhGlobalData::myFihgtCount--;
	
	
	JhServerDataSwap::init(NULL)->getChallengeResult(lastrank, fightPlayerData->playerid, fightPlayerData->herotype, fightPlayerData->rank, win);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void JhPlayerChallengeResultLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		JhRankLayer* rlayer = (JhRankLayer*)g_gameLayer->getChildByName("ranklayer");
		if (rlayer != NULL)
			rlayer->getRankData(2);
	}
}

void JhPlayerChallengeResultLayer::onSuccess()
{
	
}

void JhPlayerChallengeResultLayer::onErr(int errcode)
{
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

void JhPlayerChallengeResultLayer::showRank(float dt)
{

	float step = (_myrank - _visualmyrank) * 0.2f;
	if (fabs(step) < 0.21)
	{
		step = 0.21 * ((step > 0) ? 1 : -1);
	}
	_visualmyrank += step;

	if (fabs(_myrank - _visualmyrank) < 1.0f) {
		_visualmyrank = _myrank;
		this->unschedule(schedule_selector(JhPlayerChallengeResultLayer::showRank));
	}

	std::string rankstr = StringUtils::format("%d", (int)_visualmyrank);
	ranknum->setString(rankstr);

}

void JhPlayerChallengeResultLayer::delayShowRank(float dt)
{
	this->schedule(schedule_selector(JhPlayerChallengeResultLayer::showRank), 1.0f / 30);

}