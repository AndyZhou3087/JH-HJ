#include "JhFightLayer.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhCommonFuncs.h"
#include "JhHintBox.h"
#include "JhGameScene.h"
#include "Winlayer.h"
#include "SoundManager.h"
#include "JhNewerGuideLayer.h"
#include "JhAnalyticUtil.h"
#include "JhMapLayer.h"
#include "Shake.h"
#include "JhChallengeCountLayer.h"
#include "JhMyActionProgressTimer.h"

JhFightLayer::JhFightLayer()
{
	isecapeok = false;
	isUseWg = false;
	winnpcount = 0;
	winProperCount = 0;
	isWin = false;
}


JhFightLayer::~JhFightLayer()
{
	if (g_hero != NULL)
	{
		g_hero->setTotalAtkBonusPercent(0.0f);
		g_hero->setTotalDfBonusPercent(0.0f);
		g_hero->setIsWDChallenge(false);
	}
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
}

JhFightLayer* JhFightLayer::create(std::string addrid, std::string npcid)
{
	JhFightLayer *pRet = new JhFightLayer();
	if (pRet && pRet->init(addrid, npcid))
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

bool JhFightLayer::init(std::string addrid, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("jhfightLayer.csb");
	this->addChild(csbnode);

	m_addrid = addrid;
	m_npcid = npcid;

	//地点名称
	cocos2d::ui::Text* addrnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrnametxt->setString(JhGlobalData::map_maps[m_addrid].cname);

	if (m_addrid.compare("m13-1") == 0)
	{
		g_hero->setIsWDChallenge(true);
		m_npcid = JhGlobalData::map_maps[m_addrid].npcs[0];
		continuefight = JhGlobalData::map_maps[m_addrid].npcs.size() - 1;
	}
	else if (m_addrid.compare("m1-6") == 0)
	{
		m_npcid = JhGlobalData::map_maps[m_addrid].npcs[0];
		continuefight = JhGlobalData::map_maps[m_addrid].npcs.size() - 1;
	}
	else
	{
		continuefight = 0;
	}

	cocos2d::ui::ImageView* heroicon = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroicon");
	std::string heroiconstr = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	heroicon->loadTexture(heroiconstr, cocos2d::ui::TextureResType::PLIST);
	heroicon->setScale(0.6f);
	// NPC 图标
	npchead = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");
	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);

	//NPC名称
	npcnametxt = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcnametxt->setString(JhGlobalData::map_npcs[m_npcid].name);
	
	//角色名
	cocos2d::ui::Text* heronametxt = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	heronametxt->setString(g_hero->getMyName());

	int maxlife = g_hero->getMaxLifeValue();;
	//角色血量显示
	herohpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("herohpvaluetext");
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), maxlife);
	herohpvaluetext->setString(hpstr);

	//角色血量进度
	float herohppercent = 100 * g_hero->getLifeValue() / maxlife;

	herohpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar");
	herohpbar->setPercent(herohppercent);

	herohpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("herohpbar2");
	herohpbar2->setPercent(herohppercent);

	npcmaxhp = JhGlobalData::map_npcs[m_npcid].life;
	npchp = npcmaxhp;

	npcatk = JhGlobalData::map_npcs[m_npcid].atk;
	npcdf = JhGlobalData::map_npcs[m_npcid].df;

	//NPC血量显示
	npchpvaluetext = (cocos2d::ui::Text*)csbnode->getChildByName("npchpvaluetext");
	hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);

	//NCP血量进度
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar");
	npchpbar->setPercent(npchppercent);

	npchpbar2 = (cocos2d::ui::LoadingBar*)csbnode->getChildByName("npchpbar2");
	npchpbar2->setPercent(npchppercent);

	//逃跑按钮
	m_escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	m_escapebtn->addTouchEventListener(CC_CALLBACK_2(JhFightLayer::onEscape, this));
	m_escapebtn->setTag(0);

	if (JhNewerGuideLayer::checkifNewerGuide(39))
		m_escapebtn->setVisible(false);
	// 滚动文字
	m_fihgtScorll = UIScroll::create(610.0f, 435.0f);
	m_fihgtScorll->setPosition(Vec2(360, 350));
	csbnode->addChild(m_fihgtScorll);

	heroactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroactimg");
	npcactimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcactimg");

	herocritfnt = (cocos2d::ui::TextBMFont*)heroactimg->getChildByName("herocritfnt");
	npccritfnt = (cocos2d::ui::TextBMFont*)npcactimg->getChildByName("npccritfnt");

	if (npcid.compare("n001") != 0)
		this->scheduleOnce(schedule_selector(JhFightLayer::delayHeroFight), 1.0f);//1.0s，hero->npc
	else
	{
		addrnametxt->setString(JhCommonFuncs::gbk2utf("路上"));
		m_fightbtn = (cocos2d::ui::Button*)csbnode->getChildByName("fightbtn");
		m_fightbtn->addTouchEventListener(CC_CALLBACK_2(JhFightLayer::onFihgt, this));
		m_fightbtn->setVisible(true);
		m_escapebtn->setPositionX(480);
		this->schedule(schedule_selector(JhFightLayer::checkHeroLife), 0.5f);
	}

	herohurticon = (cocos2d::ui::Widget*)csbnode->getChildByName("herohurt");
	npchurticon = (cocos2d::ui::Widget*)csbnode->getChildByName("npchurt");

	resetSkills();
	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	int r = JhGlobalData::createRandomNum(4);
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_FIGHT_0 + r);
	return true;
}

void JhFightLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_npcid.compare("n001") == 0)
	{
        if (g_maplayer != NULL)
            g_maplayer->heroPauseMoving();
	}

}

void JhFightLayer::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		JhGlobalData::isFightMaster = false;
		if (isWin)
			return;

		if (node->getTag() == 0)
		{
			int r = JhGlobalData::createRandomNum(100);
			if (r < JhGlobalData::map_npcs[m_npcid].escapernd)
			{
				JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("逃跑成功！"));
				addChild(hbox);

				isecapeok = true;
			}
			else
			{
				g_uiScroll->addEventText(JhCommonFuncs::gbk2utf("你乘机逃跑，可惜失败了！！"), 25, Color3B(204, 4, 4));
				m_escapebtn->setEnabled(false);
				if (m_npcid.compare("n001") == 0)
				{
					fightRobber();
				}
			}
			m_escapebtn->setTitleText(JhCommonFuncs::gbk2utf("返回"));
			m_escapebtn->setTag(1);
			if (isecapeok && m_npcid.compare("n001") == 0)//山贼
			{
                if (g_maplayer != NULL)
                    g_maplayer->heroResumeMoving();
				this->removeFromParentAndCleanup(true);
			}
		}
		else
		{
			this->removeFromParentAndCleanup(true);
		}
	}
}

void JhFightLayer::onFihgt(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_escapebtn->setEnabled(false);
		fightRobber();
	}
}

void JhFightLayer::fightRobber()
{
	m_fightbtn->setEnabled(false);
	this->unschedule(schedule_selector(JhFightLayer::checkHeroLife));
	this->scheduleOnce(schedule_selector(JhFightLayer::delayHeroFight), 1.0f);
}

void JhFightLayer::npcDie()
{
	if (m_addrid.compare("m1-6") == 0)
	{
		if (continuefight <= 0)
		{
			isWin = true;
			this->scheduleOnce(schedule_selector(JhFightLayer::delayShowWinLayer), 1.5f);
		}
		else
		{
			int totalnpc = JhGlobalData::map_maps[m_addrid].npcs.size();
			m_npcid = JhGlobalData::map_maps[m_addrid].npcs[totalnpc - continuefight];
			continuefight--;
			updateFightNextNpc();
		}

	}
	else
	{
		isWin = true;
		this->scheduleOnce(schedule_selector(JhFightLayer::delayShowWinLayer), 1.5f);
	}
}

int JhFightLayer::getNpcHurt()
{
	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	int skilltype = checkNpcSkill();
	if (isBossS6)
	{
		npchurt = npchurt * (100 - JhGlobalData::map_npcs[m_npcid].skilleffect) / 100;
	}

	return npchurt;
}

int JhFightLayer::getHeroHurt()
{
	int curheroDf = g_hero->getTotalDf();

	int herohurt = npcatk - curheroDf;

	float fminack = 0.1f * npcatk;
	int intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;

	if (isHeroS6)
	{
		herohurt = herohurt * (100 - JhGlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect) / 100;
	}
	return herohurt;
}

void JhFightLayer::skillHeroComboAtk(float dt)
{
	int count = JhGlobalData::map_gfskills[S_SKILL_3].leftval;
	int c = getNpcHurt();

	int totalcount = 0;
	int skilltype = checkHeroSkill(H_WG);
	if (skilltype == S_SKILL_3)
	{
		if (g_hero->getAtrByType(H_WG)->count > 0)
		{
			totalcount = JhGlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].skilleffect;
		}
	}

	npchp -= c*(100 - (totalcount - count)*10) / 100;

	JhGlobalData::map_gfskills[S_SKILL_3].leftval--;

	if (npchp <= 0)
	{
		npchp = 0;
		this->unschedule(schedule_selector(JhFightLayer::skillHeroComboAtk));
	}
	updateNpcLife();
}

void JhFightLayer::skillNpcComboAtk(float dt)
{
	int totalcount = JhGlobalData::map_npcs[m_npcid].skilleffect;
	int count = JhGlobalData::map_npcs[m_npcid].skillleftval;
	int c = getHeroHurt();
	float curheroHp = g_hero->getLifeValue();
	curheroHp -= c*(100 - (totalcount - count) * 10) / 100;
	JhGlobalData::map_npcs[m_npcid].skillleftval--;
	if (curheroHp <= 0)
	{
		curheroHp = 0;
		this->unschedule(schedule_selector(JhFightLayer::skillHeroComboAtk));
	}
	g_hero->setLifeValue(curheroHp);
	checkHeroLife(0);
}

