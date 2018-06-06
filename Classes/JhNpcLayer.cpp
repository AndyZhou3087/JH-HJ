#include "JhNpcLayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhFightLayer.h"
#include "JhGameScene.h"
#include "JhMapLayer.h"
#include "SoundManager.h"
#include "JhNewerGuideLayer.h"
#include "JhGameDataSave.h"
#include "JhExchangeLayer.h"
#include "JhHintBox.h"
#include "JhCannotTouchLayer.h"
#include "JhMyActionProgressTimer.h"
#include "JhRepairLayer.h"
#include "JhGiveLayer.h"
#include "JhNewerGuide2Layer.h"
#include "JhNpcTalkLayer.h"
#include "Winlayer.h"

std::string replacestr[] = {"少侠","小子","小兄弟","小伙子", "兄台"};
std::string areplacestr[] = {"女侠","小娘子","小姑娘","小姑娘","姑娘"};

int silvercost[] = { 1, 2, 2 };
JhNpcLayer::JhNpcLayer()
{

}


JhNpcLayer::~JhNpcLayer()
{
	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_MAP);

}

JhNpcLayer* JhNpcLayer::create(std::string addrname)
{
	JhNpcLayer *pRet = new JhNpcLayer();
	if (pRet && pRet->init(addrname))
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

bool JhNpcLayer::init(std::string addrid)
{
	m_csbnode = CSLoader::createNode("jhnpcLayer.csb");
	this->addChild(m_csbnode);

	m_npctalkbg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("npctalkbg");
	m_npctalkbg->setOpacity(0);

	m_addrstr = addrid;

	MapData mdata = JhGlobalData::map_maps[addrid];

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(mdata.cname);

	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.png", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onBack, this));

	m_scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");

	m_scrollview->setScrollBarEnabled(false);
	m_scrollview->setBounceEnabled(true);

	m_lastWxpPos = JhGlobalData::getWxbMapPos();
	m_lastDgqbPos = JhGlobalData::getDgqbMapPos();

	refreshNpcNode();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundManager::getInstance()->playBackMusic(SoundManager::MUSIC_ID_ENTER_MAPADDR);
	this->scheduleOnce(schedule_selector(JhNpcLayer::delayShowNewerGuide), 0.2f);
	this->schedule(schedule_selector(JhNpcLayer::checkUpateNpc), 1.0f);
	return true;
}

void JhNpcLayer::refreshNpcNode()
{
	MapData mdata = JhGlobalData::map_maps[m_addrstr];
	int ncpsize = mdata.npcs.size();

	int itemheight = 200;
	int innerheight = itemheight * ncpsize;
	int contentheight = m_scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollview->setInnerContainerSize(Size(m_scrollview->getContentSize().width, innerheight));

	/*cocos2d::ui::Widget* hintdown = (cocos2d::ui::Widget*)m_csbnode->getChildByName("hintdown");
	if (ncpsize > 2)
	{
		hintdown->runAction(RepeatForever::create(Blink::create(2, 2)));
	}
	else
	{
		hintdown->stopAllActions();
		hintdown->setVisible(false);
	}*/

	int scrollviewChildSize = m_scrollview->getChildrenCount();

	int lesscount = scrollviewChildSize - ncpsize;
	if (lesscount > 0)
	{
		for (int i = 0; i < lesscount; i++)
		{
			std::string removechildname = StringUtils::format("npcnode%d", ncpsize + i);
			m_scrollview->removeChildByName(removechildname);
			std::string keepchildname = StringUtils::format("npcnode%d", i);
			m_scrollview->getChildByName(keepchildname)->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
		}
	}
	else
	{
		for (int i = 0; i < scrollviewChildSize; i++)
		{
			std::string keepchildname = StringUtils::format("npcnode%d", i);
			m_scrollview->getChildByName(keepchildname)->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
		}
		for (int i = scrollviewChildSize; i < ncpsize; i++)
		{
			Node* npcitem = CSLoader::createNode("jhnpcNode.csb");
			npcitem->setPosition(Vec2(m_scrollview->getContentSize().width / 2, innerheight - i * itemheight - itemheight / 2));
			std::string childname = StringUtils::format("npcnode%d", i);
			npcitem->setTag(i);
			m_scrollview->addChild(npcitem, 0, childname);

			cocos2d::ui::ImageView* npchead = (cocos2d::ui::ImageView*)npcitem->getChildByName("npcicon");
			std::string npcheadstr = StringUtils::format("ui/%s.png", mdata.npcs[i].c_str());
			npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);
			npchead->setScale(0.6f);

			cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)npcitem->getChildByName("npcname");
			npcname->setString(JhGlobalData::map_npcs[mdata.npcs[i]].name);

			cocos2d::ui::ImageView* npcrsi = (cocos2d::ui::ImageView*)npcitem->getChildByName("npcrsi");

			int count = checkFightCount(mdata.npcs[i]);

			std::string rsistr;
			if (count > 0)
			{
				if (count <= 10)
					rsistr = "ui/npcrsi0.png";
				else
					rsistr = "ui/npcrsi1.png";
			}
			else
			{
				if (count >= -10)
					rsistr = "ui/npcrsi2.png";
				else
					rsistr = "ui/npcrsi1.png";
			}

			npcrsi->loadTexture(rsistr, cocos2d::ui::TextureResType::PLIST);

			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			talkbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onItemTalk, this));
			talkbtn->setTag(i);

			cocos2d::ui::Button* fightbtn = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
			fightbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onItemFight, this));

			cocos2d::ui::Button* masterbtn = (cocos2d::ui::Button*)npcitem->getChildByName("msterbtn");
			masterbtn->setTag(i);
			masterbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onItemMaster, this));

			cocos2d::ui::Button* friendbtn = (cocos2d::ui::Button*)npcitem->getChildByName("friendbtn");
			friendbtn->setTag(i);
			friendbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onItemFriend, this));

			int relation = F_NOMAR;
			if (JhGlobalData::map_myfriendly.find(mdata.npcs[i]) != JhGlobalData::map_myfriendly.end())
				relation = JhGlobalData::map_myfriendly[mdata.npcs[i]].relation;

			if (relation == F_MASTEROUT)
			{
				masterbtn->setTitleText(JhCommonFuncs::gbk2utf("出师"));
				masterbtn->setEnabled(false);
			}
			else if (relation == F_FRIEND)
			{
				friendbtn->setTitleText(JhCommonFuncs::gbk2utf("绝交"));
			}
			else if (relation == F_MASTER)
			{
				masterbtn->setTitleText(JhCommonFuncs::gbk2utf("出师"));
			}

			cocos2d::ui::Button* givebtn = (cocos2d::ui::Button*)npcitem->getChildByName("givebtn");
			givebtn->setTag(i);
			givebtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onItemGive, this));

			cocos2d::ui::Button* exchangebtn = (cocos2d::ui::Button*)npcitem->getChildByName("exchgbtn");
			exchangebtn->setTag(i);
			exchangebtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onItemExchange, this));
			if (JhGlobalData::map_npcs[mdata.npcs[i]].exchgres.size() <= 0)
				exchangebtn->setVisible(false);

			if (mdata.npcs[i].compare("n009") == 0)
			{
				talkbtn->setTitleText(JhCommonFuncs::gbk2utf("吃饭"));
				talkbtn->setTag(10 * i);
				talkbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onHostelAction, this));
				fightbtn->setTitleText(JhCommonFuncs::gbk2utf("睡觉"));
				fightbtn->setTag(10 * i + 1);
				fightbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onHostelAction, this));
				masterbtn->setVisible(true);
				masterbtn->setTitleText(JhCommonFuncs::gbk2utf("喝酒"));
				masterbtn->setTag(10 * i + 2);
				masterbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onHostelAction, this));
				friendbtn->setVisible(false);
				givebtn->setVisible(false);
				exchangebtn->setVisible(false);
			}
			else if (mdata.npcs[i].compare("n092") == 0)
			{
				fightbtn->setTitleText(JhCommonFuncs::gbk2utf("修理"));
				fightbtn->setTag(1);
				fightbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onRepair, this));
				masterbtn->setVisible(true);
				masterbtn->setTitleText(JhCommonFuncs::gbk2utf("强化"));
				masterbtn->setTag(2);
				masterbtn->addTouchEventListener(CC_CALLBACK_2(JhNpcLayer::onRepair, this));
				friendbtn->setVisible(false);
				givebtn->setVisible(false);
				exchangebtn->setVisible(false);
			}
		}
	}

	reFreshFriendlyUI();

	updatePlotUI(0);
	updatePlotUI(1);
}

