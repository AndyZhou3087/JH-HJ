﻿#include "JhBuyOrangeGFLayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhSoundManager.h"
#include "JhGameScene.h"

#define SILVERCOUNT 30

JhBuyOrangeGFLayer::JhBuyOrangeGFLayer()
{
}


JhBuyOrangeGFLayer::~JhBuyOrangeGFLayer()
{
}


JhBuyOrangeGFLayer* JhBuyOrangeGFLayer::create()
{
	JhBuyOrangeGFLayer *pRet = new JhBuyOrangeGFLayer();
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

bool JhBuyOrangeGFLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhbuyOrangeGFLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhBuyOrangeGFLayer::onBack, this));

	cocos2d::ui::ImageView* gfbox = (cocos2d::ui::ImageView*)csbnode->getChildByName("buildsmall");

	cocos2d::ui::ImageView* gfimg = (cocos2d::ui::ImageView*)gfbox->getChildByName("Image");
	cocos2d::ui::Text* gfname = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl");
	cocos2d::ui::Widget* shopexgicon = (cocos2d::ui::Widget*)csbnode->getChildByName("shopexgicon");
	cocos2d::ui::Widget* yinliangbox = (cocos2d::ui::Widget*)csbnode->getChildByName("buildsmall_0");
	cocos2d::ui::Text* yinliangname = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl_0");
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	std::string descstr;

	std::vector<std::string> vec_tmpqu5gf;
	std::map<std::string, WG_NGData>::iterator it;
	for (it = JhGlobalData::map_wgngs.begin(); it != JhGlobalData::map_wgngs.end(); ++it)
	{
		WG_NGData gfdata = JhGlobalData::map_wgngs[it->first];
		if (gfdata.qu == 5 || gfdata.qu == 4)
		{
			vec_tmpqu5gf.push_back(gfdata.id);
		}
	}
	int qu5size = vec_tmpqu5gf.size();
	int r = JhGlobalData::createRandomNum(qu5size);

	if (!g_hero->checkifHasGF_Equip(vec_tmpqu5gf[r]) && JhGlobalData::tempHasGf_Equip(vec_tmpqu5gf[r]).length() <= 0)
	{
		vec_qu5gf.push_back(vec_tmpqu5gf[r]);
		gfbox->setPositionX(360);
		gfname->setPositionX(360);
		shopexgicon->setVisible(false);
		yinliangbox->setVisible(false);
		yinliangname->setVisible(false);
		descstr = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("恭喜获得神功“").c_str(), JhGlobalData::map_allResource[vec_tmpqu5gf[r]].cname.c_str(), JhCommonFuncs::gbk2utf("”，一统江湖指日可待！").c_str());

	}
	else
	{
		std::string str = StringUtils::format("%d", 80 * 1000 + SILVERCOUNT);
		vec_qu5gf.push_back(str);

		str = StringUtils::format("银两x%d", SILVERCOUNT);
		yinliangname->setString(str);
		descstr = StringUtils::format("%s%s%s%d%s", JhCommonFuncs::gbk2utf("当前已拥有“").c_str(), JhGlobalData::map_allResource[vec_tmpqu5gf[r]].cname.c_str(), JhCommonFuncs::gbk2utf("”，自动转换成").c_str(), SILVERCOUNT, JhCommonFuncs::gbk2utf("银两！拿着银两去客栈找小宝买秘籍吧！").c_str());
	}

	std::string gwstr = vec_tmpqu5gf[r];
	if (JhGlobalData::map_wgngs[gwstr].qu == 4)
	{
		std::string gwboxstr = "ui/qubox4.png";//StringUtils::format("ui/qubox4.png");
		gfbox->loadTexture(gwboxstr, cocos2d::ui::Widget::TextureResType::PLIST);
	}

	std::string iconstr = StringUtils::format("ui/%s.png", gwstr.c_str());
	gfimg->loadTexture(iconstr, cocos2d::ui::Widget::TextureResType::PLIST);
	gfimg->setContentSize(Sprite::createWithSpriteFrameName(iconstr)->getContentSize());
	gfname->setString(JhGlobalData::map_allResource[gwstr].cname);
	desc->setString(descstr);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhBuyOrangeGFLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

std::vector<std::string> JhBuyOrangeGFLayer::getRandQu5Gf()
{
	return vec_qu5gf;
}