void JhFightLayer::delayHeroFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	bool isnormalAct = true;
	isBossS6 = false;
	isHeroS6 = false;
	int npchurt = getNpcHurt();

	int skilltype = checkNpcSkill();

	if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
	{
		JhGlobalData::map_npcs[m_npcid].skillleftval--;
	}
	float curheroHp = g_hero->getLifeValue();
	if (isHeroAct != 1)
	{
		if (curheroHp <= 0)
		{
			curheroHp = 0;
			g_hero->setLifeValue(curheroHp);
			checkHeroLife(0);
			return;
		}

		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			isnormalAct = false;
			showNpcSkill(skilltype);

			curheroHp -= getHeroHurt() * 3 / 10;
			if (curheroHp < 0)
				curheroHp = 0;
			g_hero->setLifeValue(curheroHp);
			checkHeroLife(0);
		}
		else if (skilltype == S_SKILL_2)
		{
			isnormalAct = false;
			showNpcSkill(skilltype);
			this->scheduleOnce(schedule_selector(JhFightLayer::delayBossFight), 1.5f);
			return;
		}
		skilltype = checkNpcSkill();
		if (skilltype == S_SKILL_6)
		{
			isnormalAct = false;
			isBossS6 = true;
			npchurt = getNpcHurt();
			showNpcSkill(skilltype);
		}
		else if (skilltype == S_SKILL_7)
		{
			isnormalAct = false;
			showNpcSkill(skilltype);
			int herolosthp = curheroHp * JhGlobalData::map_npcs[m_npcid].skilleffect / 100;
			
			curheroHp -= herolosthp;
			if (curheroHp < 0)
				curheroHp = 0;
			checkHeroLife(0);
			npchp += herolosthp;
			updateNpcLife();
		}

	}

	isHeroAct = -1;
	isNpcAct = - 1;

	int critrnd = g_hero->getCritRate() * 100;

	if (checkHeroSkill(H_WG) == S_SKILL_4)
	{
		showHeroSkill(S_SKILL_4);
		if (g_hero->getAtrByType(H_WG)->count > 0)
		{
			critrnd += JhGlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].skilleffect * 100;
		}
		isnormalAct = false;
	}

	int npcdodgernd = JhGlobalData::map_npcs[m_npcid].dodge * 100;
	if (checkNpcSkill() == S_SKILL_8)
	{
		isnormalAct = false;
		showNpcSkill(S_SKILL_8);

		npcdodgernd += JhGlobalData::map_npcs[m_npcid].skilleffect * 100;
	}

	int r = JhGlobalData::createRandomNum(10000);
	if (r < critrnd)
	{
		isHeroAct = 0;
		npchurt *= 2;
		std::string tmpstr = "ui/crit.png";
		npcactimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		npcactimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		npcactimg->setVisible(true);
		npcactimg->setOpacity(200);
		npcactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		npcactimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", npchurt);
		npccritfnt->setString(tmpstr);
		npccritfnt->setVisible(true);
		isnormalAct = false;
	}
	else if (r < npcdodgernd)
	{
		isNpcAct = 1;
		std::string imgstr = "ui/dodge.png";
		npcactimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		npcactimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		npcactimg->setVisible(true);
		npccritfnt->setVisible(false);
		npcactimg->setOpacity(200);
		npcactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		npcactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));
		isnormalAct = false;

	}

	if (isNpcAct != 1)
	{
		npchp -= npchurt;
		if (npchp < 0)
			npchp = 0;
	}

	updateNpcLife();

	showFightWord(0, npchurt);

	if (npchp <= 0)// NPC dead 胜利
	{
		npcDie();
	}
	else
	{
		if (checkHeroSkill(H_WG) == S_SKILL_3)
		{
			int count = JhGlobalData::map_gfskills[S_SKILL_3].leftval;
			if (count > 0)
			{
				showHeroSkill(S_SKILL_3);
				this->schedule(schedule_selector(JhFightLayer::skillHeroComboAtk), 0.3f, count - 1, 0.2f);
				isnormalAct = false;
			}
		}
		this->scheduleOnce(schedule_selector(JhFightLayer::delayBossFight), 1.5f);//延迟显示NPC 攻击，主要文字显示，需要看一下，所以延迟下
	}

	if (isnormalAct)
	{
		//ActionInterval* ac1 = Spawn::create(Show::create(), FadeIn::create(0.15f), EaseSineIn::create(ScaleTo::create(0.15f, 1)), NULL);
		//npchurticon->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	}
}