void JhNpcLayer::checkUpateNpc(float dt)
{
	if (m_lastWxpPos != JhGlobalData::getWxbMapPos())
	{
		refreshNpcNode();
		m_lastWxpPos = JhGlobalData::getWxbMapPos();
	}
	if (m_lastDgqbPos != JhGlobalData::getDgqbMapPos())
	{
		refreshNpcNode();
		m_lastDgqbPos = JhGlobalData::getDgqbMapPos();
	}
}

void JhNpcLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhNpcLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhNpcLayer::onItemTalk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		NpcData npc = JhGlobalData::map_npcs[JhGlobalData::map_maps[m_addrstr].npcs[node->getTag()]];

		if (JhGlobalData::vec_PlotMissionData[JhGlobalData::getPlotMissionIndex()].snpc.compare(npc.id) == 0 && JhGlobalData::vec_PlotMissionData[JhGlobalData::getPlotMissionIndex()].status == M_NONE && JhGlobalData::vec_PlotMissionData[JhGlobalData::getPlotMissionIndex()].words.size() <= 0)
			return;

		vec_wordstr.clear();
		std::string wordstr;
		PlotMissionData pdata = JhGlobalData::vec_PlotMissionData[JhGlobalData::getPlotMissionIndex()];
		int plottype = 1;
		if ((pdata.snpc.compare(npc.id) == 0 && pdata.status == 0) || (pdata.dnpc.compare(npc.id) == 0 && pdata.status == 1))
			plottype = 0;

		bool ismissing = doCheckPlotMisson(plottype, npc);

		if (!ismissing)
		{
			//多个任务时，一起的任务对话还是算任务中的对话
			std::string curmid = JhGlobalData::getCurBranchPlotMissison();
			if (curmid.length() > 0 && JhGlobalData::map_BranchPlotMissionData[curmid].size() > 0)
			{
				int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;

				if (subindex > 0 && subindex < JhGlobalData::map_BranchPlotMissionData[curmid].size() - 1)
				{
					for (int i = 0; i <= subindex; i++)
					{
						PlotMissionData* pd = &JhGlobalData::map_BranchPlotMissionData[curmid][i];
						if (pd->snpc.compare(npc.id) == 0)
						{
							vec_wordstr.clear();
							for (unsigned int m = 0; m < pd->words.size(); m++)
							{
								wordstr = StringUtils::format("%s%s%s", npc.name, JhCommonFuncs::gbk2utf("：").c_str(), pd->words[m].c_str());
								if (g_hero->getHeadID() == 4)
									wordstr = replaceSexWord(wordstr);
								vec_wordstr.push_back(wordstr);

								wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), JhCommonFuncs::gbk2utf("：").c_str(), pd->mywords[m].c_str());
								vec_wordstr.push_back(wordstr);
							}
							ismissing = true;
						}
					}
				}
			}
			if (!ismissing)
			{
				wordstr = StringUtils::format("%s%s%s", npc.name, JhCommonFuncs::gbk2utf("：").c_str(), npc.words[0].c_str());
				if (g_hero->getHeadID() == 4)
					wordstr = replaceSexWord(wordstr);
				vec_wordstr.push_back(wordstr);
			}
		}

		if (vec_wordstr.size() > 0)
		{
			JhNpcTalkLayer* nlayer = JhNpcTalkLayer::create(vec_wordstr, npc.id);
			g_gameLayer->addChild(nlayer, 5);
		}
	}
}

