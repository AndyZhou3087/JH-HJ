#include "JhRankLayer.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhHintBox.h"
#include "WaitingProgress.h"
#include "JhConst.h"
#include "SoundManager.h"
#include "JhPlayerChallengeLayer.h"
#include "JhAddFightCountLayer.h"
#include "JhGameDataSave.h"

JhRankLayer::JhRankLayer()
{
	isLastFight = false;
}


JhRankLayer::~JhRankLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}


JhRankLayer* JhRankLayer::create()
{
	JhRankLayer *pRet = new JhRankLayer();
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

bool JhRankLayer::init()
{
	m_csbnode = CSLoader::createNode("jhrankLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhRankLayer::onBack, this));

	tag0btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("tagbtn0");
	tag0btn->addTouchEventListener(CC_CALLBACK_2(JhRankLayer::onRank, this));
	tag0btn->setTag(0);
	tag0btn->setBright(false);

	tag1btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("tagbtn1");
	tag1btn->addTouchEventListener(CC_CALLBACK_2(JhRankLayer::onRank, this));
	tag1btn->setTag(1);
	tag1btn->setBright(true);

	tag2btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("tagbtn2");
	tag2btn->addTouchEventListener(CC_CALLBACK_2(JhRankLayer::onRank, this));
	tag2btn->setTag(2);
	tag2btn->setBright(true);

	srollView = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	//rankcatabox0 = (cocos2d::ui::Widget*)m_csbnode->getChildByName("rankcatabox0");

	//rankcatabox1 = (cocos2d::ui::Widget*)m_csbnode->getChildByName("rankcatabox1");

	//rankcatabox0->setVisible(true);
	//rankcatabox1->setVisible(false);

	//text6 = (cocos2d::ui::Text*) rankcatabox0->getChildByName("text6");
	text3 = (cocos2d::ui::Text*) m_csbnode->getChildByName("text3");
	text4 = (cocos2d::ui::Text*) m_csbnode->getChildByName("text4");

	myFihgtNode = (cocos2d::ui::Widget*)m_csbnode->getChildByName("mynode");
	myFihgtNode->setVisible(false);

	JhGlobalData::g_gameStatus = GAMEPAUSE;

	selectrank = 0;
	myfightingpower = 0;

	getRankData(0);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhRankLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhRankLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhRankLayer::onRank(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Button* btnnode = (cocos2d::ui::Button*)pSender;
		isLastFight = false;

		btnnode->setBright(false);
		int tag = btnnode->getTag();

		if (selectrank == tag)
			return;

		if (tag == 0)
		{
			//rankcatabox0->setVisible(true);
			//rankcatabox1->setVisible(false);
			tag1btn->setBright(true);
			tag2btn->setBright(true);
			text4->setString(JhCommonFuncs::gbk2utf("天数"));
			text3->setString(JhCommonFuncs::gbk2utf("性别"));
		}
		else if (tag == 1)
		{
			//rankcatabox0->setVisible(true);
			//rankcatabox1->setVisible(false);
			tag0btn->setBright(true);
			tag2btn->setBright(true);
			text4->setString(JhCommonFuncs::gbk2utf("战斗力"));
			text3->setString(JhCommonFuncs::gbk2utf("性别"));
		}
		else if (tag == 2)
		{
			//rankcatabox0->setVisible(false);
			//rankcatabox1->setVisible(true);
			tag0btn->setBright(true);
			tag1btn->setBright(true);
			text3->setString(JhCommonFuncs::gbk2utf("战斗力"));
			text4->setString(JhCommonFuncs::gbk2utf("挑战"));
		}

		selectrank = btnnode->getTag();
		getRankData(selectrank);
	}
}

void JhRankLayer::getRankData(int type)
{
	WaitingProgress* waitbox = WaitingProgress::create("排名中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	myfightingpower = 0;

	std::string orderstr = StringUtils::format("days=%d", g_nature->getPastDays());
	if (type == 1)
	{
		if (g_hero != NULL)
		{
			myfightingpower = g_hero->getMaxLifeValue() + g_hero->getTotalDf() * 20 + g_hero->getTotalAtck() * 10 + g_hero->getCritRate() * 100 + g_hero->getdodgeRate() * 100;
		}
		orderstr = StringUtils::format("fightingpower=%d", myfightingpower);
	}

	if (myfightingpower >= 1000000)
		myfightingpower = 30000;
	std::string addtypestr = StringUtils::format("&ranktype=%d", type);
	orderstr.append(addtypestr);

	if (type == 2)
	{
		ServerDataSwap::init(this)->getChallengeranklist();
	}
	else
	{
		ServerDataSwap::init(this)->getRankData(orderstr);
	}
}

void JhRankLayer::delayShowData(float dt)
{
	int rankitemtype = 0;

	int contentheight = 700;
	int scrolly = 182;

	if (selectrank == 2)
	{
		rankitemtype = 1;
		contentheight = 650;
		scrolly = 238;
	}

	this->unschedule(schedule_selector(JhRankLayer::updateFightCount));

	srollView->removeAllChildrenWithCleanup(true);
	int size = 100;// JhGlobalData::vec_rankData.size();

	int itemheight = 90;
	int innerheight = itemheight * size;

	srollView->setContentSize(Size(srollView->getContentSize().width, contentheight));
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));
	srollView->setPositionY(scrolly);

	for (unsigned int i = 0; i < size; i++)
	{
		RankItem* node = RankItem::create(&JhGlobalData::vec_rankData[i], rankitemtype);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		srollView->addChild(node);
	}

	RankData myrankdata;
	myrankdata.rank = JhGlobalData::myrank;
	myrankdata.nickname = JhGlobalData::getMyNickName();
	myrankdata.herotype = g_hero->getHeadID();
	myrankdata.herolv = g_hero->getLVValue();
	myrankdata.herosex = g_hero->getSex();

	if (myfightingpower > 0)
		myrankdata.heroval = myfightingpower;
	else
		myrankdata.heroval = g_nature->getPastDays();

	this->removeChildByName("mynode");
	if (rankitemtype == 0)
	{
		Node* mynode = RankItem::create(&myrankdata);
		mynode->setPosition(Vec2(360, 130));
		this->addChild(mynode, 0, "mynode");
		myFihgtNode->setVisible(false);
	}
	else
	{
		myFihgtNode->setVisible(true);
		cocos2d::ui::Button* addbtn = (cocos2d::ui::Button*)myFihgtNode->getChildByName("addbtn");
		addbtn->addTouchEventListener(CC_CALLBACK_2(JhRankLayer::onAddCount, this));

		ranktext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("myrank");
		std::string str = StringUtils::format("%d", JhGlobalData::myrank);
		ranktext->setString(str);

		str = StringUtils::format("%d", JhGlobalData::myFihgtexp);
		exptext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("myfightexp");
		exptext->setString(str);

		int fightingpower = g_hero->getMaxLifeValue() + g_hero->getTotalDf() * 20 + g_hero->getTotalAtck() * 10 + g_hero->getCritRate() * 100 + g_hero->getdodgeRate() * 100;
		str = StringUtils::format("%d", fightingpower);

		cocos2d::ui::Text* fightpowertext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("myfight");
		fightpowertext->setString(str);

		str = StringUtils::format("%d/%d", JhGlobalData::myFihgtCount, 5);
		countext = (cocos2d::ui::Text*)myFihgtNode->getChildByName("countext");
		countext->setString(str);

		this->schedule(schedule_selector(JhRankLayer::updateFightCount), 1.0f);
	}

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void JhRankLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhAddFightCountLayer* layer = JhAddFightCountLayer::create(0);
		g_gameLayer->addChild(layer, 5);
	}
}