void JhFightLayer::delayBossFight(float dt)
{
	if (isecapeok)//逃跑成功
		return;

	isBossS6 = false;
	isHeroS6 = false;
	bool isnormalAct = true;
	float curheroHp = g_hero->getLifeValue();

	int herohurt = getHeroHurt();

	int skilltype = checkHeroSkill(H_WG);
    
    if (skilltype == S_SKILL_1 || skilltype == S_SKILL_2 || skilltype == S_SKILL_5)
    {
       JhGlobalData::map_gfskills[skilltype].leftval--;
    }
    
	if (isNpcAct != 1)
	{
		if (npchp <= 0)
		{
			npcDie();
			return;
		}

		
		if (skilltype == S_SKILL_1 || skilltype == S_SKILL_5)
		{
			isnormalAct = false;
			showHeroSkill(skilltype);

			npchp -= getNpcHurt() * 3 / 10;
			if (npchp < 0)
				npchp = 0;
			updateNpcLife();

			if (npchp <= 0)
			{
				npcDie();
				return;
			}
		}
		else if (skilltype == S_SKILL_2)
		{
			isnormalAct = false;
			showHeroSkill(skilltype);
			this->scheduleOnce(schedule_selector(JhFightLayer::delayHeroFight), 1.5f);
			return;
		}
		skilltype = checkHeroSkill(H_NG);
		if (skilltype == S_SKILL_6)
		{
			isnormalAct = false;
			isHeroS6 = true;
			herohurt = getHeroHurt();
			showHeroSkill(skilltype);
		}
		else if (skilltype == S_SKILL_7)
		{
			isnormalAct = false;
			showHeroSkill(skilltype);
			int npclosthp = 0;
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				npclosthp = npchp * JhGlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect / 100;
			}
			npchp -= npclosthp;
			if (npchp < 0)
				npchp = 0;
			updateNpcLife();
			g_hero->setLifeValue(curheroHp + npclosthp);
			checkHeroLife(0);
			if (npchp <= 0)
			{
				npcDie();
				return;
			}
		}
		
	}


	isHeroAct = -1;
	isNpcAct = -1;

	int dodgernd = g_hero->getdodgeRate() * 100;
	if (checkHeroSkill(H_NG) == S_SKILL_8)
	{
		isnormalAct = false;
		showHeroSkill(S_SKILL_8);
		if (g_hero->getAtrByType(H_NG)->count > 0)
		{
			dodgernd += JhGlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].skilleffect * 100;
		}
		
	}

	int npccritrnd = JhGlobalData::map_npcs[m_npcid].crit * 100;

	if (checkNpcSkill() == S_SKILL_4)
	{
		showNpcSkill(S_SKILL_4);

		npccritrnd += JhGlobalData::map_npcs[m_npcid].skilleffect * 100;
		isnormalAct = false;
	}

	int r = JhGlobalData::createRandomNum(10000);
	if (r < npccritrnd)
	{
		isNpcAct = 0;
		herohurt *= 2;
		std::string tmpstr = "ui/crit.png";
		heroactimg->loadTexture(tmpstr, cocos2d::ui::TextureResType::PLIST);
		heroactimg->setContentSize(Sprite::createWithSpriteFrameName(tmpstr)->getContentSize());
		heroactimg->setVisible(true);
		herocritfnt->setVisible(true);
		heroactimg->setOpacity(200);
		heroactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		heroactimg->runAction(Sequence::create(ac1, DelayTime::create(1.0f), Hide::create(), NULL));
		tmpstr = StringUtils::format("%d", herohurt);
		herocritfnt->setString(tmpstr);
		isnormalAct = false;
	}
	else if (r < dodgernd)
	{
		isHeroAct = 1;
		std::string imgstr = "ui/dodge.png";
		heroactimg->loadTexture(imgstr, cocos2d::ui::TextureResType::PLIST);
		heroactimg->setContentSize(Sprite::createWithSpriteFrameName(imgstr)->getContentSize());
		heroactimg->setVisible(true);
		herocritfnt->setVisible(false);
		heroactimg->setOpacity(200);
		heroactimg->setScale(3);
		ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
		heroactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.8f), Hide::create(), NULL));
		isnormalAct = false;
	}

	if (isHeroAct != 1)
	{
		curheroHp -= herohurt;
	}

	showFightWord(1, herohurt);

	if (curheroHp < 0.0f)
		curheroHp = 0.0f;

	g_hero->setLifeValue(curheroHp);
	checkHeroLife(0);

	if (g_hero->getLifeValue() > 0.0f)
	{

		if (checkNpcSkill() == S_SKILL_3)
		{
			int count = JhGlobalData::map_npcs[m_npcid].skillleftval;
			if (count > 0)
			{
				showNpcSkill(S_SKILL_3);
				this->schedule(schedule_selector(JhFightLayer::skillNpcComboAtk), 0.3f, count - 1, 0.2f);
				isnormalAct = false;
			}
		}

		this->scheduleOnce(schedule_selector(JhFightLayer::delayHeroFight), 1.5f);

		float f1maxlife = g_hero->getMaxLifeValue() * 0.05f;
		if (herohurt >= (int)f1maxlife)//受到大于10%伤害
		{
			int r = JhGlobalData::createRandomNum(100);
			if (r < 30)//30%概率收内伤
			{
				float curvalue = g_hero->getInnerinjuryValue() - 5.0f;
				if (curvalue < 0.0f)
					curvalue = 0.0f;
				g_hero->setInnerinjuryValue(curvalue);

			}
			else//70%概率收外伤
			{
				float curvalue = g_hero->getOutinjuryValue() - 5.0f;
				if (curvalue < 0.0f)
					curvalue = 0.0f;
				g_hero->setOutinjuryValue(curvalue);
			}
		}

		if (isnormalAct)
		{
			//ActionInterval* ac1 = Spawn::create(Show::create(), FadeIn::create(0.15f), EaseSineIn::create(ScaleTo::create(0.15f, 1)), NULL);
			//herohurticon->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
		}
	}
	else
	{
		m_escapebtn->setTitleText(JhCommonFuncs::gbk2utf("返回"));
		m_escapebtn->setTag(1);

#ifdef ANALYTICS
		std::string deathstr = StringUtils::format("d-%s", m_npcid.c_str());
		JhAnalyticUtil::onEvent(m_npcid.c_str());
#endif
	}
	
}

