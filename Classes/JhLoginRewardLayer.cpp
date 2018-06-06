#include "JhLoginRewardLayer.h"
#include "JhCommonFuncs.h"
#include "SoundManager.h"
#include "JhGlobalData.h"
#include "JhConst.h"
#include "StorageRoom.h"

JhLoginRewardLayer::JhLoginRewardLayer()
{

}


JhLoginRewardLayer::~JhLoginRewardLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}

bool JhLoginRewardLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhloginRewardLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *closebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(JhLoginRewardLayer::onClose, this));

	int logindays = JhGlobalData::continueLoginDays;


	for (int i = 1; i<= 7; i++)
	{
		std::string resid = LOGINREWARDS[i - 1];
		int res = atoi(resid.c_str());
		int count = 1;

		if (res > 0)
		{
			resid = StringUtils::format("%d", res / 1000);
			count = res % 1000;
		}

		std::string str = StringUtils::format("item%d", i);
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);

		cocos2d::ui::ImageView* getimg = (cocos2d::ui::ImageView*)item->getChildByName("getimg");
		getimg->addTouchEventListener(CC_CALLBACK_2(JhLoginRewardLayer::onGet, this));

		cocos2d::ui::Text* statutext = (cocos2d::ui::Text*)getimg->getChildByName("statutext");

		//cocos2d::ui::ImageView* rwdboximg = (cocos2d::ui::ImageView*)item->getChildByName("rwdboximg");
		//cocos2d::ui::ImageView* rwdimg = (cocos2d::ui::ImageView*)item->getChildByName("rwdimg");

		cocos2d::ui::Text* name = (cocos2d::ui::Text*)item->getChildByName("name");

		std::string namsstr;
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			namsstr = StringUtils::format("%sx%d", JhCommonFuncs::gbk2utf("金元宝").c_str(), count);
			resid = "gd0";
		}

		else
			namsstr = StringUtils::format("%sx%d", JhGlobalData::map_allResource[resid].cname.c_str(), count);
		name->setString(namsstr.c_str());

		/*str = "ui/buildsmall.png";
		int type = JhGlobalData::getResType(resid);
		if (type == WEAPON || type == PROTECT_EQU)
		{
			str = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[resid].qu);
		}
		else if (type == N_GONG || type == W_GONG)
		{
			str = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[resid].qu);
		}

		Sprite * rwdboximg = Sprite::createWithSpriteFrameName(str);
		rwdboximg->setPosition(Vec2(72, 130));
		item->addChild(rwdboximg, 0, "rwdboximg");*/

		str = StringUtils::format("ui/%s.png", resid.c_str());
		Sprite * rwdimg = Sprite::createWithSpriteFrameName(str);
		rwdimg->setPosition(Vec2(60, 96));
		item->addChild(rwdimg, 0, "rwdimg");

		if (i < logindays)
		{
			item->loadTexture("ui/loginritem0.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->loadTexture("ui/disnpcbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->setEnabled(false);
			statutext->setString(JhCommonFuncs::gbk2utf("已领取"));
			//JhCommonFuncs::changeGray(rwdboximg);
			JhCommonFuncs::changeGray(rwdimg);
		}
		else if (i == logindays)
		{
			item->loadTexture("ui/loginritem1.png", cocos2d::ui::Widget::TextureResType::PLIST);
			getimg->loadTexture("ui/loginrbtn.png", cocos2d::ui::Widget::TextureResType::PLIST);
			statutext->setString(JhCommonFuncs::gbk2utf("领取"));
		}
		else
		{
			getimg->setEnabled(false);
			const std::string cndays[] = { "一", "二", "三", "四", "五", "六", "七" };
			str = StringUtils::format("第%s天", cndays[i - 1].c_str());
			statutext->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		}
	}

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

JhLoginRewardLayer* JhLoginRewardLayer::create()
{
	JhLoginRewardLayer *pRet = new JhLoginRewardLayer();
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

void JhLoginRewardLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::ImageView* getimg = (cocos2d::ui::ImageView*)pSender;

		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)getimg->getParent();
		item->loadTexture("ui/loginritem0.png", cocos2d::ui::Widget::TextureResType::PLIST);
		getimg->loadTexture("ui/disnpcbtn1.png", cocos2d::ui::Widget::TextureResType::PLIST);
		getimg->setEnabled(false);
		cocos2d::ui::Text* statutext = (cocos2d::ui::Text*)getimg->getChildByName("statutext");
		statutext->setString(JhCommonFuncs::gbk2utf("已领取"));
		//JhCommonFuncs::changeGray(item->getChildByName("rwdboximg"));
		JhCommonFuncs::changeGray(item->getChildByName("rwdimg"));

		std::string resid = LOGINREWARDS[JhGlobalData::continueLoginDays - 1];
		int res = atoi(resid.c_str());
		int count = 1;
		if (res > 0)
		{
			resid = StringUtils::format("%d", res / 1000);
			count = res % 1000;
		}
		if (resid.compare(0, 1, "g") == 0)
		{
			count = atoi(resid.substr(1).c_str());
			JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() + count);

		}
		else
		{
			PackageData pdata;
			pdata.strid = resid;
			pdata.count = count;
			pdata.type = JhGlobalData::getResType(resid);
			pdata.extype = JhGlobalData::getResExType(resid);
			StorageRoom::add(pdata);
		}

		JhGlobalData::continueLoginDays = 0;

	}
}

void JhLoginRewardLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}