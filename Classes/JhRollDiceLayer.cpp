#include "JhRollDiceLayer.h"
#include "JhConst.h"

#include "JhSoundManager.h"
#include "JhAnalyticUtil.h"
#include "JhCommonFuncs.h"
#include "JhWaitingProgress.h"
#include "JhHintBox.h"
#include "MD5.h"
#include "JhShopLayer.h"

JhRollDiceLayer::JhRollDiceLayer()
{
}


JhRollDiceLayer::~JhRollDiceLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}

JhRollDiceLayer* JhRollDiceLayer::create()
{
	JhRollDiceLayer *pRet = new JhRollDiceLayer();
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

bool JhRollDiceLayer::init()
{
	Node* csbnode = CSLoader::createNode("jhrollDiceLayer.csb");
	this->addChild(csbnode);

	backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhRollDiceLayer::onBack, this));

	rollbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("getbtn");
	rollbtn->addTouchEventListener(CC_CALLBACK_2(JhRollDiceLayer::onRoll, this));
	
	mygoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("mygoldlbl");

	countlbl = (cocos2d::ui::Text*)csbnode->getChildByName("count");

	counttext = (cocos2d::ui::Text*)csbnode->getChildByName("counttext");

	costgoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("constgoldlbl");
	costgoldlbl->setVisible(false);

	costgold = (cocos2d::ui::Widget*)csbnode->getChildByName("constgold");
	costgold->setVisible(false);

	word = (cocos2d::ui::Text*)csbnode->getChildByName("word");

	animnode = CSLoader::createNode("jhrollDiceAnim.csb");
	animnode->setPosition(Vec2(360, 800));
	csbnode->addChild(animnode);
	if (JhGlobalData::g_gameStatus == GAMESTART)
		JhGlobalData::g_gameStatus = GAMEPAUSE;

	refreshGoldCount(0);
	this->schedule(schedule_selector(JhRollDiceLayer::refreshGoldCount), 1);
	actiontype = 0;

	JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	JhServerDataSwap::init(this)->getlotteryData(actiontype);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhRollDiceLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhRollDiceLayer::refreshGoldCount(float dt)
{
	std::string countstr = StringUtils::format("%d", JhGlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void JhRollDiceLayer::onRoll(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		word->setString(JhCommonFuncs::gbk2utf("小宝为了回馈江湖兄弟，特举办保底掷骰子拿元宝，点数既是倍数，点数越高拿的越高！！！"));
		if (JhGlobalData::myLotteryData.leftcount <= 0)
		{
			JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("每人最多只能参与六轮，不然小宝我就算再有钱也不够赔的啦~"));
			this->addChild(hbox, 10);
			return;
		}

		int mygold = JhGlobalData::getMyGoldCount();

		if (mygold >= useGold)
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
		this->removeChildByName("dice");
		animnode->getChildByName("node")->getChildByName("cover")->setVisible(true);
		anim_action = CSLoader::createTimeline("jhrollDiceAnim.csb");
		animnode->runAction(anim_action);
		anim_action->gotoFrameAndPlay(0, true);
		actiontype = 1;
		backbtn->setEnabled(false);
		rollbtn->setEnabled(false);
		JhServerDataSwap::init(this)->getlotteryData(actiontype);
	}
}

void JhRollDiceLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	std::string str = StringUtils::format("%d", JhGlobalData::myLotteryData.nextcostgold);
	costgoldlbl->setString(str);
	const std::string countcnstr[] = { "一", "二", "三", "四", "五", "六", "六" };
	str = StringUtils::format("%s", JhCommonFuncs::gbk2utf(countcnstr[6 - JhGlobalData::myLotteryData.leftcount].c_str()).c_str());
	countlbl->setString(str);
	setUiVisual(true);
	if (actiontype == 1)
	{
		JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - useGold);
		winbs = JhGlobalData::myLotteryData.wingold / useGold;
		int r = JhGlobalData::createRandomNum(3);

		this->scheduleOnce(schedule_selector(JhRollDiceLayer::showResult), r + 1.0f);
	}

	if (JhGlobalData::myLotteryData.leftcount <= 0)
	{
		costgoldlbl->setVisible(false);
		costgold->setVisible(false);
	}
	useGold = JhGlobalData::myLotteryData.nextcostgold;
	actiontype = 0;
}

void JhRollDiceLayer::onErr(int errcode)
{
	animnode->stopAllActions();
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	std::string str = "数据异常，请稍后重试或者联系客服！";
	if (errcode == -3)
	{
		str = "次数已达限制";
		setUiVisual(false);
	}
	backbtn->setEnabled(true);
	rollbtn->setEnabled(true);
	JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf(str.c_str()));
	this->addChild(hbox, 10);
	actiontype = 0;

}

void JhRollDiceLayer::setUiVisual(bool val)
{
	counttext->setVisible(val);
	costgoldlbl->setVisible(val);
	costgold->setVisible(val);
	if (val)
		backbtn->setPositionX(160);
	else
		backbtn->setPositionX(360);
	rollbtn->setVisible(val);
}

void JhRollDiceLayer::showResult(float dt)
{
	backbtn->setEnabled(true);
	rollbtn->setEnabled(true);

	anim_action = CSLoader::createTimeline("jhrollDiceAnim.csb");
	animnode->runAction(anim_action);
	anim_action->gotoFrameAndPlay(0, 0, false);
	animnode->stopAllActions();

	this->scheduleOnce(schedule_selector(JhRollDiceLayer::openDice), 1.0f);
}

void JhRollDiceLayer::openDice(float dt)
{
	animnode->getChildByName("node")->getChildByName("cover")->setVisible(false);
	std::string dicestr = StringUtils::format("ui/dice%d.png", winbs);

	Sprite* dicesprite = Sprite::createWithSpriteFrameName(dicestr);
	dicesprite->setPosition(animnode->getPosition());
	this->addChild(dicesprite, 1, "dice");

	std::string str = StringUtils::format("恭喜兄弟掷出%d点，获得%d倍元宝，共%d，再接再厉！！！", winbs, winbs, JhGlobalData::myLotteryData.wingold);
	word->setString(JhCommonFuncs::gbk2utf(str.c_str()));
	JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() + JhGlobalData::myLotteryData.wingold);
}