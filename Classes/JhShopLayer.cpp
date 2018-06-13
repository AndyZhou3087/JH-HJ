#include "JhShopLayer.h"
#include "JhConst.h"
#include "JhBuildingUILayer.h"
#include "JhStorageRoom.h"
#include "JhGameScene.h"
#include "JhSelectHeroScene.h"
#include "JhSoundManager.h"
#include "JhReviveLayer.h"
#include "json.h"
#include "JhAnalyticUtil.h"
#include "JhGameDataSave.h"
#include "JhCommonFuncs.h"
#include "JhRmbGoodsItem.h"
#include "JhGoldGoodItem.h"
#include "JhGetVipRewardLayer.h"
#include "JhMapLayer.h"
#include "JhServerDataSwap.h"
#include "JhVipShopLayer.h"
#include "JhRechargeLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif

int JhShopLayer::payindex = -1;
bool JhShopLayer::isPaying = false;
JhShopLayer::JhShopLayer()
{
	ischangePause = false;
}


JhShopLayer::~JhShopLayer()
{
	if (JhGlobalData::g_gameStatus == GAMEPAUSE && ischangePause)
		JhGlobalData::g_gameStatus = GAMESTART;
	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroResumeMoving();
	}
}

JhShopLayer* JhShopLayer::create()
{
	JhShopLayer *pRet = new JhShopLayer();
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

bool JhShopLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhshopLayer.csb");
	this->addChild(m_csbnode);

	refreshGoldCount(0);

	std::vector<GoodsData*> vec_rmbGoods;
	std::vector<GoodsData*> vec_goldGoods;

	int goodsize = JhGlobalData::vec_goods.size();

	for (int i = 0; i < goodsize; i++)
	{
		GoodsData* gdata = &JhGlobalData::vec_goods[i];
		if (gdata->type == 0)
			vec_rmbGoods.push_back(gdata);
		else if (gdata->type == 1)
			vec_goldGoods.push_back(gdata);
	}


	/*m_rmbScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("rmbgoodsscroll");
	m_rmbScrollview->setScrollBarEnabled(false);
	m_rmbScrollview->setBounceEnabled(true);*/

	/*int itemwidth = 130;
	int innerwidth = itemwidth * vec_rmbGoods.size();
	int contentwidth = m_rmbScrollview->getContentSize().width;
	if (innerwidth < contentwidth)
		innerwidth = contentwidth;
	m_rmbScrollview->setInnerContainerSize(Size(innerwidth, m_rmbScrollview->getContentSize().height));*/

	m_goldScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("goldgoodsscroll");
	m_goldScrollview->setScrollBarEnabled(false);
	m_goldScrollview->setBounceEnabled(true);

	int itemheight = 165;
	int innerheight = itemheight * (vec_goldGoods.size() / 2 + vec_goldGoods.size() % 2 + vec_rmbGoods.size() % 2 + vec_rmbGoods.size() / 2);
	int contentheight = m_goldScrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_goldScrollview->setInnerContainerSize(Size(m_goldScrollview->getContentSize().width, innerheight));

	for (unsigned int i = 0; i < vec_rmbGoods.size(); i++)
	{
		JhRmbGoodsItem* node = JhRmbGoodsItem::create(vec_rmbGoods[i]);
		node->setTag(sizeof(heroprice) / sizeof(heroprice[0]) + i);
		m_goldScrollview->addChild(node);
		node->setPosition(Vec2(142 + (i % 2) * 280, innerheight - itemheight / 2 - (i / 2) * 165));
	}

	for (unsigned int i = 0; i < vec_goldGoods.size(); i++)
	{
		JhGoldGoodItem* node = JhGoldGoodItem::create(vec_goldGoods[i]);
		m_goldScrollview->addChild(node);
		node->setPosition(Vec2(142 + (i % 2) * 280, innerheight - itemheight / 2 - (vec_rmbGoods.size() / 2 + vec_rmbGoods.size() % 2) * itemheight - (i / 2) * 165));
	}

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhShopLayer::onBack, this));


	qqtitle = (cocos2d::ui::Text*)m_csbnode->getChildByName("qqtext");
	qqtitle->setVisible(false);

	qq1 = (cocos2d::ui::Text*)m_csbnode->getChildByName("qq");
	qq1->setVisible(false);
	qq1->addTouchEventListener(CC_CALLBACK_2(JhShopLayer::onQQ, this));

	qq2 = (cocos2d::ui::Text*)m_csbnode->getChildByName("qq_1");
	qq2->addTouchEventListener(CC_CALLBACK_2(JhShopLayer::onQQ, this));
	qq2->setVisible(false);

	int qqsize = JhGlobalData::vec_qq.size();
	if (qqsize > 0)
	{
		qqtitle->setVisible(true);
		int rqq = JhGlobalData::createRandomNum(qqsize);
		qq1->setString(JhGlobalData::vec_qq[rqq]);
		qq1->setVisible(true);
		if (qqsize > 1)
		{
			qq2->setVisible(true);
			qq2->setString(JhGlobalData::vec_qq[1 - rqq]);
		}
		else
		{
			qq2->setVisible(false);
			qq1->setPositionX(qq2->getPositionX());
			m_csbnode->getChildByName("qqtext")->setPositionX(480);
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (JhGlobalData::g_gameStatus == GAMESTART)
	{
		JhGlobalData::g_gameStatus = GAMEPAUSE;
		ischangePause = true;
	}

	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroPauseMoving();
	}

	this->schedule(schedule_selector(JhShopLayer::refreshGoldCount), 1);
	return true;
}

void JhShopLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}



void JhShopLayer::beginPay(int index)
{
	if (isPaying)
		return;

	isPaying = true;
	payindex = index;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setMessage(PAY_SUCC);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//JhPaySelectLayer* layer = JhPaySelectLayer::create(payindex);
	//Director::getInstance()->getRunningScene()->addChild(layer, 1);
	JniMethodInfo methodInfo;
	char p_str[32] = { 0 };
	sprintf(p_str, "%s", payCode[index].c_str());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/PayAction", "pay", "(Ljava/lang/String;I)V"))
	{
		jstring str1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, str1, payindex);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//payCode
	buy((char*)payCode[payindex].c_str());
#endif
}
void JhShopLayer::setMessage(PYARET ret)
{
	if (ret == PAY_SUCC && payindex >= 0)
	{
		int herocount = sizeof(heroprice) / sizeof(heroprice[0]);
		int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);
		int vipcount = sizeof(vipgoldcount) / sizeof(vipgoldcount[0]);
		if (payindex < herocount) // 人物解锁
		{
			if (g_SelectHeroScene != NULL)
				g_SelectHeroScene->unlockSucc(payindex);
#ifdef ANALYTICS
			std::string heroname[] = { "bxym", "bssy", "bjxb", "baq" };
			JhAnalyticUtil::onEvent(heroname[payindex].c_str());
#endif
		}
		else if (payindex < herocount + golditemcount)//买元宝
		{
			//addBuyGoods();
			JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() + goldcount[payindex - herocount]);
			JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUYOK);
#ifdef ANALYTICS
			std::string name[] = { "b6", "b12", "b30", "b68", "b128"};
			JhAnalyticUtil::onEvent(name[payindex - herocount].c_str());
#endif
		}
		else if (payindex < herocount + golditemcount + vipcount)//买VIP
		{
			std::string vipid = JhGlobalData::vec_goods[payindex - herocount].icon;
			JhGlobalData::vec_buyVipIds.push_back(vipid);
			//int monthdays = JhGlobalData::getMonth_Days();
			//if (JhGlobalData::map_buyVipDays.find(vipid) != JhGlobalData::map_buyVipDays.end())
			//	JhGlobalData::map_buyVipDays[vipid] += monthdays;
			//else
			//	JhGlobalData::map_buyVipDays[vipid] = monthdays;

			JhServerDataSwap::init()->vipSuccNotice(vipid);
			if (g_gameLayer != NULL)
			{
				g_gameLayer->scheduleOnce(schedule_selector(JhShopLayer::showVipReward), 0.1f);
			}
#ifdef ANALYTICS
			std::string name[] = { "byk6", "byk30", "byk68"};
			JhAnalyticUtil::onEvent(name[payindex - herocount - golditemcount].c_str());
#endif
		}
		else if (payindex == TIMEGIFT)
		{
			if (g_gameLayer != NULL)
			{
				g_gameLayer->removeChildByName("gift");
			}
			JhGlobalData::setIsBuyTimeGift(true);
			JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() + 100);
			JhGoldGoodItem::addBuyGoods(&JhGlobalData::vec_goods[payindex - herocount]);

#ifdef ANALYTICS
			JhAnalyticUtil::onEvent("timegift");
#endif
		}
#ifdef ANALYTICS
		JhAnalyticUtil::pay("pay", buyprice[payindex], 1);
#endif
		JhServerDataSwap::init(NULL)->postMyRecharge(buyprice[payindex], 0);
		if (JhGlobalData::isRecharge && g_gameLayer != NULL)
		{
			g_gameLayer->scheduleOnce(schedule_selector(JhShopLayer::delayShowRecharge), 1.0f);
		}
	}
	payindex = -1;
	isPaying = false;
}

void JhShopLayer::showVipReward(float dt)
{
	JhGetVipRewardLayer* layer = JhGetVipRewardLayer::create();
	if (g_gameLayer != NULL)
	{
		g_gameLayer->addChild(layer, 10, "viprewardlayer");
		JhVipShopLayer* vipshop = (JhVipShopLayer*)g_gameLayer->getChildByName("vipshoplayer");
		if (vipshop != NULL)
			vipshop->getLeftDays();
	}
}

void JhShopLayer::refreshGoldCount(float dt)
{
	mygoldlbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("mygoldlbl");
	std::string countstr = StringUtils::format("%d", JhGlobalData::getMyGoldCount());
	mygoldlbl->setString(countstr);
}

void JhShopLayer::onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		cocos2d::ui::Text* qq = (cocos2d::ui::Text*)pSender;
		JhGlobalData::copyToClipBoard(qq->getString());
	}

}

void JhShopLayer::delayShowRecharge(float dt)
{
	int maxamount = JhRechargeLayer::getRechargeMaxAmount();
	if ((maxamount == 0 || JhGlobalData::recharageData.mygotton < maxamount) && g_gameLayer != NULL)
		g_gameLayer->addChild(JhRechargeLayer::create(), 5);

}