void JhNpcLayer::onItemFight(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getParent()->getTag();
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[tag];
		std::string protectword;
		
		if (npcid.compare("n004") == 0 && checkFightCount("n004") < 0)
		{
			protectword = StringUtils::format("%s%s", JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("：就凭你？还不够我一刀的，我不杀无名小卒，再去练几年吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！").c_str());
			
		}
		else if (npcid.compare("n005") == 0 && checkFightCount("n005") < 0)
		{
			protectword = StringUtils::format("%s%s", JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("：少侠，你现在还太弱，还是在修炼修炼吧！先去打打野狼和兔子，升升等级再来吧！以后的人可没我这么好心！").c_str());
			if (g_hero->getHeadID() == 4)
				protectword = replaceSexWord(protectword);	
		}
		else
		{
			int count = checkFightCount(npcid);
			if (JhNewerGuide2Layer::checkifNewerGuide(101))
			{
				if (count < 0 && count >= -10)
				{
					std::vector<Node*> vec_node;
					int npcitemcount = m_scrollview->getChildrenCount();
					if (npcitemcount > 0)
					{
						std::string childname = StringUtils::format("npcnode%d", tag);
						Node* npcitemnode = m_scrollview->getChildByName(childname);

						cocos2d::ui::Widget* npcrsi = (cocos2d::ui::Widget*)npcitemnode->getChildByName("npcrsi");
						npcrsi->setUserData((void*)"npcrsi2");
						vec_node.push_back(npcrsi);
					}
					JhNewerGuide2Layer *layer = JhNewerGuide2Layer::create(101, vec_node);
					g_gameLayer->addChild(layer, NEWERLAYERZOER);
					return;
				}
			}
		}
		if (protectword.length() > 0)
		{
			vec_wordstr.clear();
			vec_wordstr.push_back(protectword);
			JhNpcTalkLayer* nlayer = JhNpcTalkLayer::create(vec_wordstr, npcid);
			g_gameLayer->addChild(nlayer, 5);

			return;
		}

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(JhFightLayer::create(m_addrstr, npcid), 4, "fightlayer");

		for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
		{
			if (JhGlobalData::vec_achiveData[i].type == A_12)
			{
				if (JhGlobalData::vec_achiveData[i].vec_para[0].compare(npcid) == 0)
				{
					if (JhGlobalData::vec_achiveData[i].finish != -1)
					{
						JhGlobalData::vec_achiveData[i].finish += 1;
						JhGlobalData::saveAchiveData();
					}
				}
			}
		}

		PlotMissionData pdata = JhGlobalData::vec_PlotMissionData[JhGlobalData::getPlotMissionIndex()];
		int plottype = 1;
		if (pdata.snpc.compare(npcid) == 0 || pdata.dnpc.compare(npcid) == 0)
			plottype = 0;

		if (!checkIsMissing(plottype, npcid))
			updateFriendly(npcid);
	}
}


void JhNpcLayer::onItemMaster(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)pSender;
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[btn->getTag()];
		if (btn->getTitleText().compare(JhCommonFuncs::gbk2utf("拜师")) == 0)
		{
			int friendly = 0;
			int relation = F_NOMAR;
			
			if (JhGlobalData::map_myfriendly.find(npcid) != JhGlobalData::map_myfriendly.end())
			{
				friendly = JhGlobalData::map_myfriendly[npcid].friendly;
				relation = JhGlobalData::map_myfriendly[npcid].relation;
			}
			if (relation == F_FRIEND)
			{
				g_uiScroll->addEventText(JhCommonFuncs::gbk2utf("你我关系这么好，我怎么能当你的师傅呢！"), 25, Color3B(204, 4, 4));
				return;
			}

			std::map<std::string, FriendlyData>::iterator mit;
			for (mit = JhGlobalData::map_myfriendly.begin(); mit != JhGlobalData::map_myfriendly.end(); ++mit)
			{
				if (JhGlobalData::map_myfriendly[mit->first].relation == F_MASTER)
				{
					std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("你已拜入").c_str(), JhGlobalData::map_npcs[mit->first].name, JhCommonFuncs::gbk2utf("门下！请回吧！").c_str());
					g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
					return;
				}
			}

			int needfriendly = JhGlobalData::map_NPCMasterData[npcid].needfriendly;
			if (friendly >= needfriendly)
			{
				bool ishasenemy = false;
				std::string e_npcnamstr;
				std::vector<std::string> e_npc = JhGlobalData::map_NPCFriendData[npcid].vec_enemynpc;
				std::map<std::string, FriendlyData>::iterator it;
				for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
				{
					std::string nid = it->first;

					for (unsigned int m = 0; m < e_npc.size(); m++)
					{
						if (nid.compare(e_npc[m]) == 0 && JhGlobalData::map_myfriendly[nid].relation == F_FRIEND)
						{
							JhGlobalData::map_myfriendly[nid].friendly -= JhGlobalData::map_NPCFriendData[nid].maxfriendly/5;
							JhGlobalData::map_myfriendly[nid].relation = F_NOMAR;
							e_npcnamstr.append(JhGlobalData::map_npcs[nid].name);
							e_npcnamstr.append(JhCommonFuncs::gbk2utf("、"));
							ishasenemy = true;
						}
					}
				}

				if (ishasenemy)
				{

					JhGlobalData::map_myfriendly[npcid].friendly = JhGlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
					JhGlobalData::map_myfriendly[npcid].relation = F_NOMAR;
					e_npcnamstr = e_npcnamstr.substr(0, e_npcnamstr.length() - 3);
					std::string desc = StringUtils::format("%s%s%s%s", JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("发现你与").c_str(), e_npcnamstr.c_str(), JhCommonFuncs::gbk2utf("的关系，他们发现你是这种三头两面的人，纷纷离你而去！").c_str());
					JhHintBox* hbox = JhHintBox::create(desc);
					this->addChild(hbox);
					JhGlobalData::saveFriendly();
					reFreshFriendlyUI();
					reFreshRelationUI();

					return;
				}

				JhGlobalData::map_myfriendly[npcid].relation = F_MASTER;
				JhGlobalData::saveFriendly();
				std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("拜入").c_str(), JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("门下！一日为师，终生为父！").c_str());
				g_uiScroll->addEventText(desc, 25, Color3B(27, 141, 0));
				btn->setTitleText(JhCommonFuncs::gbk2utf("出师"));
			}
			else
			{
				g_uiScroll->addEventText(JhCommonFuncs::gbk2utf("师徒缘分未到！"), 25, Color3B(204, 4, 4));
			}

		}
		else
		{
			JhGlobalData::isFightMaster = true;
			JhFightLayer* fightlayer = JhFightLayer::create(m_addrstr, npcid);
			this->addChild(fightlayer);
		}

	}
}

