#include "JhGiveLayer.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhMyPackage.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhStorageRoom.h"
#include "JhSoundManager.h"
#include "JhNpcLayer.h"
#include "JhMyMenu.h"
#include "JhGameDataSave.h"
#include "JhWinlayer.h"

JhGiveLayer::JhGiveLayer()
{
	lastMyGoodsSrollViewHeight = -1;
	lastGiveGoodsSrollViewHeight = -1;
}


JhGiveLayer::~JhGiveLayer()
{

}


JhGiveLayer* JhGiveLayer::create(std::string npcid, std::string addrid)
{
	JhGiveLayer *pRet = new JhGiveLayer();
	if (pRet && pRet->init(npcid, addrid))
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

bool JhGiveLayer::init(std::string npcid, std::string addrid)
{

	Node* csbnode = CSLoader::createNode("jhgiveLayer.csb");
	this->addChild(csbnode);

	m_npcid = npcid;
	m_addrid = addrid;

	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(JhGiveLayer::onBack, this));

	m_givebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("givebtn");
	m_givebtn->addTouchEventListener(CC_CALLBACK_2(JhGiveLayer::onGive, this));

	cocos2d::ui::ImageView* npchead = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");
	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);
	npchead->setScale(0.6f);

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcname->setString(JhGlobalData::map_npcs[npcid].name);

	m_giveGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("givescroll");
	m_giveGoodsSrollView->setScrollBarEnabled(false);
	m_myGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("mygoodsscroll");
	m_myGoodsSrollView->setScrollBarEnabled(false);

	giveval = 0;
	friendly = JhGlobalData::map_myfriendly[m_npcid].friendly;
	friendlylbl = (cocos2d::ui::Text*)csbnode->getChildByName("friendly");

	if (friendly < -100000 || friendly > 100000)
		friendly = 0;

	std::string friendstr = StringUtils::format("%d", friendly);
	friendlylbl->setString(friendstr);


	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		myGoodsData.push_back(JhMyPackage::vec_packages[i]);
	}
	updataMyGoodsUI();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhGiveLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhGiveLayer::onGiveGoodsItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);

	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();

	int datatag = node->getTag();

	PackageData udata = *data;
	udata.count = 1;

	if (--data->count <= 0)
	{
		myGiveData.erase(myGiveData.begin() + datatag);
	}
	updateMyGoods(udata);

	updata();
}

void JhGiveLayer::updateMyGoods(PackageData data)
{

	unsigned int i = 0;
	for (i = 0; i < myGoodsData.size(); i++)
	{
		if (data.strid.compare(myGoodsData[i].strid) == 0 && (myGoodsData[i].type == FOOD || myGoodsData[i].type == MEDICINAL || myGoodsData[i].type == RES_1 || myGoodsData[i].type == RES_2))
		{
			if (myGoodsData[i].count < 10)
			{
				myGoodsData[i].count++;
				break;
			}
			else
				continue;
		}
	}
	if (i == myGoodsData.size())
	{
		data.count = 1;
		myGoodsData.push_back(data);
	}

}

void JhGiveLayer::updateGiveGoods(PackageData data)
{

	unsigned int i = 0;
	for (i = 0; i < myGiveData.size(); i++)
	{
		if (data.strid.compare(myGiveData[i].strid) == 0 && (myGiveData[i].type == FOOD || myGiveData[i].type == MEDICINAL || myGiveData[i].type == RES_1 || myGiveData[i].type == RES_2))
		{
			myGiveData[i].count++;
			break;
		}
	}
	if (i == myGiveData.size())
	{
		data.count = 1;
		myGiveData.push_back(data);
	}

}

void JhGiveLayer::onMyGoodsItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);

	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int datatag = node->getTag();

	PackageData udata = *data;
	udata.count = 1;

	if (--data->count <= 0)
	{
		myGoodsData.erase(myGoodsData.begin() + datatag);
	}
	updateGiveGoods(udata);
	updata();
}

void JhGiveLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhGiveLayer::removeSelf(float dt)
{
	this->removeFromParentAndCleanup(true);
}

void JhGiveLayer::onGive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_backbtn->setEnabled(false);
		m_givebtn->setEnabled(false);
		JhMyPackage::vec_packages.clear();

		for (unsigned int i = 0; i < myGoodsData.size(); i++)
		{
			JhMyPackage::vec_packages.push_back(myGoodsData[i]);
		}
		JhGlobalData::map_myfriendly[m_npcid].friendly = friendly + giveval;
		JhGlobalData::saveFriendly();
		JhMyPackage::save();
		JhNpcLayer* npclayer = (JhNpcLayer*)g_gameLayer->getChildByName("npclayer");
		npclayer->reFreshFriendlyUI();
		doGiveMission();
	}
}
void JhGiveLayer::checkValue()
{
	giveval = 0;

	for (unsigned int i = 0; i < myGiveData.size(); i++)
	{
		std::string resid = myGiveData[i].strid;
		giveval += JhGlobalData::map_allResource[resid].fval * myGiveData[i].count;
	}
	std::string friendstr = StringUtils::format("%d", friendly + giveval);
	friendlylbl->setString(friendstr);
}

void JhGiveLayer::updata()
{
	updataMyGoodsUI();
	updataGiveGoodsUI();
	checkValue();
}