void JhFightLayer::delayShowWinLayer(float dt)
{
	m_escapebtn->setTitleText(JhCommonFuncs::gbk2utf("返回"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(1);

	Winlayer* layer = Winlayer::create(m_addrid, m_npcid);
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(layer, 10, "Winlayer");

	if (continuefight > 0)
	{
		winnpcount++;
		winProperCount++;
		int totalnpc = JhGlobalData::map_maps[m_addrid].npcs.size();
		m_npcid = JhGlobalData::map_maps[m_addrid].npcs[totalnpc - continuefight];
		continuefight--;
	}
	isWin = false;
	if (m_addrid.compare("m1-6") == 0)
		this->removeFromParentAndCleanup(true);
}

void JhFightLayer::showFightWord(int type, int value)
{
	std::string wordstr;
	int size = 0;
	int r = 0;
	if (type == 0)//
	{
		std::string herowordstr;
		if (g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)//是否有武器
		{
			int showr = JhGlobalData::createRandomNum(2);

			std::string wstrid = g_hero->getAtrByType(H_WEAPON)->strid;
			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				if (showr == 0)
					isUseWg = true;
				if (m_npcid.compare("n003") == 0 || m_npcid.compare("n002") == 0)
					isUseWg = false;
			}
			if (isUseWg)
			{
				herowordstr = getGfFightStr();
			}
			else
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc1[extype - 1]) / sizeof(herofightdesc1[extype - 1][0]);
				r = JhGlobalData::createRandomNum(size);
				wordstr = herofightdesc1[extype - 1][r];
				herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), JhGlobalData::map_allResource[wstrid].cname.c_str(), JhGlobalData::map_npcs[m_npcid].name);
			}

			
			int r = JhGlobalData::createRandomNum(100);
			if (r < 40)
			{
				g_hero->getAtrByType(H_WEAPON)->goodvalue--;
				if (g_hero->getAtrByType(H_WEAPON)->goodvalue <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("你的").c_str(), JhGlobalData::map_allResource[wstrid].cname.c_str(), JhCommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (g_hero->getAtrByType(H_WEAPON)->goodvalue == 20 || g_hero->getAtrByType(H_WEAPON)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", JhGlobalData::map_allResource[wstrid].cname.c_str(), JhCommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType(H_WEAPON)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}
		}
		else//没有武器
		{

			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				isUseWg = true;
			}
			if (isUseWg)
			{
				herowordstr = getGfFightStr();
			}
			else
			{
				size = sizeof(herofightdesc) / sizeof(herofightdesc[0]);
				r = JhGlobalData::createRandomNum(size);
				wordstr = herofightdesc[r];
				herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(wordstr.c_str()).c_str(), g_hero->getMyName().c_str(), JhGlobalData::map_npcs[m_npcid].name);
			}
		}
		
		if (m_npcid.compare("n003") == 0)
			herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(herofightworfword[0].c_str()).c_str(), g_hero->getMyName().c_str(), JhGlobalData::map_npcs[m_npcid].name);
		else if (m_npcid.compare("n002") == 0)
			herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(herofightRabbitword[0].c_str()).c_str(), g_hero->getMyName().c_str(), JhGlobalData::map_npcs[m_npcid].name);

		checkWordLblColor(herowordstr);

		if (isUseWg)
		{
			int r1 = JhGlobalData::createRandomNum(4);
			wordstr = bossgetherowg[r1];
			herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(wordstr.c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, value);
		}
		else
		{
			if (g_hero->getAtrByType(H_WEAPON)->count > 0)
			{
				int extype = g_hero->getAtrByType(H_WEAPON)->extype;
				size = sizeof(herofightdesc2[extype - 1]) / sizeof(herofightdesc2[extype - 1][0]);
				r = JhGlobalData::createRandomNum(size);
				wordstr = herofightdesc2[extype - 1][r];
				herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(wordstr.c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, value);
			}
			else
			{
				herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(herofightdesc3[0].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, value);
			}
		}

		if (m_npcid.compare("n003") == 0)
			herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(herofightworfword2[0].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, value);
		else if (m_npcid.compare("n002") == 0)
			herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(herofightRabbitword2[0].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, value);

		if (isNpcAct == 1)//npc闪避
		{
			int dcount = sizeof(npcdodgedesc) / sizeof(npcdodgedesc[0]);
			int dodgernd = JhGlobalData::createRandomNum(dcount);
			herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(npcdodgedesc[dodgernd].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name);

			if (m_npcid.compare("n003") == 0 || m_npcid.compare("n002") == 0)
				herowordstr = StringUtils::format(JhCommonFuncs::gbk2utf(worfdodgedesc.c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name);
		}

		checkWordLblColor(herowordstr);

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_ATTACK);
	}
	else//
	{
		std::string bosswordstr;

		if (g_hero->getAtrByType(H_ARMOR)->count > 0 && g_hero->getAtrByType(H_ARMOR)->goodvalue > 0)//是有有防具
		{
			int r = JhGlobalData::createRandomNum(100);
			std::string astrid = g_hero->getAtrByType(H_ARMOR)->strid;
			if (r < 40)
			{
				g_hero->getAtrByType(H_ARMOR)->goodvalue--;

				if (g_hero->getAtrByType(H_ARMOR)->goodvalue <= 0)
				{
					std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("你的").c_str(), JhGlobalData::map_allResource[astrid].cname.c_str(), JhCommonFuncs::gbk2utf("已毁坏！！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
				}
				else if (g_hero->getAtrByType(H_ARMOR)->goodvalue == 20 || g_hero->getAtrByType(H_ARMOR)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", JhGlobalData::map_allResource[astrid].cname.c_str(), JhCommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType(H_ARMOR)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}

			size = sizeof(bossfight1) / sizeof(bossfight1[0]);
			r = JhGlobalData::createRandomNum(size);
			wordstr = bossfight1[r];
			bosswordstr = StringUtils::format(JhCommonFuncs::gbk2utf(wordstr.c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), JhGlobalData::map_allResource[astrid].cname.c_str(), value);
		}
		else//没有防具
		{
			size = sizeof(bossfight) / sizeof(bossfight[0]);
			r = JhGlobalData::createRandomNum(size);
			wordstr = bossfight[r];
			bosswordstr = StringUtils::format(JhCommonFuncs::gbk2utf(wordstr.c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		}

		if (m_npcid.compare("n003") == 0)
			bosswordstr = StringUtils::format(JhCommonFuncs::gbk2utf(worffight[0].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);
		else if (m_npcid.compare("n002") == 0)
			bosswordstr = StringUtils::format(JhCommonFuncs::gbk2utf(rabbitfightword[0].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str(), value);

		if (isHeroAct == 1)
		{
			int dcount = sizeof(herododgedesc) / sizeof(herododgedesc[0]);
			int dodgernd = JhGlobalData::createRandomNum(dcount);
			bosswordstr = StringUtils::format(JhCommonFuncs::gbk2utf(herododgedesc[dodgernd].c_str()).c_str(), JhGlobalData::map_npcs[m_npcid].name, g_hero->getMyName().c_str());
		}
		checkWordLblColor(bosswordstr);

		isUseWg = false;
		if (g_hero->getHeadID() == 4)
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_HURT1);
		else
			SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_HURT0);
	}
}

void JhFightLayer::checkWordLblColor(std::string wordstr)
{
	Label* wordlbl = Label::createWithTTF(wordstr, "fonts/STXINGKA.TTF", 28);
	wordlbl->setLineBreakWithoutSpace(true);
	wordlbl->setMaxLineWidth(610);
	int index = 0;
	//getletter --是每个字，中文，英文都算一个字
	//UFT8 中文是3个字符所以除以3转换下
	while (wordlbl->getLetter(index) != NULL)
	{
		wordlbl->getLetter(index)->setColor(Color3B::BLACK);
		index++;
	}

	//NPC名称颜色 红色
	std::map<std::string, NpcData>::iterator it;
	for (it = JhGlobalData::map_npcs.begin(); it != JhGlobalData::map_npcs.end(); ++it)
	{
		std::string npcname = JhGlobalData::map_npcs[it->first].name;
		std::size_t findpos;
		std::string temp = wordstr;
		int count = 0;
		while (1){
			findpos = temp.find(npcname);
			if (findpos != std::string::npos){
				temp = temp.substr(findpos + npcname.length(), temp.length() - npcname.length());
				int sindex = (wordstr.length() - temp.length() - npcname.length()) / 3;
				int len = npcname.length() / 3;
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
				}
			}
			else 
				break;
		}

		//std::size_t findpos = wordstr.find(npcname);
		//if (findpos != std::string::npos)
		//{
		//	int sindex = findpos / 3;
		//	int len = npcname.size() / 3;
		//	for (int i = sindex; i < sindex + len; i++)
		//	{
		//		wordlbl->getLetter(i)->setColor(Color3B(230,35,35));
		//	}
		//}
	}
	//角色名称颜色 绿色

	std::size_t findpos;
	std::string temp = wordstr;
	std::string heroname = g_hero->getMyName();
	while (1){
		findpos = temp.find(g_hero->getMyName());
		if (findpos != std::string::npos){
			temp = temp.substr(findpos + heroname.length(), temp.length() - heroname.length());
			int sindex = (wordstr.length() - temp.length() - heroname.length()) / 3;
			int len = heroname.length() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
			}
		}
		else
			break;
	}
	//武器，功法名称颜色 紫色
	std::map<std::string, EquipData>::iterator ite;
	for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
	{
		std::string ename = JhGlobalData::map_allResource[ite->first].cname;
		std::size_t findpos;
		std::string temp = wordstr;
		while (1){
			findpos = temp.find(ename);
			if (findpos != std::string::npos){
				temp = temp.substr(findpos + ename.length(), temp.length() - ename.length());
				int sindex = (wordstr.length() - temp.length() - ename.length()) / 3;
				int len = ename.length() / 3;
				for (int i = sindex; i < sindex + len; i++)
				{
					wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
				}
			}
			else
				break;
		}

		//std::size_t findpos = wordstr.find(ename);
		//if (findpos != std::string::npos)
		//{
		//	int sindex = findpos / 3;
		//	int len = ename.size() / 3;
		//	for (int i = sindex; i < sindex + len; i++)
		//	{
		//		wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
		//	}
		//}
	}
	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		GFTrickData gftrickdata = JhGlobalData::map_gftricks[g_hero->getAtrByType(H_WG)->strid];
		int mygftricksize = gftrickdata.snames.size();
		
		for (int n = 0; n < mygftricksize; n++)
		{
			std::string gfname = gftrickdata.snames[n];
			std::size_t findpos;
			std::string temp = wordstr;
			while (1){
				findpos = temp.find(gfname);
				if (findpos != std::string::npos){
					temp = temp.substr(findpos + gfname.length(), temp.length() - gfname.length());
					int sindex = (wordstr.length() - temp.length() - gfname.length()) / 3;
					int len = gfname.length() / 3;
					for (int i = sindex; i < sindex + len; i++)
					{
						wordlbl->getLetter(i)->setColor(Color3B(171, 34, 210));
					}
				}
				else
					break;
			}
		}
	}

	//数字颜色 红色
	//目前只实现了一段中只能改变一组数字的颜色，并且数字要在上面 关键字的后面
	int sindex = -1;
	findpos = 0;
	for (unsigned int i = 0; i < wordstr.length(); i++)
	{
		char a = wordstr[i];
		if (wordstr[i] >= '0' && wordstr[i] <= '9')
		{
			sindex = i / 3;
			findpos = i;
			wordlbl->getLetter(sindex)->setColor(Color3B(230, 35, 35));
			break;
		}
	}

	if (sindex > 0)
	{
		//找到地一个数字后，往后再找4位
		for (int i = 1; i <= 4; i++)
		{
			char a = wordstr[findpos + i];
			if (wordstr[findpos + i] >= '0' && wordstr[findpos + i] <= '9')
			{
				wordlbl->getLetter(sindex + i)->setColor(Color3B(230, 35, 35));
			}
		}
	}

	m_fihgtScorll->addEventLabel(wordlbl);
}

std::string JhFightLayer::getGfFightStr()
{
	std::string retstr;

	int r1 = JhGlobalData::createRandomNum(2);
	int r2 = JhGlobalData::createRandomNum(4);

	std::string gfstr = g_hero->getAtrByType(H_WG)->strid;
	GFTrickData ssdata = JhGlobalData::map_gftricks[g_hero->getAtrByType(H_WG)->strid];
	int r3 = JhGlobalData::createRandomNum(ssdata.snames.size());
	std::string gfsname = ssdata.snames[r3];
	std::string	wordstr = JhCommonFuncs::gbk2utf(herousewgfight[r1][r2].c_str());
	if (r1 == 0)
		retstr = StringUtils::format(wordstr.c_str(), g_hero->getMyName().c_str(), gfsname.c_str(), JhGlobalData::map_npcs[m_npcid].name);
	else
		retstr = StringUtils::format(wordstr.c_str(), g_hero->getMyName().c_str(), JhGlobalData::map_npcs[m_npcid].name, gfsname.c_str(), gfsname.c_str(), JhGlobalData::map_npcs[m_npcid].name);

	return retstr;
}

void JhFightLayer::checkHeroLife(float dt)
{
	std::string hpstr = StringUtils::format("%d/%d", (int)g_hero->getLifeValue(), (int)g_hero->getMaxLifeValue());
	herohpvaluetext->setString(hpstr);
	float herohppercent = 100 * g_hero->getLifeValue() / g_hero->getMaxLifeValue();
	herohpbar->setPercent(herohppercent);

	MyProgressTo * fromto = MyProgressTo::create(0.5f, herohppercent);
	herohpbar2->runAction(fromto);
}

void JhFightLayer::updateNpcLife()
{
	//NPC血量显示
	std::string hpstr = StringUtils::format("%d/%d", npchp, npcmaxhp);
	npchpvaluetext->setString(hpstr);

	//NCP血量进度
	int npchppercent = 100 * npchp / npcmaxhp;
	npchpbar->setPercent(npchppercent);
	MyProgressTo * to = MyProgressTo::create(0.5f, npchppercent);
	npchpbar2->runAction(to);
}

void JhFightLayer::nextFightNpc(float dt)
{
	this->unschedule(schedule_selector(JhFightLayer::delayHeroFight));

	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);

	npcnametxt->setString(JhGlobalData::map_npcs[m_npcid].name);

	npcmaxhp = JhGlobalData::map_npcs[m_npcid].life;
	npchp = npcmaxhp;

	npcatk = JhGlobalData::map_npcs[m_npcid].atk;
	npcdf = JhGlobalData::map_npcs[m_npcid].df;

	updateNpcLife();

	resetSkills();

	this->scheduleOnce(schedule_selector(JhFightLayer::delayHeroFight), 1.5f);
}