void JhNpcLayer::onItemFriend(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)pSender;
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[btn->getTag()];
		if (btn->getTitleText().compare(JhCommonFuncs::gbk2utf("结交")) == 0)
		{
			int friendly = 0;
			int relation = F_NOMAR;

			if (JhGlobalData::map_myfriendly.find(npcid) != JhGlobalData::map_myfriendly.end())
			{
				friendly = JhGlobalData::map_myfriendly[npcid].friendly;
				relation = JhGlobalData::map_myfriendly[npcid].relation;
			}

			if (relation == F_MASTER)
			{
				g_uiScroll->addEventText(JhCommonFuncs::gbk2utf("逆徒！还想跟为师称兄道弟？！"), 25, Color3B(204, 4, 4));
				return;
			}

			int needfriendly = JhGlobalData::map_NPCFriendData[npcid].needfriendly;
			if (friendly >= needfriendly)
			{
				bool ishasenemy = false;
				std::string e_npcnamstr;
				std::vector<std::string> e_npc = JhGlobalData::map_NPCFriendData[npcid].vec_enemynpc;
				std::map<std::string, FriendlyData>::iterator it;
				for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
				{
					std::string nid = it->first;
					
					for (unsigned int m = 0; m < e_npc.size(); m++)
					{
						if (nid.compare(e_npc[m]) == 0 && (JhGlobalData::map_myfriendly[nid].relation == F_FRIEND || JhGlobalData::map_myfriendly[nid].relation == F_MASTER))
						{
							JhGlobalData::map_myfriendly[nid].friendly -= JhGlobalData::map_NPCFriendData[nid].maxfriendly / 5;
							JhGlobalData::map_myfriendly[nid].relation = F_NOMAR;
							e_npcnamstr.append(JhGlobalData::map_npcs[nid].name);
							e_npcnamstr.append(JhCommonFuncs::gbk2utf("、"));
							ishasenemy = true;
						}
					}
				}

				if (ishasenemy)
				{

					JhGlobalData::map_myfriendly[npcid].friendly -= JhGlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
					JhGlobalData::map_myfriendly[npcid].relation = F_NOMAR;
					e_npcnamstr = e_npcnamstr.substr(0, e_npcnamstr.length() - 3);
					std::string desc = StringUtils::format("%s%s%s%s", JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("发现你与").c_str(), e_npcnamstr.c_str(), JhCommonFuncs::gbk2utf("的关系，他们发现你是这种三头两面的人，纷纷离你而去！").c_str());
					JhHintBox* hbox = JhHintBox::create(desc);
					this->addChild(hbox);
					JhGlobalData::saveFriendly();
					reFreshFriendlyUI();
					reFreshRelationUI();

					return;
				}

				JhGlobalData::map_myfriendly[npcid].relation = F_FRIEND;
				JhGlobalData::saveFriendly();
				std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("与").c_str(), JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("成为好友！").c_str());
				g_uiScroll->addEventText(desc, 25, Color3B(27, 141, 0));
				btn->setTitleText(JhCommonFuncs::gbk2utf("绝交"));
			}
			else
			{
				g_uiScroll->addEventText(JhCommonFuncs::gbk2utf("咱们两个不是很熟，就不要走的太近了！"), 25, Color3B(204, 4, 4));
			}

		}
		else
		{
			JhGlobalData::map_myfriendly[npcid].relation = F_NOMAR;
			JhGlobalData::saveFriendly();
			btn->setTitleText(JhCommonFuncs::gbk2utf("结交"));
			JhGlobalData::map_myfriendly[npcid].friendly -= JhGlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
			updateFriendly(npcid);
		}
	}
}

void JhNpcLayer::onItemGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[node->getTag()];
		Layer* layer = JhGiveLayer::create(npcid, m_addrstr);
		g_gameLayer->addChild(layer, 4);
	}
}


void JhNpcLayer::onItemExchange(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[node->getTag()];
		g_gameLayer->addChild(JhExchangeLayer::create(npcid), 4, "exchangelayer");
	}
}

void JhNpcLayer::onHostelAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int itemindex = node->getTag() / 10;
		int actionIndex = node->getTag()%10;
		int silver = 0;

		for (int i = 0; i < JhMyPackage::getSize(); i++)
		{
			if (JhMyPackage::vec_packages[i].strid.compare("80") == 0)
			{
				silver = JhMyPackage::vec_packages[i].count;
				break;
			}
		}

		if (silver < silvercost[actionIndex])
		{
			JhHintBox* hintBox = JhHintBox::create(JhCommonFuncs::gbk2utf("我靠。。。银子没带够！！"));
			addChild(hintBox);
		}
		else
		{
			JhCannotTouchLayer* layer = JhCannotTouchLayer::create();
			g_gameLayer->addChild(layer, 5, "notouchlayer");

			std::string childname = StringUtils::format("npcnode%d", itemindex);
			Node* npcitem = m_scrollview->getChildByName(childname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			talkbtn->setEnabled(false);

			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");
			onFight->setEnabled(false);

			cocos2d::ui::Button* onmsterbtn = (cocos2d::ui::Button*)npcitem->getChildByName("msterbtn");
			onmsterbtn->setEnabled(false);

			cocos2d::ui::Widget* pbg = (cocos2d::ui::Widget*)npcitem->getChildByName("progressbg");
			pbg->setVisible(true);
			cocos2d::ui::LoadingBar* pbar = (cocos2d::ui::LoadingBar*)npcitem->getChildByName("loadingbar");
			pbar->setVisible(true);

			cocos2d::ui::Text* ptext = (cocos2d::ui::Text*)npcitem->getChildByName("actiontext");
			ptext->setVisible(true);

			if (actionIndex == 0)
			{
				ptext->setString(JhCommonFuncs::gbk2utf("吃饭中..."));
			}
			else if (actionIndex == 1)
			{
				ptext->setString(JhCommonFuncs::gbk2utf("睡觉八小时"));
				int exminute = 480;
				g_nature->setTimeInterval(exminute * NORMAL_TIMEINTERVAL * 1.0f / (ACTION_BAR_TIME * TIMESCALE));
				g_hero->sleep(TIMESCALE * ACTION_BAR_TIME, exminute / 60);
			}

			else if (actionIndex == 2)
			{
				ptext->setString(JhCommonFuncs::gbk2utf("喝酒中..."));
			}
			pbar->runAction(Sequence::create(MyProgressTo::create(ACTION_BAR_TIME, 100), CallFuncN::create(CC_CALLBACK_1(JhNpcLayer::actionOver, this, npcitem, actionIndex)), NULL));
		}
	}
}

void JhNpcLayer::onRepair(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		JhRepairLayer* layer = JhRepairLayer::create(tag);
		g_gameLayer->addChild(layer, 4);
	}
}

