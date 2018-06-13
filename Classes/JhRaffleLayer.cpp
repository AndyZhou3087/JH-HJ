#include "JhRaffleLayer.h"
#include "JhConst.h"
#include "JhGameDataSave.h"
#include "JhSoundManager.h"
#include "JhAnalyticUtil.h"
#include "JhCommonFuncs.h"
#include "JhWaitingProgress.h"
#include "JhHintBox.h"
#include "MD5.h"
#include "JhRaffleResultLayer.h"

JhRaffleLayer::JhRaffleLayer()
{
}


JhRaffleLayer::~JhRaffleLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}

JhRaffleLayer* JhRaffleLayer::create()
{
	JhRaffleLayer *pRet = new JhRaffleLayer();
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

bool JhRaffleLayer::init()
{
	Node* csbnode = CSLoader::createNode("jhraffleLayer.csb");
	this->addChild(csbnode);

	backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhRaffleLayer::onBack, this));

	JhGlobalData::myRaffleData.iscanplay = false;
	JhGlobalData::myRaffleData.mywingold = -1;
	JhGlobalData::myRaffleData.mywinrank = 0;
	JhGlobalData::myRaffleData.curstage = "";

	joinbtn = (cocos2d::ui::Button*)csbnode->getChildByName("joinbtn");
	joinbtn->addTouchEventListener(CC_CALLBACK_2(JhRaffleLayer::onJoin, this));
	joinbtn->setEnabled(false);

	mygoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("mygoldlbl");

	poolnum = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("poolnum");

	lefttimelbl = (cocos2d::ui::Text*)csbnode->getChildByName("time");
	prizebox = (cocos2d::ui::Widget*)csbnode->getChildByName("prizebox");
	for (int i = 0; i < 3; i++)
	{
		std::string namestr = StringUtils::format("name%d", i + 1);
		winnames[i] = (cocos2d::ui::Text*)prizebox->getChildByName(namestr);
		namestr = StringUtils::format("winnum%d", i + 1);
		wingolds[i] = (cocos2d::ui::Text*)prizebox->getChildByName(namestr);
	}
	prizebox->setVisible(false);

	if (JhGlobalData::g_gameStatus == GAMESTART)
		JhGlobalData::g_gameStatus = GAMEPAUSE;

	refreshGoldCount(0);
	this->schedule(schedule_selector(JhRaffleLayer::refreshGoldCount), 1);
	updatePool(0);
	this->schedule(schedule_selector(JhRaffleLayer::updatePool), 60);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhRaffleLayer::updatePool(float dt)
{
	actiontype = 0;

	JhWaitingProgress* waitbox = JhWaitingProgress::create("刷新中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	JhServerDataSwap::init(this)->getCoinpoolData();
}

void JhRaffleLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhRaffleLayer::refreshGoldCount(float dt)
{
	std::string countstr = StringUtils::format("%d", JhGlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void JhRaffleLayer::onJoin(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int mygold = JhGlobalData::getMyGoldCount();
		if (mygold >= 20)
		{
			if (JhGlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				JhGlobalData::dataIsModified = true;
				JhGlobalData::setMyGoldCount(0);
				JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}
		}
		else
		{
			JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("金元宝不足！"));
			this->addChild(hbox, 0, "hintbox");
			return;
		}
		actiontype = 1;
		JhWaitingProgress* waitbox = JhWaitingProgress::create("投注中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		JhServerDataSwap::init(this)->playCoinpoolData();
	}
}

void JhRaffleLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	if (actiontype == 0)
	{
		int leftmin = JhGlobalData::myRaffleData.leftime % 60 == 0 ? JhGlobalData::myRaffleData.leftime / 60 : (JhGlobalData::myRaffleData.leftime / 60 + 1);

		if (leftmin < 10)
			lefttimelbl->setTextColor(Color4B(204, 4, 4, 255));

		std::string str = StringUtils::format("%d", leftmin);
		lefttimelbl->setString(str);

		str = StringUtils::format("%d", JhGlobalData::myRaffleData.poolgold);
		poolnum->setString(str);

		if (JhGlobalData::myRaffleData.iscanplay)
		{
			joinbtn->setEnabled(true);
			joinbtn->setTitleText(JhCommonFuncs::gbk2utf("参与"));
			joinbtn->setTitleFontSize(40);
		}
		else
		{
			joinbtn->setEnabled(false);
			joinbtn->setTitleText(JhCommonFuncs::gbk2utf("本期已参与"));
			joinbtn->setTitleFontSize(28);
		}

		for (unsigned int i = 0; i < JhGlobalData::myRaffleData.vec_nicknames.size(); i++)
		{
			winnames[i]->setString(JhGlobalData::myRaffleData.vec_nicknames[i]);
			prizebox->setVisible(true);
		}
		for (unsigned int i = 0; i < JhGlobalData::myRaffleData.vec_wingold.size(); i++)
		{
			std::string resstr = StringUtils::format("%d", JhGlobalData::myRaffleData.vec_wingold[i]);
			wingolds[i]->setString(resstr);
		}

		if (JhGlobalData::myRaffleData.mywingold > 0)
		{
			JhRaffleResultLayer* rrlayer = JhRaffleResultLayer::create();
			this->addChild(rrlayer, 100);
			JhGameDataSave::getInstance()->setRaffleStage("");
		}
		else
		{
			std::string myrafflestage = JhGameDataSave::getInstance()->getRaffleStage();

			if (myrafflestage.length() > 0 && myrafflestage.compare(JhGlobalData::myRaffleData.curstage) != 0)
			{
				JhGameDataSave::getInstance()->setRaffleStage("");
				std::string str = StringUtils::format("您参与的第%s期未中奖，祝下次好运！", JhGlobalData::myRaffleData.mywinstage.c_str());
				JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf(str.c_str()));
				this->addChild(hbox, 10);
			}
		}
		JhGlobalData::myRaffleData.mywingold = -1;
	}
	else
	{
		std::string str = StringUtils::format("%d", JhGlobalData::myRaffleData.poolgold);
		poolnum->setString(str);

		JhGameDataSave::getInstance()->setRaffleStage(JhGlobalData::myRaffleData.curstage);

		JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - 20);
		JhGlobalData::myRaffleData.iscanplay = false;
		joinbtn->setEnabled(false);
		joinbtn->setTitleText(JhCommonFuncs::gbk2utf("本期已参与"));
		joinbtn->setTitleFontSize(28);
	}
	actiontype = 0;
}

void JhRaffleLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	std::string str = "数据异常，请稍后重试或者联系客服！";
	JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf(str.c_str()));
	this->addChild(hbox, 10);

	actiontype = 0;
}