void JhFightLayer::continueChallenge()
{
	if (winnpcount % 3 == 0)
	{
		m_escapebtn->setTitleText(JhCommonFuncs::gbk2utf("返回"));
		m_escapebtn->setEnabled(true);
		m_escapebtn->setTag(1);
		showChallengeCountLayer(false);
	}
	else
	{
		updateFightNextNpc();
	}
}

void JhFightLayer::updateFightNextNpc()
{
	isecapeok = false;
	m_escapebtn->setTitleText(JhCommonFuncs::gbk2utf("逃跑"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(0);
	checkHeroLife(0);
	this->scheduleOnce(schedule_selector(JhFightLayer::nextFightNpc), 0.5f);
}

void JhFightLayer::reviveContinueChallege()
{
	isecapeok = false;
	m_escapebtn->setTitleText(JhCommonFuncs::gbk2utf("逃跑"));
	m_escapebtn->setEnabled(true);
	m_escapebtn->setTag(0);
	checkHeroLife(0);
	this->scheduleOnce(schedule_selector(JhFightLayer::delayHeroFight), 1.5f);
}

void JhFightLayer::showChallengeCountLayer(bool isRevive)
{
	JhChallengeCountLayer* layer = JhChallengeCountLayer::create(&winProperCount, winnpcount, isRevive);
	g_gameLayer->addChild(layer, 4, "challengecountlayer");
}

int JhFightLayer::checkHeroSkill(HeroAtrType gftype)
{
	int ret = S_SNONE;
	if (g_hero->getAtrByType(gftype)->count > 0)
	{
		int lv = g_hero->getAtrByType(gftype)->lv;
		std::string gfstr = g_hero->getAtrByType(gftype)->strid;
		int stype = JhGlobalData::map_wgngs[gfstr].skilltype;
		if (JhGlobalData::map_gfskills[stype].leftval > 0)
		{
			ret =  stype;
		}
		else
		{
			if (stype > S_SNONE)
			{
				int rand = JhGlobalData::map_wgngs[gfstr].vec_skrnd[lv] * 100;
				int r = JhGlobalData::createRandomNum(10000);
				if (r < rand)
				{
					if (stype != S_SKILL_4 && stype != S_SKILL_6 && stype != S_SKILL_7 && stype != S_SKILL_8)
						JhGlobalData::map_gfskills[stype].leftval = JhGlobalData::map_wgngs[gfstr].skilleffect;
					ret = stype;
				}
			}
		}
	}

	if (ret == S_SKILL_6)
		isHeroS6 = true;
	return ret;
}

int JhFightLayer::checkNpcSkill()
{
	int ret = S_SNONE;

	int stype = JhGlobalData::map_npcs[m_npcid].skilltype;
	if (JhGlobalData::map_npcs[m_npcid].skillleftval > 0)
	{
		ret = stype;
	}
	else
	{
		if (stype > S_SNONE)
		{
			int rand = JhGlobalData::map_npcs[m_npcid].skillrnd * 100;
			int r = JhGlobalData::createRandomNum(10000);
			if (r < rand)
			{
				if (stype != S_SKILL_4 && stype != S_SKILL_6 && stype != S_SKILL_7 && stype != S_SKILL_8)
					JhGlobalData::map_npcs[m_npcid].skillleftval = JhGlobalData::map_npcs[m_npcid].skilleffect;
				ret = stype;
			}
		}
	}

	if (ret == S_SKILL_6)
		isBossS6 = true;
	return ret;
}

void JhFightLayer::showNpcSkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showHeroTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showNpcTextAmin(str);
	}
}