void JhNpcLayer::actionOver(Ref* pSender, Node* item, int actionindex)
{
	JhMyPackage::cutone("80", silvercost[actionindex]);
	if (actionindex == 0)
	{
		float hungervale = g_hero->getHungerValue();
		int addvalue = 40;
		if (addvalue + hungervale > g_hero->getMaxHungerValue())
			g_hero->setHungerValue(g_hero->getMaxHungerValue());
		else
			g_hero->recoverHunger(addvalue);
	}
	else if (actionindex == 2)
	{
		float value = g_hero->getSpiritValue();
		float addvalue = 40;
		if (addvalue + value > g_hero->getMaxSpiritValue())
			g_hero->setSpiritValue(g_hero->getMaxSpiritValue());
		else
			g_hero->setSpiritValue(addvalue + value);
	}

	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);

	cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)item->getChildByName("talkbtn");
	talkbtn->setEnabled(true);

	cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)item->getChildByName("fightbtn");
	onFight->setEnabled(true);

	cocos2d::ui::Button* onmsterbtn = (cocos2d::ui::Button*)item->getChildByName("msterbtn");
	onmsterbtn->setEnabled(true);

	cocos2d::ui::Widget* pbg = (cocos2d::ui::Widget*)item->getChildByName("progressbg");
	pbg->setVisible(false);
	cocos2d::ui::LoadingBar* pbar = (cocos2d::ui::LoadingBar*)item->getChildByName("loadingbar");
	pbar->setPercent(0);
	pbar->setVisible(false);

	cocos2d::ui::Widget* ptext = (cocos2d::ui::Widget*)item->getChildByName("actiontext");
	ptext->setVisible(false);

	g_gameLayer->removeChildByName("notouchlayer");
}

bool JhNpcLayer::doCheckPlotMisson(int type, NpcData npcdata)
{
	bool isplotMissioning = false;
	std::string wordstr;
	int curplot = 0;
	PlotMissionData* plotData = NULL;

	if (type == 0)
	{
		curplot = JhGlobalData::getPlotMissionIndex();
		plotData = &JhGlobalData::vec_PlotMissionData[curplot];
	}
	else
	{
		std::string curmid = JhGlobalData::getCurBranchPlotMissison();
		if (curmid.length() > 0)
		{
			int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
			PlotMissionData pd = JhGlobalData::map_BranchPlotMissionData[curmid][subindex];
			if (pd.unlockchapter <= JhGlobalData::getUnlockChapter() && JhGlobalData::map_BranchPlotMissionItem[curmid].count > 0)
			{
				plotData = &JhGlobalData::map_BranchPlotMissionData[curmid][subindex];
			}
		}
		else
		{
			std::map<std::string, std::vector<PlotMissionData>>::iterator it;
			for (it = JhGlobalData::map_BranchPlotMissionData.begin(); it != JhGlobalData::map_BranchPlotMissionData.end(); it++)
			{
				if (JhGlobalData::map_BranchPlotMissionData[it->first].size() > 0)
				{
					PlotMissionData* pmdata = &JhGlobalData::map_BranchPlotMissionData[it->first][0];
					if (pmdata->snpc.compare(npcdata.id) == 0 && pmdata->unlockchapter <= JhGlobalData::getUnlockChapter() && JhGlobalData::map_BranchPlotMissionItem[pmdata->id].count > 0)
					{
						plotData = pmdata;
						break;
					}
				}
			}
			
		}
	}
	
	if (plotData != NULL)
	{
		if (plotData->snpc.compare(npcdata.id) == 0 && plotData->status != M_DONE)
		{
			isplotMissioning = true;
			plotData->status = M_DOING;
			if (type == 1)
				JhGlobalData::saveBranchPlotMissionStatus(plotData->id, plotData->status);
			updatePlotUI(type);
			for (unsigned int m = 0; m < plotData->words.size(); m++)
			{
				wordstr = StringUtils::format("%s%s%s", npcdata.name, JhCommonFuncs::gbk2utf("：").c_str(), plotData->words[m].c_str());
				if (g_hero->getHeadID() == 4)
					wordstr = replaceSexWord(wordstr);
				vec_wordstr.push_back(wordstr);

				wordstr = StringUtils::format("%s%s%s", g_hero->getMyName().c_str(), JhCommonFuncs::gbk2utf("：").c_str(), plotData->mywords[m].c_str());
				vec_wordstr.push_back(wordstr);
			}
			if (g_maplayer != NULL)
				g_maplayer->updataPlotMissionIcon(type);
		}

		if (plotData->status == M_DOING)
		{
			if (plotData->dnpc.compare(npcdata.id) == 0)
			{
				isplotMissioning = true;

				if (plotData->type == 0)
				{
					plotData->status = M_DONE;
					std::vector<std::string> vec_rwdres = plotData->rewords;
					getWinRes(vec_rwdres, m_addrstr);

					int unlockchapter = 0;
					if (type == 0)
					{
						unlockchapter = plotData->unlockchapter;
						JhGlobalData::setUnlockChapter(unlockchapter);

						JhGlobalData::setPlotMissionIndex(curplot + 1);
					}
					else
					{
						int subindex = JhGlobalData::map_BranchPlotMissionItem[plotData->id].subindex;
						JhGlobalData::map_BranchPlotMissionData[plotData->id][subindex].status = M_NONE;
						if (subindex + 1 >= JhGlobalData::map_BranchPlotMissionData[plotData->id].size())
						{
							JhGlobalData::map_BranchPlotMissionItem[plotData->id].subindex = 0;
							JhGlobalData::map_BranchPlotMissionItem[plotData->id].count--;
							JhGlobalData::map_BranchPlotMissionItem[plotData->id].time = JhGlobalData::map_BranchPlotMissionItem[plotData->id].maxtime;
							JhGlobalData::saveBranchPlotMissionStatus("", 0);
						}
						else
						{
							JhGlobalData::map_BranchPlotMissionItem[plotData->id].subindex++;
							JhGlobalData::saveBranchPlotMissionStatus(plotData->id, M_NONE);
						}
						
						Winlayer::showMissionAnim(g_gameLayer, "任务完成", vec_rwdres);
					}

					updatePlotUI(type);

					if (g_maplayer != NULL)
					{
						g_maplayer->updataPlotMissionIcon(type);
						if (unlockchapter > 0 && type == 0 && unlockchapter <= MAXCHAPTER)
						{
							g_maplayer->scheduleOnce(schedule_selector(JhMapLayer::showUnlockLayer), 1.0f);
						}

						if (plotData->dnpc.compare("n089") == 0)
						{
							g_maplayer->scheduleOnce(schedule_selector(JhMapLayer::showEndAnim), 1.5f);
						}
					}

				}
				//else
				//{
				//	JhGlobalData::saveBranchPlotMissionStatus(plotData->id, plotData->status);
				//}

				for (unsigned int m = 0; m < plotData->bossword.size(); m++)
				{
					wordstr = StringUtils::format("%s%s%s", npcdata.name, JhCommonFuncs::gbk2utf("：").c_str(), plotData->bossword[m].c_str());
					if (g_hero->getHeadID() == 4)
						wordstr = replaceSexWord(wordstr);
					vec_wordstr.push_back(wordstr);

				}
			}
		}
		if (isplotMissioning)
		{
			if (type == 0)
			{
				JhGlobalData::savePlotMissionStatus();
				for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
				{
					if (JhGlobalData::vec_achiveData[i].type == A_6)
					{
						if (plotData->id.compare(JhGlobalData::vec_achiveData[i].vec_para[0]) == 0 && JhGlobalData::vec_achiveData[i].finish != -1)
						{
							JhGlobalData::vec_achiveData[i].finish = 1;
							JhGlobalData::saveAchiveData();
						}
					}
				}
			}
			else if (plotData->type != 0)
			{
				JhGlobalData::saveBranchPlotMissionStatus(plotData->id, plotData->status);
			}
		}
	}

	return isplotMissioning;
}


