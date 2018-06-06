#include "VipGoodsItem.h"
#include "JhCommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "JhGameScene.h"
#include "JhConst.h"
#include "JhAnalyticUtil.h"

VipGoodsItem::VipGoodsItem()
{
	isDraging = false;
}


VipGoodsItem::~VipGoodsItem()
{
}

bool VipGoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_goodData = gdata;

	Node* csbnode = CSLoader::createNode("jhvipGoodsNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	cocos2d::ui::ImageView* bgnode = (cocos2d::ui::ImageView*)csbnode->getChildByName("itembg");

	icon = (cocos2d::ui::ImageView*)bgnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)bgnode->getChildByName("name");
	descTxt = (cocos2d::ui::Text*)bgnode->getChildByName("desc");
	priceTxt = (cocos2d::ui::Text*)csbnode->getChildByName("price");

	leftday = (cocos2d::ui::Text*)bgnode->getChildByName("leftday");
	leftday_0 = (cocos2d::ui::Text*)bgnode->getChildByName("leftday_0");
	leftday_1 = (cocos2d::ui::Text*)bgnode->getChildByName("leftday_1");

	//图标
	std::string imagepath = StringUtils::format("ui/%s.png", gdata->icon.c_str());
	icon->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	icon->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(gdata->name);
	//descTxt->setString(gdata->desc);
	std::string pricestr = StringUtils::format("%d元", gdata->price);
	priceTxt->setString(JhCommonFuncs::gbk2utf(pricestr.c_str()));

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(VipGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	if (m_goodData->icon == "vip3")
	{
		bgnode->loadTexture("images/vip_1.png", cocos2d::ui::TextureResType::LOCAL);
	}

	int golditemcount = sizeof(goldcount) / sizeof(goldcount[0]);
	int index = 0;
	for (unsigned int i = 0; i < JhGlobalData::vec_goods.size(); i++)
	{
		if (JhGlobalData::vec_goods[i].icon.compare(m_goodData->icon) == 0)
		{
			index = i;
			break;
		}
	}

	int rgoldcount = vipgoldcount[index - golditemcount];

	std::vector<std::string> vec_rewardres = m_goodData->vec_res;

	int startx = -25;
	int spacex = 75;
	int starty = -15;

	int ressize = vec_rewardres.size() + 1;
	for (int i = 0; i < ressize; i++)
	{
		std::string resstr;
		std::string strcount;
		if (i == 0)
		{
			resstr = "ui/gd0.png";
			strcount = StringUtils::format("x%d", rgoldcount);
		}
		else
		{
			std::string resid = vec_rewardres[i - 1];
			int intresid = atoi(resid.c_str());
			resstr = StringUtils::format("ui/%d.png", intresid / 1000);
			strcount = StringUtils::format("x%d", intresid % 1000);
		}
		Sprite* box = Sprite::createWithSpriteFrameName("ui/timesmall.png");
		box->setPosition(Vec2(startx + (i % 3)*spacex, starty - i / 3 * 74));
		this->addChild(box);

		Sprite* res = Sprite::createWithSpriteFrameName(resstr);
		res->setPosition(box->getPosition());
		res->setScale(0.5f);
		this->addChild(res);

		Label * coutlbl = Label::createWithTTF(strcount, "fonts/SIMHEI.TTF", 18);//Label::createWithSystemFont(strcount, "", 21);
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 30, box->getPositionY() - box->getContentSize().height / 2 + 10));
		this->addChild(coutlbl);
	}

	m_buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	m_buybtn->addTouchEventListener(CC_CALLBACK_2(VipGoodsItem::onBuyBtn, this));
	m_buybtn->setSwallowTouches(false);

	updateLeftDays(0);
	this->schedule(schedule_selector(VipGoodsItem::updateLeftDays), 1.0f);

	return true;
}

VipGoodsItem* VipGoodsItem::create(GoodsData* gdata)
{
	VipGoodsItem *pRet = new VipGoodsItem();
	if (pRet && pRet->init(gdata))
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
void VipGoodsItem::onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
}

void VipGoodsItem::onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		startPos = node->convertToWorldSpace(this->getParent()->getPosition());
	}
	else if (type == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 pos = node->convertToWorldSpace(this->getParent()->getPosition());
		if (fabsf(pos.y - startPos.y) > 20)
			isDraging = true;
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!isDraging)
		{
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
			ShopLayer::beginPay(this->getTag());
		}
		isDraging = false;
	}
}

void VipGoodsItem::updateLeftDays(float dt)
{
	std::map<std::string, int>::iterator it;

	for (it = JhGlobalData::map_buyVipDays.begin(); it != JhGlobalData::map_buyVipDays.end(); ++it)
	{
		if (it->first.find(m_goodData->icon) != std::string::npos)
		{
			leftday_0->setVisible(true);
			leftday_1->setVisible(true);
			leftday->setVisible(true);
			std::string leftdaystr = StringUtils::format("%d", JhGlobalData::map_buyVipDays[it->first]);
			leftday->setString(leftdaystr);

			if (it->first.find("vip1") != std::string::npos)
			{
				m_buybtn->setVisible(false);
			}
			break;
		}
	}
}