void JhRankLayer::onSuccess()
{
	if (selectrank == 0)
	{
		std::string cdata = JhGameDataSave::getInstance()->getPlayerChallengeData();
		if (cdata.length() > 0)
		{
			std::vector<std::string> tmp;
			JhCommonFuncs::split(cdata, tmp, ",");
			if (tmp.size() >= 2)
			{
				isLastFight = true;
				JhGameDataSave::getInstance()->setPlayerChallengeData("");

				ServerDataSwap::init(this)->getChallengeResult(10000, tmp[0], atoi(tmp[1].c_str()), 1, 0);
			}
		}
		else
		{
			this->scheduleOnce(schedule_selector(JhRankLayer::delayShowData), 0.1f);
		}
	}
	else
	{
		this->scheduleOnce(schedule_selector(JhRankLayer::delayShowData), 0.1f);
	}
}

void JhRankLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	std::string descstr = "数据获取异常，请检查网络连接！！";
	if (selectrank == 2 && errcode == -3)
		descstr = "检测到你有作弊行为，无法使用该功能！！";
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf(descstr.c_str()));
	this->addChild(box);
}

void JhRankLayer::updateFightCount(float dt)
{
	std::string str = StringUtils::format("%d/%d", JhGlobalData::myFihgtCount, 5);
	countext->setString(str);

	str = StringUtils::format("%d", JhGlobalData::myrank);
	ranktext->setString(str);

	str = StringUtils::format("%d", JhGlobalData::myFihgtexp);
	exptext->setString(str);
}