int JhNpcLayer::checkIsMissing(int type, std::string npcid)
{
	bool isplotMissioning = false;
	int curplot = 0;
	PlotMissionData* plotData = NULL;
	if (type == 0)
	{
		curplot = JhGlobalData::getPlotMissionIndex();
		plotData = &JhGlobalData::vec_PlotMissionData[curplot];

		if (plotData->snpc.compare(npcid) == 0 && plotData->status != M_DONE)
		{
			isplotMissioning = true;
		}

		if (plotData->dnpc.compare(npcid) == 0 && plotData->status == M_DOING)
		{
			isplotMissioning = true;
		}
	}
	else
	{
		isplotMissioning = JhGlobalData::isDoingBranchPlotMisson();
	}

	return isplotMissioning;
}

void JhNpcLayer::updatePlotUI(int type)
{
	int plotIndex = 0;
	PlotMissionData* plotData = NULL;
	int ncpsize = JhGlobalData::map_maps[m_addrstr].npcs.size();
	if (type == 0)
	{
		plotIndex = JhGlobalData::getPlotMissionIndex();
		plotData = &JhGlobalData::vec_PlotMissionData[plotIndex];
	}
	else
	{
		std::string curmid = JhGlobalData::getCurBranchPlotMissison();
		if (curmid.length() > 0)
		{
			int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
			PlotMissionData pd = JhGlobalData::map_BranchPlotMissionData[curmid][subindex];
			if (pd.unlockchapter <= JhGlobalData::getUnlockChapter() && JhGlobalData::map_BranchPlotMissionItem[curmid].count > 0)
			{
				plotData = &JhGlobalData::map_BranchPlotMissionData[curmid][subindex];
			}
		}
		else
		{

			std::map<std::string, std::vector<PlotMissionData>>::iterator it;
			for (it = JhGlobalData::map_BranchPlotMissionData.begin(); it != JhGlobalData::map_BranchPlotMissionData.end(); it++)
			{
				if (JhGlobalData::map_BranchPlotMissionData[it->first].size() > 0)
				{
					PlotMissionData* pmdata = &JhGlobalData::map_BranchPlotMissionData[it->first][0];
					for (int i = 0; i < ncpsize; i++)
					{
						if (pmdata->snpc.compare(JhGlobalData::map_maps[m_addrstr].npcs[i]) == 0 && pmdata->unlockchapter <= JhGlobalData::getUnlockChapter() && JhGlobalData::map_BranchPlotMissionItem[pmdata->id].count > 0)
						{
							//PlotMissionData pd = *sit;
							plotData = pmdata;
							break;
						}
					}
				}
			}
		}
	}
	if (plotData != NULL)
	{
		std::string snpc = plotData->snpc;
		std::string dnpc = plotData->dnpc;
		for (int i = 0; i < ncpsize; i++)
		{
			std::string childname = StringUtils::format("npcnode%d", i);
			Node* npcitem = m_scrollview->getChildByName(childname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");

			std::string smissionname = StringUtils::format("m%d_0", type);
			if (talkbtn->getChildByName(smissionname) != NULL)
				talkbtn->removeChildByName(smissionname);

			if (snpc.compare(JhGlobalData::map_maps[m_addrstr].npcs[i]) == 0)
			{
				if (plotData->status == M_NONE)
				{
					if (plotData->words.size() <= 0)
					{
						plotData->status = M_DOING;
					}
					else
					{
						std::string miconstr = StringUtils::format("ui/mapmission%d_0.png", type);
						Sprite* micon = Sprite::createWithSpriteFrameName(miconstr);
						micon->setScale(0.6f);
						micon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
						talkbtn->addChild(micon, 0, smissionname);
					}
					//支线任务不现实提示标签
					if (type == 1)
					{
						if (onFight->getChildByName(smissionname) != NULL)
							onFight->removeChildByName(smissionname);

						if (talkbtn->getChildByName(smissionname) != NULL)
							talkbtn->removeChildByName(smissionname);
					}
				}
			}
		}

		for (int i = 0; i < ncpsize; i++)
		{
			std::string childname = StringUtils::format("npcnode%d", i);
			Node* npcitem = m_scrollview->getChildByName(childname);
			cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
			cocos2d::ui::Button* onFight = (cocos2d::ui::Button*)npcitem->getChildByName("fightbtn");

			std::string dmissionname = StringUtils::format("m%d_1", type);

			if (onFight->getChildByName(dmissionname) != NULL)
				onFight->removeChildByName(dmissionname);

			if (talkbtn->getChildByName(dmissionname) != NULL)
				talkbtn->removeChildByName(dmissionname);

			bool ret = false;
			if (plotData->mapid.length() > 0)
			{
				if (plotData->mapid.compare(m_addrstr) == 0)
					ret = true;
			}
			else
			{
				ret = true;
			}

			if (dnpc.compare(JhGlobalData::map_maps[m_addrstr].npcs[i]) == 0 && ret)
			{

				if (plotData->status == M_DOING)
				{
					std::string diconstr = StringUtils::format("ui/mapmission%d_1.png", type);
					Sprite* dicon = Sprite::createWithSpriteFrameName(diconstr);
					dicon->setScale(0.6f);
					if (plotData->type == 1)
					{
						dicon->setPosition(Vec2(onFight->getContentSize().width - 10, onFight->getContentSize().height - 10));
						onFight->addChild(dicon, 0, dmissionname);
					}
					else
					{
						dicon->setPosition(Vec2(talkbtn->getContentSize().width - 10, talkbtn->getContentSize().height - 10));
						talkbtn->addChild(dicon, 0, dmissionname);
					}
				}
			}
			//支线任务不现实提示标签
			if (type == 1)
			{
				if (onFight->getChildByName(dmissionname) != NULL)
					onFight->removeChildByName(dmissionname);

				if (talkbtn->getChildByName(dmissionname) != NULL)
					talkbtn->removeChildByName(dmissionname);
			}
		}
	}
}

void JhNpcLayer::getWinRes(std::vector<std::string> vec_res, std::string addrid)
{
	std::vector<PackageData> tempResData;//背包满了，保存到临时存放点
	std::vector<std::string> winres = vec_res;
	int addret = 0;
	PackageData data;
	for (unsigned int i = 0; i < winres.size(); i++)
	{
		int res = atoi(winres[i].c_str());

		if (res != 0)
		{
			bool isfind = false;
			std::string strid = StringUtils::format("%d", res / 1000);
			data.strid = strid;
			data.count = res % 1000;

			for (unsigned int i = 0; i < JhGlobalData::vec_resData.size(); i++)
			{
				ResData rdata = JhGlobalData::vec_resData[i];
				if (atoi(rdata.strid.c_str()) == res / 1000)
				{
					isfind = true;
					data.type = rdata.type - 1;
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, std::vector<BuildActionData>>::iterator it;
				for (it = JhGlobalData::map_buidACData.begin(); it != JhGlobalData::map_buidACData.end(); ++it)
				{
					std::vector<BuildActionData> vec_bactData = JhGlobalData::map_buidACData[it->first];

					for (unsigned int m = 0; m < vec_bactData.size(); m++)
					{
						BuildActionData bdata = vec_bactData[m];
						if (atoi(bdata.icon) == res / 1000)
						{
							isfind = true;
							data.strid = bdata.icon;
							data.count = res % 1000;
							data.type = bdata.type - 1;
							data.extype = bdata.extype;
							break;
						}
					}
					if (isfind)
						break;
				}
			}
			int count = data.count;
			for (int i = 0; i < count; i++)
			{
				PackageData onedata = data;
				onedata.count = 1;
				addret = JhMyPackage::add(onedata);
				if (addret < 0)
					break;
				else
				{
					data.count--;
				}
			}
		}
		else
		{
			std::string strid = winres[i];
			data.strid = strid;
			data.count = 1;

			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = JhGlobalData::map_wgngs.begin(); it != JhGlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = JhGlobalData::map_wgngs[it->first];
				if (winres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF_Equip(winres[i]) && JhGlobalData::tempHasGf_Equip(winres[i]).length() <= 0)
				{
					isfind = true;
					data.strid = gfdata.id;
					data.count = 1;
					data.type = gfdata.type - 1;
					data.extype = gfdata.extype;
					addret = JhMyPackage::add(data);
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = JhGlobalData::map_equips[ite->first];
					if (winres[i].compare(edata.id) == 0 && !g_hero->checkifHasGF_Equip(winres[i]) && JhGlobalData::tempHasGf_Equip(winres[i]).length() <= 0)
					{
						data.strid = edata.id;
						data.count = 1;
						data.type = edata.type - 1;
						data.extype = edata.extype;
						addret = JhMyPackage::add(data);
						break;
					}
				}
			}
		}
		if (addret < 0 && data.count > 0)
		{
			tempResData.push_back(data);
		}
	}
	if (tempResData.size() > 0)
	{
		std::string datastr = JhGameDataSave::getInstance()->getTempStorage(addrid);

		for (unsigned int i = 0; i < tempResData.size(); i++)
		{
			std::string sepstr;
			if (datastr.size() <= 0)
				sepstr = "";
			else
				sepstr = ";";
			std::string onestr = StringUtils::format("%s%s-%d-%d-%d-%d-%d-%d-%d-%d", sepstr.c_str(), tempResData[i].strid.c_str(), tempResData[i].type, tempResData[i].count, tempResData[i].extype, tempResData[i].lv, tempResData[i].exp, tempResData[i].goodvalue, tempResData[i].slv, tempResData[i].tqu);
			datastr.append(onestr);

			int tmptype = tempResData[i].type;
			std::string tmpstrid = tempResData[i].strid;
			if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
			{
				JhGlobalData::map_tempGf_Equip[addrid].clear();
				JhGlobalData::map_tempGf_Equip[addrid].push_back(tmpstrid);
			}
		}
		JhGameDataSave::getInstance()->setTempStorage(addrid, datastr);
	}

}

void JhNpcLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	std::string childname = StringUtils::format("npcnode%d", 0);
	Node* npcitem = m_scrollview->getChildByName(childname);
	cocos2d::ui::Button* talkbtn = (cocos2d::ui::Button*)npcitem->getChildByName("talkbtn");
	nodes.push_back(talkbtn);
	g_gameLayer->showNewerGuide(step, nodes);
}

