#include "JhHeroStateUILayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhHero.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhGameDataSave.h"
#include "JhMapLayer.h"
#include "JhHomeLayer.h"
#include "JhNewerGuideLayer.h"
#include "JhHintBox.h"

JhHeroStateUILayer::JhHeroStateUILayer()
{

}


JhHeroStateUILayer::~JhHeroStateUILayer()
{
}

bool JhHeroStateUILayer::init()
{

	m_csbnode = CSLoader::createNode("jhheroStateLayer.csb");
	this->addChild(m_csbnode, 0, "csbnode");

	heroAttribNode = JhHeroProperNode::create(1);
	heroAttribNode->setPosition(Vec2(360, 707));
	m_csbnode->addChild(heroAttribNode, 0, "JhHeroProperNode");

	propertybg = (cocos2d::ui::Widget*)m_csbnode->getChildByName("propertybg");
	property = (cocos2d::ui::Widget*)m_csbnode->getChildByName("property");

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhHeroStateUILayer::onBack, this));

	cocos2d::ui::ImageView* roleimg = (cocos2d::ui::ImageView*)propertybg->getChildByName("roleimg");
	std::string str = StringUtils::format("rolestate_%d.png", g_hero->getHeadID());
	roleimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);

	cocos2d::ui::Text* rolename = (cocos2d::ui::Text*)propertybg->getChildByName("rolename");
	str = heroname[g_hero->getHeadID() - 1];
	rolename->setString(str);

	btn_1 = (cocos2d::ui::Button*)m_csbnode->getChildByName("btn_1");
	btn_1->addTouchEventListener(CC_CALLBACK_2(JhHeroStateUILayer::onbtn1, this));
	btn_2 = (cocos2d::ui::Button*)m_csbnode->getChildByName("btn_2");
	btn_2->addTouchEventListener(CC_CALLBACK_2(JhHeroStateUILayer::onbtn2, this));
	btn_3 = (cocos2d::ui::Button*)m_csbnode->getChildByName("btn_3");
	btn_3->addTouchEventListener(CC_CALLBACK_2(JhHeroStateUILayer::onbtn3, this));
	btn_1->getChildByName("btnname")->setVisible(true);
	btn_2->getChildByName("btnname1")->setVisible(false);
	btn_3->getChildByName("btnname")->setVisible(false);
	btn_1->setEnabled(false);
	btn_2->setEnabled(true);
	btn_3->setEnabled(true);

	for (int i = 0; i < sizeof(herostatus) / sizeof(herostatus[0]); i++)
	{
		std::string str = StringUtils::format("herostate%d", i + 1);
		herostatus[i] = (cocos2d::ui::Text*)m_csbnode->getChildByName("property")->getChildByName(str);
	}

	m_heroexptimelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("property")->getChildByName("heroexptime");
	m_gfexptimelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("property")->getChildByName("gfexptime");

	arrow1 = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("property")->getChildByName("arrow1");
	arrow2 = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("property")->getChildByName("arrow2");
	arrow3 = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("property")->getChildByName("arrow3");

	sexhintimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("property")->getChildByName("sexhintimg");
	sexhintimg->addTouchEventListener(CC_CALLBACK_2(JhHeroStateUILayer::onSexHelpHint, this));

	m_heroexpendtime = JhGameDataSave::getInstance()->getHeroExpEndTime();
	m_gfexpendtime = JhGameDataSave::getInstance()->getGfEndTime();

	lastatk = g_hero->getTotalAtck();
	lastdf = g_hero->getTotalDf();
	lastmaxhp = g_hero->getMaxLifeValue();

	updateStatus(0);

	mixnode = JhMixGFNode::create();
	mixnode->setPosition(Vec2(366, 664));
	m_csbnode->addChild(mixnode, 0, "mixnode");


	if (!btn_2->isEnabled())
	{
		heroAttribNode->setVisible(true);
	}
	else
	{
		heroAttribNode->setVisible(false);
	}

	if (!btn_1->isEnabled())
	{
		propertybg->setVisible(true);
		property->setVisible(true);
	}
	else
	{
		propertybg->setVisible(false);
		property->setVisible(false);
	}

	if (!btn_3->isEnabled())
	{
		mixnode->setVisible(true);
	}
	else
	{
		mixnode->setVisible(false);
	}


	if (JhNewerGuideLayer::checkifNewerGuide(3))
	{
		btn_1->getChildByName("btnname")->setVisible(false);
		btn_2->getChildByName("btnname1")->setVisible(true);
		btn_3->getChildByName("btnname")->setVisible(false);
		btn_1->setEnabled(true);
		btn_2->setEnabled(false);
		btn_3->setEnabled(true);
		heroAttribNode->setVisible(true);
		mixnode->setVisible(false);
		propertybg->setVisible(false);
		property->setVisible(false);
	}

	//////layer 点击事件，屏蔽下层事件
	this->schedule(schedule_selector(JhHeroStateUILayer::updateStatus), 1.0f);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void JhHeroStateUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);

		JhTopBar* topbar = (JhTopBar*)g_gameLayer->getChildByName("topbar");
		if (topbar != NULL)
			topbar->showNewerGuide(13);
	}
}