void JhGiveLayer::updataMyGoodsUI()
{
	int size = myGoodsData.size();

	m_myGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = row * 140;

	if (lastMyGoodsSrollViewHeight != innerheight)
	{
		lastMyGoodsSrollViewHeight = innerheight;
		int contentheight = m_myGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		m_myGoodsSrollView->setInnerContainerSize(Size(m_myGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allMydata;
	for (unsigned int i = 0; i < myGoodsData.size(); i++)
	{
		allMydata.push_back(&myGoodsData[i]);
	}

	int allsize = allMydata.size();
	for (int i = 0; i < allsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData* tmpdata = allMydata[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhGiveLayer::onMyGoodsItem, this));
		boxItem->setUserData(allMydata[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		JhMyMenu* menu = JhMyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_myGoodsSrollView);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_myGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", tmpdata->count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void JhGiveLayer::updataGiveGoodsUI()
{
	int size = myGiveData.size();

	m_giveGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = m_giveGoodsSrollView->getInnerContainerSize().height;
	if (lastGiveGoodsSrollViewHeight < 0)
	{
		innerheight = row * 140;
		int contentheight = m_giveGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastGiveGoodsSrollViewHeight = innerheight;
		m_giveGoodsSrollView->setInnerContainerSize(Size(m_giveGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allNpcdata;
	for (unsigned int i = 0; i < myGiveData.size(); i++)
	{
		allNpcdata.push_back(&myGiveData[i]);
	}

	int allsize = allNpcdata.size();
	for (int i = 0; i < allsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData *tmpdata = allNpcdata[i];
		if (tmpdata->type == WEAPON || tmpdata->type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata->strid].qu);
		}
		else if (tmpdata->type == N_GONG || tmpdata->type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata->strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhGiveLayer::onGiveGoodsItem, this));
		boxItem->setUserData(allNpcdata[i]);
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));
		JhMyMenu* menu = JhMyMenu::create();
		menu->addChild(boxItem);
		menu->setTouchlimit(m_giveGoodsSrollView);

		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		m_giveGoodsSrollView->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", tmpdata->count);
		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 25));
		box->addChild(reslbl);
	}
}

void JhGiveLayer::doGiveMission()
{
	bool isAnim = false;
	std::string curmid = JhGlobalData::getCurBranchPlotMissison();
	std::map<std::string, int> map_needGoods;
	std::map<std::string, int>::iterator it;
	
	if (curmid.length() > 0)
	{
		int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
		PlotMissionData pd = JhGlobalData::map_BranchPlotMissionData[curmid][subindex];
		std::vector<std::string> vec_rwdres = pd.rewords;
		std::string savedgstr = JhGameDataSave::getInstance()->getBranchPlotMissionGiveGoods();
		std::vector<std::string> needgoods;
		if (savedgstr.length() > 0)
		{
			std::vector<std::string> vec_retstr;
			JhCommonFuncs::split(savedgstr, needgoods, ";");
		}
		else
		{
			needgoods = pd.needgoods;
		}

		if (pd.status == M_DOING && pd.type == 2 && JhGlobalData::map_BranchPlotMissionItem[curmid].count > 0)
		{
			for (unsigned int i = 0; i < needgoods.size(); i++)
			{
				std::string resid = needgoods[i];
				int intresid = atoi(resid.c_str());
				int count = 1;
				if (intresid > 0)
				{
					resid = StringUtils::format("%d", intresid / 1000);
					count = intresid % 1000;
				}
				map_needGoods[resid] = count;
			}
			for (unsigned int i = 0; i < myGiveData.size(); i++)
			{
				for (it = map_needGoods.begin(); it != map_needGoods.end(); it++)
				{
					if (myGiveData[i].strid.compare(it->first) == 0)
					{
						if (myGiveData[i].count >= map_needGoods[it->first])
						{
							map_needGoods.erase(it);
							break;
						}
						else
						{
							map_needGoods[it->first] -= myGiveData[i].count;
						}
					}
				}
			}
			if (needgoods.size() > 0)
			{
				if (map_needGoods.size() <= 0)
				{
					//完成
					int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
					JhGlobalData::map_BranchPlotMissionData[curmid][subindex].status = M_NONE;

					giveRes(JhGlobalData::map_BranchPlotMissionData[curmid][subindex].rewords);

					if (subindex + 1 >= JhGlobalData::map_BranchPlotMissionData[curmid].size())
					{
						JhGlobalData::map_BranchPlotMissionItem[curmid].subindex = 0;
						JhGlobalData::map_BranchPlotMissionItem[curmid].count--;
						JhGlobalData::map_BranchPlotMissionItem[curmid].time = JhGlobalData::map_BranchPlotMissionItem[curmid].maxtime;
						JhGlobalData::saveBranchPlotMissionStatus("", 0);
					}
					else
					{
						JhGlobalData::map_BranchPlotMissionItem[curmid].subindex++;
						JhGlobalData::saveBranchPlotMissionStatus(curmid, M_NONE);
					}

					JhGameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
					JhWinlayer::showMissionAnim(this, "任务完成", vec_rwdres);
					isAnim = true;
				}
				else
				{
					std::string str;
					//未完成
					for (it = map_needGoods.begin(); it != map_needGoods.end(); it++)
					{
						std::string tempstr;
						int intresid = atoi(it->first.c_str());
						if (intresid > 0)
							tempstr = StringUtils::format("%d;", intresid * 1000 + map_needGoods[it->first]);
						else
							tempstr = StringUtils::format("%s;", it->first.c_str());

						str.append(tempstr);
					}
					if (str.length() > 0)
					{
						JhGameDataSave::getInstance()->setBranchPlotMissionGiveGoods(str.substr(0, str.length() - 1));
					}
				}
			}
		}
	}

	if (isAnim)
	{
		this->scheduleOnce(schedule_selector(JhGiveLayer::removeSelf), 2.5f);
	}
	else
	{
		removeSelf(0);
	}
}

void JhGiveLayer::giveRes(std::vector<std::string> vec_res)
{
	JhNpcLayer::getWinRes(vec_res, m_addrid);
}