void JhNpcLayer::delayShowNewerGuide(float dt)
{
	if (JhNewerGuideLayer::checkifNewerGuide(49) && m_addrstr.compare("m1-4") == 0)
		showNewerGuide(49);
	else if (JhNewerGuideLayer::checkifNewerGuide(50) && m_addrstr.compare("m1-3") == 0)
		showNewerGuide(50);
}

int JhNpcLayer::checkFightCount(std::string npcid)
{

	int npcdf = JhGlobalData::map_npcs[npcid].df;
	int npcatk = JhGlobalData::map_npcs[npcid].atk;

	int heroCurAck = g_hero->getTotalAtck();

	int npchurt = heroCurAck - npcdf;
	float fminack = 0.1f * heroCurAck;
	int intminack = int(fminack + 0.5f);
	if (npchurt < intminack)
		npchurt = intminack;

	if (npchurt <= 0)
		npchurt = 1;

	int npclife = JhGlobalData::map_npcs[npcid].life;
	
	int heroVsBossCount = npclife % npchurt == 0 ? npclife / npchurt : (npclife / npchurt + 1);

	int curheroDf = g_hero->getTotalDf();

	int herohurt = npcatk - curheroDf;

	fminack = 0.1f * npcatk;
	intminack = int(fminack + 0.5f);

	if (herohurt < intminack)
		herohurt = intminack;

	if (herohurt <= 0)
		herohurt = 1;

	int intlife = int(g_hero->getLifeValue() + 1);
	int bossVsHeroCount = intlife % herohurt == 0 ? intlife / herohurt : (intlife / herohurt + 1);

	if (heroVsBossCount <= bossVsHeroCount)
		return heroVsBossCount;
	else
		return -bossVsHeroCount;
}

std::string JhNpcLayer::replaceSexWord(std::string dstr)
{
	std::string retstr = dstr;
	int size = sizeof(replacestr) / sizeof(replacestr[0]);
	for (int i = 0; i < size; i++)
	{
		retstr = JhCommonFuncs::replace_all(dstr, JhCommonFuncs::gbk2utf(replacestr[i].c_str()), JhCommonFuncs::gbk2utf(areplacestr[i].c_str()));
	}
	return retstr;
}