void JhFightLayer::showHeroSkill(int skilltype)
{
	if (skilltype >= S_SKILL_1 && skilltype <= S_SKILL_5)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showNpcTextAmin(str);
	}
	else if (skilltype >= S_SKILL_6 && skilltype <= S_SKILL_8)
	{
		std::string str = StringUtils::format("ui/skill%dtext.png", skilltype);
		showHeroTextAmin(str);
	}
}

void JhFightLayer::showHeroTextAmin(std::string filename)
{
	heroactimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	heroactimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	heroactimg->setVisible(true);
	herocritfnt->setVisible(false);
	heroactimg->setOpacity(200);
	heroactimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	heroactimg->runAction(Sequence::create(ac1, Shake::create(0.2f, 20, 1), DelayTime::create(0.6f), Hide::create(), NULL));
	herocritfnt->setVisible(false);
}


void JhFightLayer::showNpcTextAmin(std::string filename)
{
	npcactimg->loadTexture(filename, cocos2d::ui::TextureResType::PLIST);
	npcactimg->setContentSize(Sprite::createWithSpriteFrameName(filename)->getContentSize());
	npcactimg->setVisible(true);
	npcactimg->setOpacity(200);
	npcactimg->setScale(3);
	ActionInterval* ac1 = Spawn::create(FadeIn::create(0.1f), EaseSineIn::create(ScaleTo::create(0.1f, 1)), NULL);
	npcactimg->runAction(Sequence::create(ac1, DelayTime::create(0.8f), Hide::create(), NULL));
	npccritfnt->setVisible(false);
}

void JhFightLayer::restartFightNpc(std::string npcid)
{
	m_npcid = npcid;
	updateFightNextNpc();
}

void JhFightLayer::resetSkills()
{
	for (int i = S_SKILL_1; i <= S_SKILL_8; i++)
	{
		JhGlobalData::map_gfskills[i].leftval = 0;
		JhGlobalData::map_gfskills[i].fightPlayerleftval = 0;
	}

	JhGlobalData::map_npcs[m_npcid].skillleftval = 0;
}