RankItem::RankItem()
{

}
RankItem::~RankItem()
{

}

bool RankItem::init(RankData *data, int type)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;

	Node* csbnode = CSLoader::createNode("jhrankNode.csb");//物品节点
	if (type == 1)
	{
		csbnode = CSLoader::createNode("jhfightlistNode.csb");
	}
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	int rank = data->rank;

	std::string itemstr;

	if (type == 1)
	{
		if (rank % 2 == 0)
		{
			itemstr = "ui/fightitem0.png";
		}
	}
	else
	{
		if (data->nickname.compare(JhGlobalData::getMyNickName()) == 0)
		{
			itemstr = "ui/myrankitem.png";
		}
		else if (rank % 2 == 0)
		{
			itemstr = "ui/rankitem0.png";
		}
	}
	if (itemstr.length() > 0)
	{
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);
	}

	cocos2d::ui::TextBMFont* ranknumlbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("ranknum");
	std::string str = StringUtils::format("%d", rank);
	ranknumlbl->setString(str);

	cocos2d::ui::ImageView* tophero = (cocos2d::ui::ImageView*)csbnode->getChildByName("tophero");
	str = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	tophero->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);

	if (rank >= 1000)
		ranknumlbl->setScale(0.7f);

	if (rank <= 3)
	{
		ranknumlbl->setVisible(false);
		std::string rankspritename = StringUtils::format("ui/ranknum%d.png", rank);
		Sprite* ranknum = Sprite::createWithSpriteFrameName(rankspritename);
		ranknum->setPosition(ranknumlbl->getPosition());
		csbnode->addChild(ranknum);
	}

	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("nickname");
	nicknamelbl->setString(data->nickname);

	if (type == 0)
	{
		cocos2d::ui::Text* heronamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
		heronamelbl->setString(JhCommonFuncs::gbk2utf(heroname[(data->herotype - 1)].c_str()));
	}
	else
	{
		cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");
		std::string headimgstr = StringUtils::format("ui/fhero%d.png", data->herotype);
		headimg->loadTexture(headimgstr, cocos2d::ui::TextureResType::PLIST);
		cocos2d::ui::Button* fightbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
		fightbtn->addTouchEventListener(CC_CALLBACK_2(RankItem::onFight, this));

		if (data->playerid.compare(JhGlobalData::UUID()) == 0 && data->herotype == g_hero->getHeadID())
			fightbtn->setEnabled(false);
	}

	cocos2d::ui::Text* herolvlbl = (cocos2d::ui::Text*)csbnode->getChildByName("herolv");
	str = StringUtils::format("LV:%d", data->herolv + 1);
	herolvlbl->setString(str);

	const std::string sexstr[] = { "不详", "男", "女" };
	cocos2d::ui::Text* herosexlbl = (cocos2d::ui::Text*)csbnode->getChildByName("herosex");
	herosexlbl->setString(JhCommonFuncs::gbk2utf(sexstr[data->herosex].c_str()));
	if (data->herosex == 0)
	{
		herosexlbl->setTextColor(Color4B(204, 4, 4,255));
	}

	cocos2d::ui::Text* herovallbl = (cocos2d::ui::Text*)csbnode->getChildByName("heroval");
	str = StringUtils::format("%d", data->heroval);
	herovallbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));

	return true;
}

RankItem* RankItem::create(RankData *data ,int type)
{
	RankItem *pRet = new RankItem();
	if (pRet && pRet->init(data, type))
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

void RankItem::onFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (JhGlobalData::myFihgtCount <= 0)
		{
			JhAddFightCountLayer* alayer = JhAddFightCountLayer::create(0);
			Director::getInstance()->getRunningScene()->addChild(alayer, 1);
			return;
		}
		WaitingProgress* waitbox = WaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		ServerDataSwap::init(this)->getMyFihgterData(m_data->playerid, m_data->herotype);
	}
}

void RankItem::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	JhPlayerChallengeLayer* layer = JhPlayerChallengeLayer::create("m1-7", m_data);
	g_gameLayer->addChild(layer, 5, "playerchallengelayer");

}

void RankItem::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box, 2);
}