void JhHeroStateUILayer::onbtn1(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		btn_1->getChildByName("btnname")->setVisible(true);
		btn_2->getChildByName("btnname1")->setVisible(false);
		btn_3->getChildByName("btnname")->setVisible(false);
		btn_1->setEnabled(false);
		btn_2->setEnabled(true);
		btn_3->setEnabled(true);
		heroAttribNode->setVisible(false);
		mixnode->setVisible(false);
		propertybg->setVisible(true);
		property->setVisible(true);
	}
}

void JhHeroStateUILayer::onbtn2(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		btn_1->getChildByName("btnname")->setVisible(false);
		btn_2->getChildByName("btnname1")->setVisible(true);
		btn_3->getChildByName("btnname")->setVisible(false);
		btn_1->setEnabled(true);
		btn_2->setEnabled(false);
		btn_3->setEnabled(true);
		heroAttribNode->setVisible(true);
		mixnode->setVisible(false);
		propertybg->setVisible(false);
		property->setVisible(false);
	}
}

void JhHeroStateUILayer::onbtn3(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		btn_1->getChildByName("btnname")->setVisible(false);
		btn_2->getChildByName("btnname1")->setVisible(false);
		btn_3->getChildByName("btnname")->setVisible(true);
		btn_1->setEnabled(true);
		btn_2->setEnabled(true);
		btn_3->setEnabled(false);
		heroAttribNode->setVisible(false);
		mixnode->setVisible(true);
		propertybg->setVisible(false);
		property->setVisible(false);
	}
}