void JhNpcLayer::reFreshFriendlyUI()
{
	int npcitemcount = m_scrollview->getChildrenCount();
	for (int i = 0; i < npcitemcount; i++)
	{
		std::string childname = StringUtils::format("npcnode%d", i);
		Node* npcitemnode = m_scrollview->getChildByName(childname);
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[i];

		int friendly = 0;
		if (JhGlobalData::map_myfriendly.find(npcid) != JhGlobalData::map_myfriendly.end())
		{
			friendly = JhGlobalData::map_myfriendly[npcid].friendly;
		}

		int maxfriendly = JhGlobalData::map_NPCFriendData[npcid].maxfriendly;
		if (friendly > maxfriendly)
			friendly = maxfriendly;
		else if (friendly < -maxfriendly)
			friendly = -maxfriendly;

		int per = maxfriendly / 5;
		int count = abs(friendly/per);

		for (int m = 0; m < 5; m++)
		{
			std::string barstr = StringUtils::format("fbar_%d", m);
			cocos2d::ui::LoadingBar* friendbar = (cocos2d::ui::LoadingBar*)npcitemnode->getChildByName(barstr);
			friendbar->setPercent(0);
			if (friendly < 0)
			{
				friendbar->loadTexture("ui/fheart0.png", cocos2d::ui::TextureResType::PLIST);
			}
			else
			{
				friendbar->loadTexture("ui/fheart1.png", cocos2d::ui::TextureResType::PLIST);
			}
			if (m < count)
			{
				friendbar->setPercent(100);
			}
			else if (m == count)
			{
				float percent = fabs(friendly % per * 100.0f / per);
				friendbar->setPercent(percent);
			}
		}

	}
}

void JhNpcLayer::reFreshRelationUI()
{
	int npcitemcount = m_scrollview->getChildrenCount();
	for (int i = 0; i < npcitemcount; i++)
	{
		std::string childname = StringUtils::format("npcnode%d", i);
		Node* npcitemnode = m_scrollview->getChildByName(childname);
		std::string npcid = JhGlobalData::map_maps[m_addrstr].npcs[i];

		cocos2d::ui::Button* masterbtn = (cocos2d::ui::Button*)npcitemnode->getChildByName("msterbtn");
		cocos2d::ui::Button* friendbtn = (cocos2d::ui::Button*)npcitemnode->getChildByName("friendbtn");

		int relation = F_NOMAR;

		if (JhGlobalData::map_myfriendly.find(npcid) != JhGlobalData::map_myfriendly.end())
		{
			relation = JhGlobalData::map_myfriendly[npcid].relation;
		}

		if (relation == F_MASTEROUT)
		{
			masterbtn->setEnabled(false);
			masterbtn->setTitleText(JhCommonFuncs::gbk2utf("出师"));
		}
		else if (relation == F_NOMAR)
		{
			friendbtn->setTitleText(JhCommonFuncs::gbk2utf("结交"));
			masterbtn->setTitleText(JhCommonFuncs::gbk2utf("拜师"));
		}
		else if (relation == F_FRIEND)
		{
			friendbtn->setTitleText(JhCommonFuncs::gbk2utf("绝交"));
		}
		else if (relation == F_MASTER)
		{
			masterbtn->setTitleText(JhCommonFuncs::gbk2utf("出师"));
		}

	}
}

void JhNpcLayer::updateFriendly(std::string npcid)
{
	JhGlobalData::map_myfriendly[npcid].friendly -= 5;
	int friendly = JhGlobalData::map_myfriendly[npcid].friendly;
	int needfriendly = JhGlobalData::map_NPCMasterData[npcid].needfriendly;
	int masterfriendly = JhGlobalData::map_NPCMasterData[npcid].needfriendly;
	if (friendly < needfriendly)
	{
		if (JhGlobalData::map_myfriendly[npcid].relation == F_FRIEND)
		{
			JhGlobalData::map_myfriendly[npcid].relation = F_NOMAR;
			std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("与").c_str(), JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("关系恶化，不再是朋友！").c_str());
			g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
			JhGlobalData::map_myfriendly[npcid].friendly -= JhGlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
		}
	}
	if (friendly < masterfriendly)
	{
		if (JhGlobalData::map_myfriendly[npcid].relation == F_MASTER)
		{
			JhGlobalData::map_myfriendly[npcid].relation = F_NOMAR;
			std::string desc = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("不懂尊师重道，被").c_str(), JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("逐出师门！").c_str());
			g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
			JhGlobalData::map_myfriendly[npcid].friendly -= JhGlobalData::map_NPCFriendData[npcid].maxfriendly / 5;
		}
	}
	JhGlobalData::saveFriendly();
	reFreshFriendlyUI();
	reFreshRelationUI();
}

void JhNpcLayer::showTalkGuider()
{
	if (JhNewerGuide2Layer::checkifNewerGuide(100) && JhGlobalData::getUnlockChapter() == 2)
	{
		std::vector<Node*> vec_node;
		int npcitemcount = m_scrollview->getChildrenCount();
		if (npcitemcount > 0)
		{
			std::string childname = StringUtils::format("npcnode%d", 0);
			Node* npcitemnode = m_scrollview->getChildByName(childname);

			cocos2d::ui::Widget* fbtn = (cocos2d::ui::Widget*)npcitemnode->getChildByName("friendbtn");
			fbtn->setUserData((void*)"npcbtn1");
			vec_node.push_back(fbtn);

			cocos2d::ui::Widget* gbtn = (cocos2d::ui::Widget*)npcitemnode->getChildByName("givebtn");
			gbtn->setUserData((void*)"npcbtn1");
			vec_node.push_back(gbtn);
			
			for (int i = 0; i < 5; i++)
			{
				std::string str = StringUtils::format("fheartbg_%d", i);
				cocos2d::ui::Widget* npcheartbg = (cocos2d::ui::Widget*)npcitemnode->getChildByName(str);
				npcheartbg->setUserData((void*)"fheartbg");
				vec_node.push_back(npcheartbg);

				//str = StringUtils::format("fbar_%d", i);
				//cocos2d::ui::Widget* npcheart = (cocos2d::ui::Widget*)npcitemnode->getChildByName(str);
				//npcheart->setUserData("fheart1");
				//vec_node.push_back(npcheart);
			}
		}
		JhNewerGuide2Layer *layer = JhNewerGuide2Layer::create(100, vec_node);
		g_gameLayer->addChild(layer, NEWERLAYERZOER);
	}
}
