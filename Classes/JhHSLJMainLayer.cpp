#include "JhHSLJMainLayer.h"
#include "JhCommonFuncs.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhFactionCreateLayer.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhMatchFightLayer.h"
#include "JhAddFightCountLayer.h"
#include "JhHSLJRewardDescLayer.h"
#include "JhHSLJRankLayer.h"
#include "JhHSLJRewardLayer.h"
#include "JhGameDataSave.h"

JhHSLJMainLayer::JhHSLJMainLayer()
{
	datatype = 0;
	changenexthero = 1;
}


JhHSLJMainLayer::~JhHSLJMainLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}


JhHSLJMainLayer* JhHSLJMainLayer::create()
{
	JhHSLJMainLayer *pRet = new JhHSLJMainLayer();
	if (pRet && pRet->init())
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

bool JhHSLJMainLayer::init()
{
	Node* csbnode = CSLoader::createNode("jhHSLJMainLayer.csb");
	this->addChild(csbnode);

	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJMainLayer::onBack, this));

	m_matchbtn = (cocos2d::ui::Button*)csbnode->getChildByName("findbtn");
	m_matchbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJMainLayer::onMacth, this));
	m_matchbtn->setEnabled(false);

	cocos2d::ui::Widget* addbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("addbtn");
	addbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJMainLayer::onAddCount, this));

	cocos2d::ui::Widget* rewarddescbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("awarddescbtn");
	rewarddescbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJMainLayer::onRewardDesc, this));

	cocos2d::ui::Widget* ranbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("ranbtn");
	ranbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJMainLayer::onRank, this));

	heroNode = (cocos2d::ui::Widget*)csbnode->getChildByName("heronode");
	heroNode->setVisible(false);

	m_hinttext = (cocos2d::ui::Text*)csbnode->getChildByName("hinttext");

	m_time = (cocos2d::ui::Text*)csbnode->getChildByName("time");
	m_time->setString("");

	m_matchno = (cocos2d::ui::Text*)csbnode->getChildByName("cccount");
	m_matchno->setString("");

	m_matchwincount = (cocos2d::ui::Text*)csbnode->getChildByName("matchwincount");
	m_matchwincount->setString("");

	cocos2d::ui::Text* mynicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("mynickname");
	mynicknamelbl->setString(JhGlobalData::getMyNickName());
	
	cocos2d::ui::ImageView* myheroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("myheroimg");
	std::string headimgstr = StringUtils::format("images/hsljhero%d.png", g_hero->getHeadID());
	myheroimg->loadTexture(headimgstr, cocos2d::ui::Widget::TextureResType::LOCAL);

	m_mydw = (cocos2d::ui::Text*)csbnode->getChildByName("mydw");
	m_mydw->setString("");
	
	m_mywinpercent = (cocos2d::ui::Text*)csbnode->getChildByName("mywin");
	m_mywinpercent->setString("");

	m_progresstext = (cocos2d::ui::Text*)csbnode->getChildByName("progresstext");
	m_progresstext->setString("");

	m_expbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("expbar");
	m_expbar->setPercent(0);

	m_fightcount = (cocos2d::ui::Text*)csbnode->getChildByName("fightcount");
	m_fightcount->setString("5/5");

	m_herodw = (cocos2d::ui::Text*)heroNode->getChildByName("herodw");
	m_herowinpercent = (cocos2d::ui::Text*)heroNode->getChildByName("herowin");
	m_heroname = (cocos2d::ui::Text*)heroNode->getChildByName("heronickname");
	
	m_playerheadimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");

	std::string mplayerid = JhGameDataSave::getInstance()->getHsljMatchPlayer();
	if (mplayerid.length() > 0)
		datatype = 2;

	getMyMatchInfo();

	JhGlobalData::g_gameStatus = GAMEPAUSE;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhHSLJMainLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhHSLJMainLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhAddFightCountLayer* layer = JhAddFightCountLayer::create(1);
		g_gameLayer->addChild(layer, 5);
	}
}

void JhHSLJMainLayer::onMacth(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (JhGlobalData::myMatchInfo.leftcount <= 0)
		{
			JhAddFightCountLayer* alayer = JhAddFightCountLayer::create(1);
			Director::getInstance()->getRunningScene()->addChild(alayer, 1);
			return;
		}
		datatype = 1;
		m_matchbtn->setEnabled(false);
		m_backbtn->setEnabled(false);
		m_hinttext->setString(JhCommonFuncs::gbk2utf("匹配中..."));
		JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		JhServerDataSwap::init(this)->getMatchFight();
		changeHeroImg(0);
		this->schedule(schedule_selector(JhHSLJMainLayer::changeHeroImg), 0.3f);
	}
}