void JhHeroStateUILayer::updateStatus(float dt)
{
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

	int curack = g_hero->getTotalAtck();
	//攻击属性
	std::string str = StringUtils::format("%d", curack);
	herostatus[0]->setString(str);

	int curdf = g_hero->getTotalDf();

	//防御属性
	str = StringUtils::format("%d", curdf);
	herostatus[1]->setString(str);

	//经验值属性
	str = StringUtils::format("%d/%d", g_hero->getExpValue(), JhGlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp[g_hero->getLVValue()]);
	herostatus[2]->setString(str);
	//等级属性
	str = StringUtils::format("%d", g_hero->getLVValue() + 1);
	int lvmax = JhGlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp.size();
	if (g_hero->getLVValue() + 1 >= lvmax)
		str = StringUtils::format("%d%s", lvmax, JhCommonFuncs::gbk2utf("（满级）").c_str());
	herostatus[3]->setString(str);

	int curmaxhp = g_hero->getMaxLifeValue();
	//生命值属性
	str = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), curmaxhp);
	herostatus[4]->setString(str);

	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (JhGlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			herostatus[5]->setString(JhGlobalData::map_npcs[nid].name);
			break;
		}
	}
	//性别
	if (g_hero->getSex() == S_NONE)
	{
		m_csbnode->getChildByName("property")->getChildByName("herostatetext7")->setVisible(true);
		herostatus[6]->setVisible(true);
		herostatus[6]->setString(JhCommonFuncs::gbk2utf("不详"));
		sexhintimg->setVisible(true);
	}

	int letftime1 = m_heroexpendtime - JhGlobalData::getSysSecTime();
	if (letftime1 > 0)
	{
		std::string str;
		int day = letftime1 / 86400;
		int sectime = letftime1 % 86400;
		if (day > 0)
			str = StringUtils::format("经验药水效果剩%d天%02d:%02d:%02d", day, sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		else
			str = StringUtils::format("经验药水效果剩%02d:%02d:%02d", sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		m_heroexptimelbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		m_heroexptimelbl->setVisible(true);
	}
	else
	{
		m_heroexptimelbl->setVisible(false);
	}

	int letftime2 = m_gfexpendtime - JhGlobalData::getSysSecTime();
	if (letftime2 > 0)
	{
		std::string str;
		int day = letftime2 / 86400;
		int sectime = letftime2 % 86400;
		if (day > 0)
			str = StringUtils::format("大力丸效果剩%d天%02d:%02d:%02d", day, sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		else
			str = StringUtils::format("大力丸效果剩%02d:%02d:%02d", sectime / 3600, sectime % 3600 / 60, sectime % 3600 % 60);
		m_gfexptimelbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		m_gfexptimelbl->setVisible(true);
	}
	else
	{
		m_gfexptimelbl->setVisible(false);
	}
	arrow1->setPositionX(herostatus[0]->getPositionX() + herostatus[0]->getContentSize().width);
	arrow2->setPositionX(herostatus[1]->getPositionX() + herostatus[1]->getContentSize().width);
	arrow3->setPositionX(herostatus[4]->getPositionX() + herostatus[4]->getContentSize().width);
}

void JhHeroStateUILayer::onExit()
{
	Layer::onExit();
}

void JhHeroStateUILayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	nodes.push_back(m_csbnode->getChildByName("backbtn"));
	g_gameLayer->showNewerGuide(step, nodes);
}

void JhHeroStateUILayer::updateArrow()
{
	int curack = g_hero->getTotalAtck();
	if (curack != lastatk)
	{
		arrow1->setVisible(true);
		arrow1->setPositionX(herostatus[0]->getPositionX() + herostatus[0]->getContentSize().width);
		if (curack > lastatk)
		{
			arrow1->loadTexture("ui/arrowup.png", cocos2d::ui::TextureResType::PLIST);
		}
		else if (curack < lastatk)
		{
			arrow1->loadTexture("ui/arrowdown.png", cocos2d::ui::TextureResType::PLIST);
		}
		arrow1->stopAllActions();
		arrow1->runAction(Repeat::create(Sequence::create(FadeOut::create(0.3f), FadeIn::create(0.3f), NULL), 5));
		lastatk = curack;
	}
	else
	{
		arrow1->stopAllActions();
		arrow1->setVisible(false);
	}

	int curdf = g_hero->getTotalDf();
	if (curdf != lastdf)
	{
		arrow2->setVisible(true);
		arrow2->setPositionX(herostatus[1]->getPositionX() + herostatus[1]->getContentSize().width);
		if (curdf > lastdf)
		{
			arrow2->loadTexture("ui/arrowup.png", cocos2d::ui::TextureResType::PLIST);
		}
		else if (curdf < lastdf)
		{
			arrow2->loadTexture("ui/arrowdown.png", cocos2d::ui::TextureResType::PLIST);
		}
		arrow2->stopAllActions();
		arrow2->runAction(Repeat::create(Sequence::create(FadeOut::create(0.3f), FadeIn::create(0.3f), NULL), 5));
		lastdf = curdf;
	}
	else
	{
		arrow2->stopAllActions();
		arrow2->setVisible(false);
	}

	int curmaxhp = g_hero->getMaxLifeValue();
	if (curmaxhp != lastmaxhp)
	{
		arrow3->setVisible(true);
		arrow3->setPositionX(herostatus[4]->getPositionX() + herostatus[4]->getContentSize().width);
		if (curmaxhp > lastmaxhp)
		{
			arrow3->loadTexture("ui/arrowup.png", cocos2d::ui::TextureResType::PLIST);
		}
		else if (curmaxhp < lastmaxhp)
		{
			arrow3->loadTexture("ui/arrowdown.png", cocos2d::ui::TextureResType::PLIST);
		}
		arrow3->stopAllActions();
		arrow3->runAction(Repeat::create(Sequence::create(FadeOut::create(0.3f), FadeIn::create(0.3f), NULL), 5));
		lastmaxhp = curmaxhp;
	}
	else
	{
		arrow3->stopAllActions();
		arrow3->setVisible(false);
	}
}

void JhHeroStateUILayer::onSexHelpHint(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("可在商城购买【华佗之手】恢复男儿之身！"));
		this->addChild(hbox);
	}
}