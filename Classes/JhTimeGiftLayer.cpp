#include "JhTimeGiftLayer.h"
#include "JhCommonFuncs.h"
#include "JhSoundManager.h"
#include "JhGlobalData.h"
#include "JhConst.h"
#include "JhShopLayer.h"

JhTimeGiftLayer::JhTimeGiftLayer()
{

}


JhTimeGiftLayer::~JhTimeGiftLayer()
{

}

bool JhTimeGiftLayer::init(int goodsId)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhtimeGfitLayer.csb");
	this->addChild(csbnode);

	int herocount = sizeof(heroprice) / sizeof(heroprice[0]);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(JhTimeGiftLayer::onbuy, this));

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(JhTimeGiftLayer::onClose, this));

	cocos2d::ui::Text* price = (cocos2d::ui::Text*)csbnode->getChildByName("price");

	std::string str = StringUtils::format("%d元", JhGlobalData::vec_goods[goodsId - herocount].price);
	price->setString(JhCommonFuncs::gbk2utf(str.c_str()));

	m_timelbl = (cocos2d::ui::Text*)csbnode->getChildByName("time");

	if (JhGlobalData::isOnline)
	{
		checkTimeGift(0);
		this->schedule(schedule_selector(JhTimeGiftLayer::checkTimeGift), 1.0f);
	}
	else
	{
		m_timelbl->setVisible(false);
		csbnode->getChildByName("timedesc")->setVisible(false);
	}

	int rgoldcount = 100;
	std::vector<std::string> vec_rewardres = JhGlobalData::vec_goods[goodsId - herocount].vec_res;

	int startx = 374;
	int spacex = 106;
	int starty = 627;
	if (vec_rewardres.size() == 2)
	{
		startx = 374;
		spacex = 106;
	}

	int ressize = vec_rewardres.size() + 1;
	for (int i = 0; i < ressize; i++)
	{
		Sprite * box = Sprite::createWithSpriteFrameName("ui/timesmall.png");
		box->setPosition(Vec2(startx + (i%3)*spacex, starty - i/3*100));
		this->addChild(box);


		std::string resstr;
		std::string strcount;
		std::string namstr;
		if (i == 0)
		{
			resstr = "ui/gd0.png";
			strcount = StringUtils::format("x%d", rgoldcount);
			namstr = JhCommonFuncs::gbk2utf("金元宝");
		}
		else
		{
			std::string resid = vec_rewardres[i - 1];
			int intresid = atoi(resid.c_str());
			resstr = StringUtils::format("ui/%d.png", intresid / 1000);
			strcount = StringUtils::format("x%d", intresid % 1000);
			std::string ridstr = StringUtils::format("%d", intresid / 1000);
			namstr = JhGlobalData::map_allResource[ridstr].cname;
		}
		Sprite* res = Sprite::createWithSpriteFrameName(resstr);
		res->setScale(0.5);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);

		Label * coutlbl = Label::createWithTTF(strcount, "fonts/SIMHEI.TTF", 20);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(0.5, 0.5));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX(), box->getPositionY() - box->getContentSize().height / 2 - 20));
		this->addChild(coutlbl);

		/*Label * namelbl = Label::createWithTTF(namstr, "fonts/SIMHEI.TTF", 20);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), box->getPositionY() - box->getContentSize().height / 2 - 20));
		this->addChild(namelbl);*/
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->setTag(goodsId);
	return true;
}

JhTimeGiftLayer* JhTimeGiftLayer::create(int goodsId)
{
	JhTimeGiftLayer *pRet = new JhTimeGiftLayer();
	if (pRet && pRet->init(goodsId))
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

void JhTimeGiftLayer::onbuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhShopLayer::beginPay(this->getTag());
	}
}

void JhTimeGiftLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhTimeGiftLayer::checkTimeGift(float dt)
{
	int lefttime = JhGlobalData::getTimeGiftLeftTime();

	int hour = lefttime / 3600;
	int min = lefttime % 3600 / 60;
	int sec = lefttime % 3600 % 60;
	std::string timestr = StringUtils::format("%02d:%02d:%02d", hour, min, sec);
	m_timelbl->setString(timestr);

}