void JhHSLJMainLayer::onRewardDesc(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhHSLJRewardDescLayer* layer = JhHSLJRewardDescLayer::create();
		this->addChild(layer);
	}
}

void JhHSLJMainLayer::onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhHSLJRankLayer* layer = JhHSLJRankLayer::create();
		g_gameLayer->addChild(layer,5, "shljranklayer");
	}
}

void JhHSLJMainLayer::getMyMatchInfo()
{
	JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	if (datatype == 2)
	{
		JhServerDataSwap::init(this)->getMatchFightResult(JhGameDataSave::getInstance()->getHsljMatchPlayer(), -13);
	}
	else if (datatype == 0)
	{
		JhServerDataSwap::init(this)->getMyMatchInfo();
	}
}

void JhHSLJMainLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (datatype == 0)
	{
		m_matchbtn->setEnabled(true);
		showMyInfo();
	}
	else if (datatype == 1)
	{
		if (JhGlobalData::vec_matchPlayerData.size() > 0)
		{
			showMatchInfo();
			int r = JhGlobalData::createRandomNum(3);
			this->scheduleOnce(schedule_selector(JhHSLJMainLayer::stopChangeHeroImg), r + 1);
		}
		else
		{
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("匹配失败！！请重新匹配！"));
			this->addChild(box);
			m_matchbtn->setEnabled(true);
			m_backbtn->setEnabled(true);
			m_hinttext->setString(JhCommonFuncs::gbk2utf("请匹配你的对手..."));
			this->unschedule(schedule_selector(JhHSLJMainLayer::changeHeroImg));
			showPlayerHeadImg(0);
		}
	}
	else if (datatype == 2)
	{
		JhGameDataSave::getInstance()->setHsljMatchPlayer("");
		datatype = 0;
		getMyMatchInfo();
	}
}

void JhHSLJMainLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (datatype != 2)
	{
		if (datatype == 0 && errcode == -3)
		{
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据异常！！"));
			this->addChild(box);
			return;
		}

		std::string desc = "数据获取异常，请检查网络连接！！";
		if (datatype == 1 && errcode == -3)
		{
			desc = "匹配失败，暂时无对手数据，请稍后重试！！";
		}
		JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf(desc.c_str()));
		this->addChild(box);
		if (datatype == 1)
		{
			m_matchbtn->setEnabled(true);
			m_backbtn->setEnabled(true);
			m_hinttext->setString(JhCommonFuncs::gbk2utf("请匹配你的对手..."));
			this->unschedule(schedule_selector(JhHSLJMainLayer::changeHeroImg));
			showPlayerHeadImg(0);
		}
		datatype = 0;
	}
	else
	{
		datatype = 0;
		getMyMatchInfo();
	}
}

void JhHSLJMainLayer::showMyInfo()
{
	std::string str = StringUtils::format("%s至%s", JhGlobalData::myMatchInfo.starttime.c_str(), JhGlobalData::myMatchInfo.endtime.c_str());
	m_time->setString(JhCommonFuncs::gbk2utf(str.c_str()));

	std::string progressstr;
	int barpercent = 0;
	if (JhGlobalData::myMatchInfo.myexp <= 100)
	{
		progressstr = StringUtils::format("%d/100", JhGlobalData::myMatchInfo.myexp);
		barpercent = JhGlobalData::myMatchInfo.myexp * 100 / 100;
	}
	else if (JhGlobalData::myMatchInfo.myexp <= 300)
	{
		progressstr = StringUtils::format("%d/300", JhGlobalData::myMatchInfo.myexp);
		barpercent = JhGlobalData::myMatchInfo.myexp * 100 / 300;
	}
	else if (JhGlobalData::myMatchInfo.myexp <= 700)
	{
		progressstr = StringUtils::format("%d/700", JhGlobalData::myMatchInfo.myexp);
		barpercent = JhGlobalData::myMatchInfo.myexp * 100 / 700;
	}
	else if (JhGlobalData::myMatchInfo.myexp <= 1000)
	{
		progressstr = StringUtils::format("%d/1000", JhGlobalData::myMatchInfo.myexp);
		barpercent = JhGlobalData::myMatchInfo.myexp * 100 / 1000;
	}
	else
	{
		progressstr = StringUtils::format("%d/1000", 1000);
		barpercent = 100;
	}
	str = getDwStr(JhGlobalData::myMatchInfo.myexp);
	m_mydw->setString(JhCommonFuncs::gbk2utf(str.c_str()));

	int percent = 0;
	int totalcount = JhGlobalData::myMatchInfo.mywincount + JhGlobalData::myMatchInfo.myfailcount;
	if (totalcount > 0)
	{
		percent = JhGlobalData::myMatchInfo.mywincount * 100 / totalcount;
	}
	str = StringUtils::format("%d%%", percent);
	m_mywinpercent->setString(str);

	str = StringUtils::format("%d", totalcount);
	m_matchno->setString(str);

	str = StringUtils::format("%d", JhGlobalData::myMatchInfo.mywincount);
	m_matchwincount->setString(str);

	m_progresstext->setString(progressstr);
	m_expbar->setPercent(barpercent);
	
	updateMyFightCount();

	showMyReWard();
}

