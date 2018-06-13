#include "JhSysSmallBox.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhHero.h"

JhSysSmallBox::JhSysSmallBox()
{
	lastvalue = -1;
}


JhSysSmallBox::~JhSysSmallBox()
{
	lastvalue = -1;
}

bool JhSysSmallBox::init(BoxType type, std::string imagepath, std::string title, std::string title1, std::string text)
{
	mType = type;
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode;
	if (mType>4)
	{
		csbnode = CSLoader::createNode("jhsysSmallLayer.csb");
	}
	else{
		csbnode = CSLoader::createNode("jhsysSmallLayer2.csb");
	}
	this->addChild(csbnode);
	image = (cocos2d::ui::ImageView*)csbnode->getChildByName("damage")->getChildByName("image");
	titleTxt = (cocos2d::ui::Text*)csbnode->getChildByName("damage")->getChildByName("title");
	title1Txt = (cocos2d::ui::Text*)csbnode->getChildByName("damage")->getChildByName("title1");
	valueTxt = (cocos2d::ui::Text*) csbnode->getChildByName("damage")->getChildByName("valuelbl");

	image->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	image->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());
	//image->setScale(1.5f);
	titleTxt->setString(JhCommonFuncs::gbk2utf(title.c_str()));
	title1Txt->setString(JhCommonFuncs::gbk2utf(title1.c_str()));

	if (mType>4)
	{
		textTxt = Label::createWithTTF(JhCommonFuncs::gbk2utf(text.c_str()), "fonts/SIMHEI.TTF", 22);
		textTxt->setPosition(Vec2(170, 680));
		textTxt->setMaxLineWidth(400);
	}
	else
	{
		textTxt = Label::createWithTTF(JhCommonFuncs::gbk2utf(text.c_str()), "fonts/SIMHEI.TTF", 20);
		textTxt->setPosition(Vec2(250, 630));
		textTxt->setMaxLineWidth(230);
	}
	textTxt->setColor(Color3B(0, 0, 0));
	textTxt->setAnchorPoint(Vec2(0, 1));
	csbnode->addChild(textTxt);

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		this->removeFromParentAndCleanup(true);

		JhTopBar* topbar = (JhTopBar*)g_gameLayer->getChildByName("topbar");
		if (topbar != NULL)
			topbar->showNewerGuide(++topbar->newerStep);
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(JhSysSmallBox::updataUI), 0.2f);
	return true;
}

JhSysSmallBox* JhSysSmallBox::create(BoxType type, std::string imagepath, std::string title, std::string title1, std::string text)
{
	JhSysSmallBox *pRet = new JhSysSmallBox();
	if (pRet && pRet->init(type, imagepath, title, title1, text))
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

void JhSysSmallBox::updataUI(float dt)
{
	if (mType == REASON)
	{
		int v = g_nature->getReason();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getReason();
		}
		std::string str = StringUtils::format("ui/top_season%d.png", v);

		image->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		image->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
		image->setScale(1.5f);

		titleTxt->setString(JhCommonFuncs::gbk2utf(reasonname[v].c_str()));
		title1Txt->setString(JhCommonFuncs::gbk2utf(reasondesc1[v].c_str()));
		textTxt->setString(JhCommonFuncs::gbk2utf(reasondesc[v].c_str()));
	}
	if (mType == WEATHER)
	{
		int v = g_nature->getWeather();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getWeather();
			std::string str = StringUtils::format("ui/topweather%d.png", v);

			image->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
			image->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
			image->setScale(1.5f);

			titleTxt->setString(JhCommonFuncs::gbk2utf(weathername[v].c_str()));
			title1Txt->setString(JhCommonFuncs::gbk2utf(weatherdesc1[v].c_str()));
			textTxt->setString(JhCommonFuncs::gbk2utf(weatherdesc[v].c_str()));
		}
	}

	if (mType == LIVEDAYS)
	{
		int v = g_nature->getPastDays();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getPastDays();
			std::string str = StringUtils::format("%d天", g_nature->getPastDays());
			titleTxt->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		}

	}

	if (mType == TIME)
	{
		int v = g_nature->getTime();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getTime();
			int hour = lastvalue / 60;
			int minute = (int)lastvalue % 60;
			std::string str = StringUtils::format("%02d:%02d", hour, minute);
			titleTxt->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		}

	}

	if (mType == TEMPERATURE)
	{
		int v = g_nature->getTemperature();

		if (lastvalue != v)
		{
			lastvalue = g_nature->getTemperature();
			std::string str = StringUtils::format("%d℃", g_nature->getTemperature());
			titleTxt->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		}

	}

	if (mType == OUTERINJURY)
	{
		int v = g_hero->getOutinjuryValue();

		if (lastvalue != v)
		{
			lastvalue = g_hero->getOutinjuryValue();
			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getOutinjuryValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}

			title1Txt->setString(JhCommonFuncs::gbk2utf(outInjurydesc1[index].c_str()));
			std::string vstr = StringUtils::format("(%d/%d)", v, (int)g_hero->getMaxOutinjuryValue());
			valueTxt->setString(vstr);
		}
	}
	if (mType == INNERINJURY)
	{
		int v = g_hero->getInnerinjuryValue();

		if (lastvalue != v)
		{
			lastvalue = g_hero->getInnerinjuryValue();
			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getInnerinjuryValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}
			title1Txt->setString(JhCommonFuncs::gbk2utf(innerInjurydesc1[index].c_str()));
			std::string vstr = StringUtils::format("(%d/%d)", v, (int)g_hero->getMaxInnerinjuryValue());
			valueTxt->setString(vstr);
		}
	
	}
	if (mType == HUNGER)
	{
		int v = g_hero->getHungerValue();

		if (lastvalue != v)
		{
			lastvalue = g_hero->getHungerValue();
			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getHungerValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}
			title1Txt->setString(JhCommonFuncs::gbk2utf(hungerdesc1[index].c_str()));
			std::string vstr = StringUtils::format("(%d/%d)", v, (int)g_hero->getMaxHungerValue());
			valueTxt->setString(vstr);
		}
	}
	if (mType == SPIRIT)
	{
		int v = g_hero->getSpiritValue();

		if (lastvalue != v)
		{
			lastvalue = v;
			int index = 0;
			int valuerange[] = { 90, 70, 40, 30, 20, 10, 0 };
			for (int i = 0; i < 7; i++)
			{
				if (g_hero->getSpiritValue() >= valuerange[i])
				{
					index = i;
					break;
				}
			}
			title1Txt->setString(JhCommonFuncs::gbk2utf(spiritInjurydesc1[index].c_str()));
			std::string vstr = StringUtils::format("(%d/%d)", v, (int)g_hero->getMaxSpiritValue());
			valueTxt->setString(vstr);
		}
	}
	if (mType == LIFE)
	{
		std::string livevaluestr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), (int)g_hero->getMaxLifeValue());
		title1Txt->setString(JhCommonFuncs::gbk2utf(livevaluestr.c_str()));
	}
}
