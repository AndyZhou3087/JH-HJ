#include "JhNpcTalkLayer.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhGameScene.h"

JhNpcTalkLayer::JhNpcTalkLayer()
{
	m_wordindex = 0;
	m_wordcount = 0;
	lasttalklbl = NULL;
}
JhNpcTalkLayer::~JhNpcTalkLayer()
{

}

JhNpcTalkLayer* JhNpcTalkLayer::create(std::vector<std::string> vec_words, std::string npcid)
{
	JhNpcTalkLayer *pRet = new JhNpcTalkLayer();
	if (pRet && pRet->init(vec_words, npcid))
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

bool JhNpcTalkLayer::init(std::vector<std::string> vec_words, std::string npcid)
{
	if (!Layer::init())
	{
		return false;
	}

	m_wordslist = vec_words;
	auto colorlayer = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(colorlayer);

	Node* csbnode = CSLoader::createNode("jhnpctalkLayer2.csb");
	this->addChild(csbnode, 2);

	m_talknode = (cocos2d::ui::Widget*)csbnode->getChildByName("node");

	npcimg = (cocos2d::ui::ImageView*)m_talknode->getChildByName("npcimg");
	std::string npcimgstr = StringUtils::format("ui/t%s.png", npcid.c_str());
	npcimg->loadTexture(npcimgstr, cocos2d::ui::Widget::TextureResType::PLIST);
	npcimg->setOpacity(0);

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)npcimg->getChildByName("npcname");
	npcname->setString(JhGlobalData::map_npcs[npcid].name);

	heroimg = (cocos2d::ui::ImageView*)m_talknode->getChildByName("heroimg");
	cocos2d::ui::Text* m_heroname = (cocos2d::ui::Text*)heroimg->getChildByName("heroname");
	m_heroname->setString(heroname[g_hero->getHeadID() - 1]);

	std::string heroidstr = StringUtils::format("ui/fhero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);

	cocos2d::ui::Text* wordlbl = (cocos2d::ui::Text*)m_talknode->getChildByName("text");
	wordlbl->setVisible(false);

	heroimg->setOpacity(0);
	npcimg->setOpacity(255);

	checkWordLblColor(vec_words[0]);

	//float dt = 0.0f;
	//for (unsigned int i = 0; i < vec_words.size(); i++)
	//{
	//	this->scheduleOnce(schedule_selector(JhNpcTalkLayer::showTypeText), dt);
	//	dt += vec_words[i].size() / 3 * 0.1f + 1.0f;
	//}

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{

		return true;
	};
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		if (isShowWord)
		{
			if (m_wordslist.size() <= 0)
				return;

			if (m_wordslist[m_wordindex].length() > 0)
			{
				fastShowWord();
				return;
			}
		}
		m_wordindex++;

		int size = m_wordslist.size();

		if (m_wordindex >= size)
		{
			this->removeFromParentAndCleanup(true);
			return;
		}
		else
		{
			checkWordLblColor(vec_words[m_wordindex]);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhNpcTalkLayer::checkWordLblColor(std::string wordstr)
{
	m_wordlbl = Label::createWithTTF(wordstr, "fonts/SIMHEI.TTF", 20);
	m_wordlbl->setLineBreakWithoutSpace(true);
	m_wordlbl->setAnchorPoint(Vec2(0, 1));
	m_wordlbl->setHorizontalAlignment(CCTextAlignment::LEFT);
	m_wordlbl->setVerticalAlignment(CCVerticalTextAlignment::TOP);
	m_wordlbl->setMaxLineWidth(450);
	m_wordlbl->setPosition(Vec2(-315,-112));
	m_talknode->addChild(m_wordlbl, 0, "talklbl");


	int index = 0;
	while (m_wordlbl->getLetter(index) != NULL)
	{
		m_wordlbl->getLetter(index)->setColor(Color3B::WHITE);
		m_wordlbl->getLetter(index)->setScale(0);
		index++;
	}

	std::map<std::string, NpcData>::iterator it;
	for (it = JhGlobalData::map_npcs.begin(); it != JhGlobalData::map_npcs.end(); ++it)
	{
		std::string npcname = JhGlobalData::map_npcs[it->first].name;
		std::size_t findpos = wordstr.find(npcname);
		if (findpos != std::string::npos)
		{
			int sindex = findpos / 3;
			int len = npcname.size() / 3;
			for (int i = sindex; i < sindex + len; i++)
			{
				m_wordlbl->getLetter(i)->setColor(Color3B(230, 35, 35));
			}
		}
	}
	std::size_t findpos = wordstr.find(g_hero->getMyName());
	if (findpos != std::string::npos)
	{
		m_wordlbl->setPosition(Vec2(-145, -112));
		int sindex = findpos / 3;
		int len = g_hero->getMyName().size() / 3;
		for (int i = sindex; i < sindex + len; i++)
		{
			m_wordlbl->getLetter(i)->setColor(Color3B(27, 141, 0));
		}
	}

	this->scheduleOnce(schedule_selector(JhNpcTalkLayer::showTypeText), 0.1f);
}

void JhNpcTalkLayer::showTypeText(float dt)
{
	if (lasttalklbl != NULL)
	{
		lasttalklbl->setVisible(false);
		lasttalklbl->removeFromParentAndCleanup(true);
		lasttalklbl = NULL;
	}
	if (m_wordindex % 2 == 0)
	{
		heroimg->setOpacity(0);
		npcimg->setOpacity(255);
	}
	else
	{
		heroimg->setOpacity(255);
		npcimg->setOpacity(0);
	}
	lasttalklbl = m_wordlbl;
	m_wordlbl->schedule([&](float dt){
		isShowWord = true;
		m_wordcount += 3;
		int letterindex = m_wordcount / 3 - 1;
		m_wordlbl->getLetter(letterindex)->setScale(1.0f);
		int len = m_wordlbl->getString().length();
		if (m_wordcount >= len)
		{
			//m_wordindex++;
			m_wordcount = 0;
			isShowWord = false;
			m_wordlbl->unschedule("schedule_typecallback");
			//int size = m_wordslist.size();
			//if (m_wordindex < size)
			//{
			//	checkWordLblColor(m_wordslist[m_wordindex]);
			//}
		}

	}, 0.1f, "schedule_typecallback");
}

void JhNpcTalkLayer::fastShowWord()
{
	isShowWord = false;
	m_wordcount = 0;

	this->unscheduleAllCallbacks();
	m_wordlbl->unscheduleAllCallbacks();
	int index = 0;
	while (m_wordlbl->getLetter(index) != NULL)
	{
		m_wordlbl->getLetter(index)->setScale(1);
		index++;
	}
	int wordsize = m_wordslist.size();
	if (m_wordindex >= wordsize)
	{
		return;
	}
}