void JhHSLJMainLayer::updateMyFightCount()
{
	std::string str = StringUtils::format("%d/5", JhGlobalData::myMatchInfo.leftcount);
	m_fightcount->setString(str);
}

void JhHSLJMainLayer::showMatchInfo()
{
	std::string str = getDwStr(JhGlobalData::matchPlayerInfo.exp);

	m_herodw->setString(JhCommonFuncs::gbk2utf(str.c_str()));
	
	int percent = 0;
	int totalcount = JhGlobalData::matchPlayerInfo.wincount + JhGlobalData::matchPlayerInfo.failcount;
	if (totalcount > 0)
	{
		percent = JhGlobalData::matchPlayerInfo.wincount * 100 / totalcount;
	}
	str = StringUtils::format("%d%%", percent);

	m_herowinpercent->setString(str);

	m_heroname->setString(JhGlobalData::matchPlayerInfo.nickname);
}

void JhHSLJMainLayer::showMyReWard()
{
	if (JhGlobalData::myMatchInfo.matchaward > 0)
	{
		JhHSLJRewardLayer* layer = JhHSLJRewardLayer::create(JhGlobalData::myMatchInfo.matchaward);
		this->addChild(layer);
	}
}

std::string JhHSLJMainLayer::getDwStr(int exp)
{
	std::string str;
	if (exp <= 100)
	{
		str = "乡野义士";
	}
	else if (exp <= 300)
	{
		str = "江湖侠士";
	}
	else if (exp <= 700)
	{
		str = "名门侠客";
	}
	else if (exp <= 1000)
	{
		str = "一方大侠";
	}
	else
	{
		str = "墨者";
	}
	return str;
}

void JhHSLJMainLayer::showVSAnim()
{
	std::string vscsb = "jhVS.csb";
	Node* csbnode = CSLoader::createNode(vscsb);
	csbnode->setPosition(Vec2(360, 650));
	this->addChild(csbnode);
	auto action = CSLoader::createTimeline(vscsb);
	csbnode->runAction(action);
	action->gotoFrameAndPlay(0, false);
}

void JhHSLJMainLayer::stopChangeHeroImg(float dt)
{
	this->unschedule(schedule_selector(JhHSLJMainLayer::changeHeroImg));

	heroNode->setVisible(true);
	m_hinttext->setVisible(false);

	showVSAnim();

	this->scheduleOnce(schedule_selector(JhHSLJMainLayer::delayEnterFight), 1.5f);
}

void JhHSLJMainLayer::delayEnterFight(float dt)
{
	this->removeFromParentAndCleanup(true);
	JhMatchFightLayer* layer = JhMatchFightLayer::create("m1-11");
	g_gameLayer->addChild(layer, 5);
}

void JhHSLJMainLayer::changeHeroImg(float dt)
{
	if (changenexthero > 4)
		changenexthero = 1;
	
	showPlayerHeadImg(changenexthero);
	changenexthero++;
}

void JhHSLJMainLayer::showPlayerHeadImg(int herotype)
{
	std::string str = StringUtils::format("images/hsljhero%d.png", herotype);
	m_playerheadimg->setFlippedX(true);
	m_playerheadimg->setContentSize(Sprite::create(str)->getContentSize());
	m_playerheadimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::LOCAL);
}