#include "JhVipShopLayer.h"
#include "JhConst.h"
#include "JhSoundManager.h"
#include "json.h"
#include "JhAnalyticUtil.h"
#include "JhGameDataSave.h"
#include "JhCommonFuncs.h"
#include "JhVipGoodsItem.h"
#include "JhGameScene.h"
#include "JhMapLayer.h"
#include "JhWaitingProgress.h"

JhVipShopLayer::JhVipShopLayer()
{
}


JhVipShopLayer::~JhVipShopLayer()
{

	JhGlobalData::g_gameStatus = GAMESTART;
	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroResumeMoving();
	}
}

JhVipShopLayer* JhVipShopLayer::create()
{
	JhVipShopLayer *pRet = new JhVipShopLayer();
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

bool JhVipShopLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhvipShopLayer.csb");
	this->addChild(m_csbnode);

	std::vector<GoodsData*> vec_vipGoods;

	int goodsize = JhGlobalData::vec_goods.size();

	for (int i = 0; i < goodsize; i++)
	{
		GoodsData* vipgdata = &JhGlobalData::vec_goods[i];
		if (vipgdata->type == 2)
			vec_vipGoods.push_back(vipgdata);
	}

	m_vipScrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("vipgoodsscroll");
	m_vipScrollview->setScrollBarEnabled(false);
	m_vipScrollview->setBounceEnabled(true);

	int itemheight = 230;
	int innerheight = itemheight * vec_vipGoods.size();
	int contentheight = m_vipScrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_vipScrollview->setInnerContainerSize(Size(m_vipScrollview->getContentSize().width, innerheight));

	//去掉第一个6月月卡
	int starti = 1;
	std::map<std::string, int>::iterator it;

	for (it = JhGlobalData::map_buyVipDays.begin(); it != JhGlobalData::map_buyVipDays.end(); ++it)
	{
		if (it->first.find("vip1") != std::string::npos)
		{
			starti = 0;
		}
	}

	for (unsigned int i = starti; i < vec_vipGoods.size(); i++)
	{
		JhVipGoodsItem* node = JhVipGoodsItem::create(vec_vipGoods[i]);
		node->setTag(sizeof(heroprice) / sizeof(heroprice[0]) + sizeof(goldcount) / sizeof(goldcount[0]) + i);
		m_vipScrollview->addChild(node);
		node->setPosition(Vec2(148+(i-1)*293, 416));
	}

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhVipShopLayer::onBack, this));

	if (JhGlobalData::g_gameStatus == GAMESTART)
		JhGlobalData::g_gameStatus = GAMEPAUSE;

	if (g_hero != NULL && g_hero->getIsMoving())
	{
		g_maplayer->heroPauseMoving();
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhVipShopLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhVipShopLayer::onSuccess()
{
	this->removeChildByName("waitbox");
}

void JhVipShopLayer::onErr(int errcode)
{
	this->removeChildByName("waitbox");
}

void JhVipShopLayer::getLeftDays()
{
	JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
	this->addChild(waitbox, 1, "waitbox");
	JhServerDataSwap::init(this)->vipIsOn(g_hero->getHeadID());
}
