#include "JhHeroProperNode.h"
#include "json.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhGlobalData.h"
#include "StorageRoom.h"
#include "JhGameDataSave.h"
#include "SoundManager.h"
#include "JhActionGetLayer.h"
#include "TempStorageLayer.h"
#include "Winlayer.h"
#include "JhOutDoor.h"
#include "JhHeroStateUILayer.h"
#include "JhMyMenu.h"
#include "JhBuyComfirmLayer.h"
#include "JhNewerGuideLayer.h"
#include "SpecialHintLayer.h"
#include "JhHintBox.h"
#include "JhMixGFNode.h"

//装备栏类型显示文字
const std::string name[] = { "武功", "内功", "武器", "防具", "工具", "工具", "工具", "坐骑"};

//装备栏的类型
const HeroAtrType Atrytpe[] = { H_WG, H_NG, H_WEAPON, H_ARMOR, H_GATHER, H_FELL, H_EXCAVATE, H_MOUNT };

JhHeroProperNode::JhHeroProperNode()
{
	lastclickindex = -1;
	m_lastSelectedData = NULL;
	m_step = 3;
}


JhHeroProperNode::~JhHeroProperNode()
{

}

bool JhHeroProperNode::init(int index)
{
	m_index = index;
	if (index == 1)
	{
		csbroot = CSLoader::createNode("jhheroAttribNode.csb");
	}
	else
	{
		csbroot = CSLoader::createNode("jhheroAttribNode2.csb");
	}
	this->addChild(csbroot);
	for (int i = 0; i < 8; i++)
	{
		std::string str = StringUtils::format("Image_%d", i);
		propeImages[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);

		str = StringUtils::format("lvtext_%d", i);
		lvtext[i] = (cocos2d::ui::Text*)csbroot->getChildByName(str);

		str = StringUtils::format("zbzbz_%d", i);
		zbzbz[i] = (cocos2d::ui::Widget*)csbroot->getChildByName(str);

		str = StringUtils::format("box_%d", i);
		imgbtn[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);
		imgbtn[i]->setTag(i);
		imgbtn[i]->addTouchEventListener(CC_CALLBACK_2(JhHeroProperNode::onImageClick, this));
		addCarryData(Atrytpe[i]);

		//获取每个装备栏装备的数据，如果没有hpdata->count = 0
		PackageData* hpdata = g_hero->getAtrByType(Atrytpe[i]);
		if (hpdata->count > 0)
		{
			PackageData carrydata = *hpdata;
			updataProperpanel(i, *hpdata);
		}
	}

	heroselectbg = (cocos2d::ui::Widget*)csbroot->getChildByName("heroselectbg");
	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)heroselectbg->getChildByName("okbtn");
	if (okbtn!=NULL)
	{
		okbtn->addTouchEventListener(CC_CALLBACK_2(JhHeroProperNode::onOkClick, this));
	}
	//heroppoint = (cocos2d::ui::Widget*)csbroot->getChildByName("heroppoint");

	m_scrollView = (cocos2d::ui::ScrollView*)heroselectbg->getChildByName("ScrollView");
	m_scrollView->setScrollBarEnabled(false);
	m_scrollView->setBounceEnabled(true);

	/*cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)heroselectbg->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhHeroProperNode::onOK, this));*/

	title = (cocos2d::ui::Text*)heroselectbg->getChildByName("title");

	amountdesc = (cocos2d::ui::Widget*)heroselectbg->getChildByName("amountdesc");

	isout = g_hero->getIsOut();

	m_select = Sprite::createWithSpriteFrameName("ui/atrselected.png");
	m_select->setAnchorPoint(Vec2(0, 1));
	m_scrollView->addChild(m_select, 1);
	m_select->setVisible(false);

	m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return false;
	};
	m_listener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	return true;
}

void JhHeroProperNode::onEnterTransitionDidFinish()
{
	Node::onEnterTransitionDidFinish();
	showNewerGuide(m_step);
}

void JhHeroProperNode::onOkClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heroselectbg->setVisible(false);
	}
}

void JhHeroProperNode::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		heroselectbg->setVisible(false);
		//heroppoint->setVisible(false);
		m_listener->setSwallowTouches(false);
		JhHeroStateUILayer* heroStateUILayer = (JhHeroStateUILayer*)this->getParent()->getParent();
		JhMixGFNode* mixnode = (JhMixGFNode*)this->getParent()->getChildByName("mixnode");
		if (heroStateUILayer != NULL)
		{
			if (JhNewerGuideLayer::checkifNewerGuide(12))
				heroStateUILayer->showNewerGuide(12);
			else
			{
				if (mixnode != NULL && m_lastSelectedData != NULL && ((m_lastSelectedData->type == N_GONG && g_hero->getAtrByType(H_NG)->count >0) || (m_lastSelectedData->type == W_GONG && g_hero->getAtrByType(H_WG)->count >0)))
					mixnode->showTalkGuide();
			}
		}
		this->setLocalZOrder(0);
	}
}

void JhHeroProperNode::onImageClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

		Node* node = (Node*)pSender;
		int tag = node->getTag();
		//点击相同的一个不做操作
		if (lastclickindex == tag && heroselectbg->isVisible())
			return;
		m_listener->setSwallowTouches(true);
		if (g_NewerGuideLayer != NULL)
		{
			g_NewerGuideLayer->removeFromParentAndCleanup(true);
			g_NewerGuideLayer = NULL;
		}
		removeitem();
		showSelectFrame(Atrytpe[tag]);

		lastclickindex = tag;
		title->setString(JhCommonFuncs::gbk2utf(name[tag].c_str()));
		//heroppoint->setVisible(true);
		heroselectbg->setVisible(true);
		//heroppoint->setPosition(Vec2(propeImages[tag]->getPositionX(), propeImages[tag]->getPositionY() - propeImages[tag]->getContentSize().height / 2 - 5));

		//heroselectbg->setPositionY(heroppoint->getPositionY() - heroppoint->getContentSize().height + 3);

		showNewerGuide(++m_step);
		this->setLocalZOrder(1);
	}
}

void JhHeroProperNode::addCarryData(HeroAtrType index)
{
	if (g_hero->getIsOut())//在家以外，取背包数据
	{
		for (int i = 0; i < JhMyPackage::getSize(); i++)
		{
			PackageData data = JhMyPackage::vec_packages[i];
			if (index == H_WEAPON && data.type == WEAPON)//武器
				map_carryData[index].push_back(data);
			else if (index == H_GATHER && data.extype == 1 && data.type == TOOLS)//采集工具
				map_carryData[index].push_back(data);
			else if (index == H_FELL && data.extype == 2 && data.type == TOOLS)//砍伐工具
				map_carryData[index].push_back(data);
			else if (index == H_EXCAVATE && data.extype == 3 && data.type == TOOLS)//挖掘工具
				map_carryData[index].push_back(data);
			else if (index == H_WG && data.type == W_GONG)//外功
				map_carryData[index].push_back(data);
			else if (index == H_NG && data.type == N_GONG)//内功
				map_carryData[index].push_back(data);
			else if (index == H_ARMOR && data.type == PROTECT_EQU)//防具
				map_carryData[index].push_back(data);
			else if (index == H_MOUNT && data.type == RES_2 && data.extype == 4)
			{
				map_carryData[index].push_back(data);
			}
		}
		if (g_hero->getAtrByType(index)->count > 0)//已经装备上的放在最前面。hpdata->count == 0没有装备
		{
			PackageData sdata = *g_hero->getAtrByType(index);
			map_carryData[index].insert(map_carryData[index].begin(), sdata);
		}

	}
	else//在家取仓库数据
	{
		if (index == H_WEAPON)//武器
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[WEAPON].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[WEAPON][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_WG)//外功
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[W_GONG].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[W_GONG][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_NG)//内功
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[N_GONG].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[N_GONG][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_ARMOR)//防具
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[PROTECT_EQU].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[PROTECT_EQU][m];
				map_carryData[index].push_back(data);
			}
		}
		else if (index == H_MOUNT)
		{
			for (unsigned int m = 0; m < StorageRoom::map_storageData[RES_2].size(); m++)
			{
				PackageData data = StorageRoom::map_storageData[RES_2][m];
				if (data.extype == 4)
					map_carryData[index].push_back(data);
			}
		}
		else//工具
		{
			int toolsize = StorageRoom::map_storageData[TOOLS].size();

			if (toolsize > 0)
			{

				for (unsigned int m = 0; m < StorageRoom::map_storageData[TOOLS].size(); m++)
				{
					PackageData data = StorageRoom::map_storageData[TOOLS][m];
					if (data.extype == 1 && index == H_GATHER)//采集
						map_carryData[index].push_back(data);
					else if (data.extype == 2 && index == H_FELL)//砍伐
						map_carryData[index].push_back(data);
					else if (data.extype == 3 && index == H_EXCAVATE)//挖掘
						map_carryData[index].push_back(data);
				}
			}
		}
		if (g_hero->getAtrByType(index)->count > 0) // 已经装备上的放在最前面。hpdata->count == 0没有装备
		{
			PackageData sdata = *g_hero->getAtrByType(index);
			map_carryData[index].insert(map_carryData[index].begin(), sdata);
		}
		//std::string mixgfstr = JhGlobalData::getMixGF();
		//if (JhGlobalData::getMixGF().length() > 0)
		//{
		//	std::string mastergfid = JhGlobalData::map_MixGfData[mixgfstr].mastergf;
		//	WG_NGData gfdata = JhGlobalData::map_wgngs[mastergfid];
		//	int gftype = gfdata.type - 1;
		//	PackageData* carraydata = g_hero->getAtrByType(index);
		//	if ((gftype == W_GONG && index == H_WG && carraydata->strid.compare(mastergfid) != 0 && carraydata->count <= 0) || (gftype == N_GONG && index == H_NG && carraydata->strid.compare(mastergfid) != 0 && carraydata->count <= 0))
		//	{
		//		PackageData data;
		//		data.strid = gfdata.id;
		//		data.count = 1;
		//		data.lv = gfdata.maxlv - 1;
		//		data.type = gftype;
		//		data.extype = gfdata.extype;
		//		map_carryData[index].insert(map_carryData[index].begin(), data);
		//	}
		//}
	}
}
void JhHeroProperNode::showSelectFrame(HeroAtrType index)
{
	refreshCarryData();
	int tempsize = map_carryData[index].size();
	int itemheight = 165;
	int stX = 80;
	int jtX = 145;
	if (m_index != 1)
	{
		stX = 50;
		jtX = 120;
		itemheight = 80;
	}
	int row = tempsize % 4 == 0 ? tempsize / 4 : (tempsize / 4 + 1);
	int innerheight = itemheight * row;
	int contentheight = m_scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	m_scrollView->setInnerContainerSize(Size(650, innerheight));

	m_select->setVisible(false);
	if (index == H_MOUNT)
		amountdesc->setVisible(true);
	else
		amountdesc->setVisible(false);

	Color4B colorarr[6] = { Color4B(195, 159, 97, 255), Color4B(195, 159, 97, 255), Color4B(90, 180, 135, 255), Color4B(90, 161, 181, 255), Color4B(159, 81, 171, 255), Color4B(193, 128, 71, 255) };
	for (int i = 0; i < tempsize; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = map_carryData[index][i];
		int colordex = 0;
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
			colordex = JhGlobalData::map_equips[tmpdata.strid].qu;
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
			colordex = JhGlobalData::map_wgngs[tmpdata.strid].qu;
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhHeroProperNode::onItem, this));
		boxItem->setTag(index);
		boxItem->setUserData(&map_carryData[index][i]);
		if (i == 0 && g_hero->getAtrByType(index)->count > 0)
		{
			m_lastSelectedData = &map_carryData[index][i];
		}

		if (m_index != 1)
		{
			boxItem->setScale(0.7f);
		}

		boxItem->setPosition(Vec2(stX + i % 4 * jtX, innerheight - i / 4 * itemheight - itemheight / 2));
		JhMyMenu* menu = JhMyMenu::create();
		menu->setTouchlimit(m_scrollView);
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		m_scrollView->addChild(menu, 0, name);

		std::string rstrid = map_carryData[index][i].strid;
		std::string str = StringUtils::format("ui/%s.png", rstrid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%s", JhGlobalData::map_allResource[rstrid].cname.c_str());
		Label * namelbl = Label::createWithTTF(str, "fonts/SIMHEI.TTF", 22);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->enableOutline(colorarr[colordex], 3);
		namelbl->setPosition(Vec2(box->getContentSize().width / 2, - 15));
		box->addChild(namelbl);

		Label * lvlbl = Label::createWithTTF("", "fonts/STXINGKA.TTF", 15);//Label::createWithSystemFont("", "", 15);
		lvlbl->setColor(Color3B(255, 255, 255));
		lvlbl->setAnchorPoint(Vec2(1, 0));
		lvlbl->setPosition(Vec2(box->getContentSize().width - 10, 8));
		box->addChild(lvlbl);

		std::string mymixgf = JhGlobalData::getMixGF();
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if (mymixgf.length() > 0)
		{
			if (rstrid.compare(mdata.mastergf) == 0)
			{
				Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
				mixtag->setPosition(Vec2(box->getContentSize().width - 15, box->getContentSize().height - 15));
				box->addChild(mixtag);
			}
		}

		if (index == H_WG || index == H_NG )
		{
			int lv = map_carryData[index][i].lv + 1;
			str = StringUtils::format("Lv.%d", lv);

			if (lv >= JhGlobalData::map_wgngs[map_carryData[index][i].strid].maxlv)
				str = StringUtils::format("Lv.%d(满级)", lv);
		}
		else if (index == H_GATHER || index == H_FELL || index == H_EXCAVATE || index == H_ARMOR || index == H_WEAPON)
		{
			str = StringUtils::format("耐久度%d%%", map_carryData[index][i].goodvalue);
		}
		else if (index == H_MOUNT)
		{
			if (map_carryData[index][i].strid.compare("74") == 0)
			{
				str = StringUtils::format("生命%d", map_carryData[index][i].goodvalue);
			}
			else
				str = "永久";
		}
		else
		{
			str = "";
		}
		if (map_carryData[index][i].goodvalue < 20)
			lvlbl->setColor(Color3B(204, 4, 4));
		else
			lvlbl->setColor(Color3B(255, 255, 255));

		lvlbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));

		if (g_hero->getAtrByType(index)->count > 0)//是否装备了hpdata->count ==0没有装备
		{
			if (i == 0)
			{
				m_select->setPosition(Vec2(boxItem->getPositionX() - boxItem->getContentSize().width / 2, boxItem->getPositionY() + boxItem->getContentSize().height / 2));
				m_select->setVisible(true);
			}
		}
	}
}

void JhHeroProperNode::onItem(Ref* pSender)
{
	SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);

	if (g_NewerGuideLayer != NULL)
	{
		g_NewerGuideLayer->removeFromParentAndCleanup(true);
		g_NewerGuideLayer = NULL;
	}

	if (JhGlobalData::isExercising() && !JhGlobalData::isHasFSF())
	{
		int index = -1;
		for (unsigned int i = 0; i < JhGlobalData::vec_goods.size(); i++)
		{
			if (JhGlobalData::vec_goods[i].icon.compare("72") == 0)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			JhBuyComfirmLayer* layer = JhBuyComfirmLayer::create(&JhGlobalData::vec_goods[index]);
			Director::getInstance()->getRunningScene()->addChild(layer, 1000, "buycomfirmlayer");
		}
		return;
	}

	m_select_itemnode = (Node*)pSender;
	m_select_atrype = (HeroAtrType)m_select_itemnode->getTag();
	m_select_udata = (PackageData*)m_select_itemnode->getUserData();


	if (g_hero->getSex() == S_MAN && (m_select_udata->strid.compare("w022") == 0 || m_select_udata->strid.compare("w040") == 0))
	{
		SpecialHintLayer* layer = SpecialHintLayer::create();
		g_gameLayer->addChild(layer, 11);
		return;
	}

	selectCarryData();

	showNewerGuide(++m_step);
}

void JhHeroProperNode::selectCarryData()
{
	if (m_lastSelectedData != NULL)
	{
		if (m_lastSelectedData != m_select_udata)//是否再次点击
		{
			//是否在同一种类型中切换装备，如果是先卸下，在装备上选中的
			bool issametool = false;
			bool issameother = false;
			if (m_lastSelectedData->type == TOOLS)
			{
				issametool = m_lastSelectedData->type == m_select_udata->type &&  m_lastSelectedData->extype == m_select_udata->extype;
			}
			else
			{
				issameother = m_lastSelectedData->type == m_select_udata->type;
			}
			if (issametool || issameother)
			{
				if (!isCanTakeOn())
					return;

				if (isout)
				{

					if (m_select->isVisible())
					{
						if (JhMyPackage::add(*m_lastSelectedData) < 0)
						{
							JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("背包已满！！"));
							Director::getInstance()->getRunningScene()->addChild(hint, 4);
							return;
						}
					}
				}
				else
				{
					if (m_select->isVisible())
					{
						if (m_lastSelectedData->type == N_GONG || m_lastSelectedData->type == W_GONG)
						{
							if (StorageRoom::getCountById(m_lastSelectedData->strid) <= 0)
								StorageRoom::add(*m_lastSelectedData);
						}
						else
							StorageRoom::add(*m_lastSelectedData);
	
					}
				}
			}
			m_select->setVisible(false);
		}

	}

	m_select->setPosition(Vec2(m_select_itemnode->getPositionX() - m_select_itemnode->getContentSize().width / 2, m_select_itemnode->getPositionY() + m_select_itemnode->getContentSize().height / 2));

	PackageData curCarrydata = *m_select_udata;
	//之前是选中m_select可见，现在点了就是没选中
	m_select->setVisible(!m_select->isVisible());
	if (m_select->isVisible())
	{
		if (!isCanTakeOn())
		{
			m_select->setVisible(!m_select->isVisible());
			return;
		}
		takeon(m_select_atrype, curCarrydata);
	}
	else
	{
		if (!takeoff(m_select_atrype))
		{
			m_select->setVisible(!m_select->isVisible());
			return;
		}
	}
	m_lastSelectedData = m_select_udata;

	updataMyPackageUI();

	JhHeroStateUILayer* heroStateUiLayer = (JhHeroStateUILayer*)g_gameLayer->getChildByName("JhHeroStateUILayer");

	if (heroStateUiLayer != NULL)
		heroStateUiLayer->updateArrow();
}

void JhHeroProperNode::takeon(HeroAtrType atrype, PackageData pdata)
{
	if (g_hero->getIsOut())
		JhMyPackage::cutone(pdata);
	else
		StorageRoom::use(pdata);

	g_hero->setAtrByType(atrype, pdata);

	updataProperpanel(lastclickindex, pdata);
}

bool JhHeroProperNode::takeoff(HeroAtrType atrype)
{
	PackageData mydata = *g_hero->getAtrByType(atrype);
	if (g_hero->getIsOut())
	{
		if (JhMyPackage::add(mydata) < 0)
		{
			JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("背包已满！！"));
			Director::getInstance()->getRunningScene()->addChild(hint, 4);
			return false;
		}
	}
	else
	{
		if (mydata.type == N_GONG || mydata.type == W_GONG)
		{
			if (StorageRoom::getCountById(mydata.strid) <= 0)
				StorageRoom::add(mydata);
		}
		else
		{
			StorageRoom::add(mydata);
		}
	}

	std::string strid = mydata.strid;
	if (mydata.type >= TOOLS)
		strid = StringUtils::format("hp%d-%d", m_select_udata->type + 1, m_select_udata->extype);
	else
		strid = StringUtils::format("hp%d", m_select_udata->type + 1);

	g_hero->getAtrByType(atrype)->count = 0;
	lvtext[lastclickindex]->setString("");
	zbzbz[lastclickindex]->setVisible(false);

	std::string str = "ui/buildsmall.png";

	imgbtn[lastclickindex]->removeAllChildrenWithCleanup(true);

	imgbtn[lastclickindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	imgbtn[lastclickindex]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	str = StringUtils::format("ui/%s.png", strid.c_str());
	propeImages[lastclickindex]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	return true;
}

void JhHeroProperNode::updataProperpanel(int atrypeindex, PackageData pdata)
{
	HeroAtrType atrype = Atrytpe[atrypeindex];
	std::string str; 
	if (atrype == H_WG || atrype == H_NG)
	{
		int lv = pdata.lv + 1;
		str = StringUtils::format("Lv.%d", lv);

		if (lv >= JhGlobalData::map_wgngs[pdata.strid].maxlv)
			str = StringUtils::format("Lv.%d(满级)", lv);

		std::string qustr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[pdata.strid].qu);
		imgbtn[atrypeindex]->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
		imgbtn[atrypeindex]->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());
	}
	else if (atrype == H_GATHER || atrype == H_FELL || atrype == H_EXCAVATE || atrype == H_WEAPON || atrype == H_ARMOR)
	{
		str = StringUtils::format("耐久度%d%%", pdata.goodvalue);

		if (atrype == H_WEAPON || atrype == H_ARMOR)
		{
			std::string qustr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[pdata.strid].qu);
			imgbtn[atrypeindex]->loadTexture(qustr, cocos2d::ui::TextureResType::PLIST);
			imgbtn[atrypeindex]->setContentSize(Sprite::createWithSpriteFrameName(qustr)->getContentSize());
		}
	}
	else if (atrype == H_MOUNT)
	{
		if (pdata.strid.compare("74") == 0)
		{
			str = StringUtils::format("生命%d", pdata.goodvalue);
		}
		else
			str = "永久";
	}
	else
	{
		str = "";
	}

	if (pdata.goodvalue < 20)
		lvtext[atrypeindex]->setColor(Color3B(204, 4, 4));
	else
		lvtext[atrypeindex]->setColor(Color3B(255, 255, 255));

	lvtext[atrypeindex]->setString(JhCommonFuncs::gbk2utf(str.c_str()));
	zbzbz[atrypeindex]->setVisible(true);

	std::string strid = StringUtils::format("ui/%s.png", pdata.strid.c_str());
	propeImages[atrypeindex]->loadTexture(strid, cocos2d::ui::TextureResType::PLIST);

	imgbtn[atrypeindex]->removeAllChildrenWithCleanup(true);

	std::string mymixgf = JhGlobalData::getMixGF();
	MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
	if (mymixgf.length() > 0)
	{
		if (pdata.strid.compare(mdata.mastergf) == 0)
		{
			Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
			mixtag->setPosition(Vec2(imgbtn[atrypeindex]->getContentSize().width - 15, imgbtn[atrypeindex]->getContentSize().height - 15));
			imgbtn[atrypeindex]->addChild(mixtag);
		}
	}
}

void JhHeroProperNode::removeitem()
{
	if (lastclickindex >= 0)
	{
		for (unsigned int i = 0; i < map_carryData[Atrytpe[lastclickindex]].size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			m_scrollView->removeChildByName(name);
		}
	}
}

void JhHeroProperNode::onExit()
{
	g_hero->saveProperData();
	Node::onExit();
}

void JhHeroProperNode::refreshCarryData()
{
	removeitem();
	map_carryData.clear();
	for (int i = 0; i < 8; i++)
		addCarryData(Atrytpe[i]);
}

void JhHeroProperNode::updataMyPackageUI()
{
	if (g_gameLayer != NULL)
	{
		JhActionGetLayer * Alayer = (JhActionGetLayer*)g_gameLayer->getChildByName("JhActionGetLayer");
		if (Alayer != NULL)
			Alayer->updataMyPackageUI();
		TempStorageLayer* Tlayer = (TempStorageLayer*)g_gameLayer->getChildByName("TempStorageLayer");
		if (Tlayer != NULL)
			Tlayer->updataMyPackageUI();

		Winlayer* Wlayer = (Winlayer*)g_gameLayer->getChildByName("Winlayer");
		if (Wlayer != NULL)
			Wlayer->updataMyPackageUI();

		JhOutDoor* Olayer = (JhOutDoor*)g_gameLayer->getChildByName("JhOutDoor");
		if (Olayer != NULL)
		{
			Olayer->updata();
		}
	}
}
void JhHeroProperNode::refreshGF(HeroAtrType atrype)
{
	int index = 0;

	PackageData mydata = *g_hero->getAtrByType(atrype);

	if (mydata.type == N_GONG || mydata.type == W_GONG)
	{
		if (StorageRoom::getCountById(mydata.strid) <= 0)
			StorageRoom::add(mydata);
	}
	else
		StorageRoom::add(mydata);

	g_hero->getAtrByType(atrype)->count = 0;

	if (atrype == H_WG)
		index = 0;
	else if (atrype == H_NG)
		index = 1;
	lvtext[index]->setString("");
	zbzbz[index]->setVisible(false);
	std::string str = "ui/buildsmall.png";
	imgbtn[index]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	imgbtn[index]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

	str = StringUtils::format("ui/hp%d.png", mydata.type + 1);

	propeImages[index]->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
	propeImages[index]->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());
}

void JhHeroProperNode::removeMixTag()
{
	if (g_hero->getAtrByType(H_WG)->count > 0 || g_hero->getAtrByType(H_NG)->count > 0)
	{
		imgbtn[0]->removeAllChildrenWithCleanup(true);
		imgbtn[1]->removeAllChildrenWithCleanup(true);
	}
}

int JhHeroProperNode::checkSex(int sex)
{
	std::string mymixgf = JhGlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if (sex == S_MAN)
		{
			if (mdata.sex == 1 || mdata.sex == 3 || mdata.sex == 4)
				return 1;
			else
			{
				return -mdata.sex;
			}
		}
		else if (sex == S_WOMEN)
		{
			if (mdata.sex == 2 || mdata.sex == 3 || mdata.sex == 4)
				return 1;
			else
				return -mdata.sex;
		}
		else if (sex == S_NONE)
		{
			if (mdata.sex == 0 || mdata.sex == 4)
				return 1;
			else
				return -mdata.sex;
		}
	}
	return 1;
}

bool JhHeroProperNode::isCanTakeOn()
{
	bool ret = true;
	std::string mymixgf = JhGlobalData::getMixGF();
	MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
	if (mymixgf.length() > 0)
	{

		int csex = checkSex(g_hero->getSex());
		if (m_select_udata->strid.compare(mdata.mastergf) == 0 && csex <= 0)
		{
			std::string sexstr;
			if (csex == 0)
				sexstr = StringUtils::format("%s%s", mdata.name.c_str(), JhCommonFuncs::gbk2utf("只适合自宫者装备！！").c_str());
			else if (csex == -1)
				sexstr = StringUtils::format("%s%s", mdata.name.c_str(), JhCommonFuncs::gbk2utf("只适合男性装备！！").c_str());
			else if (csex == -2)
				sexstr = StringUtils::format("%s%s", mdata.name.c_str(), JhCommonFuncs::gbk2utf("只适合女性装备！！").c_str());
			else
				sexstr = StringUtils::format("%s%s", mdata.name.c_str(), JhCommonFuncs::gbk2utf("不适合自宫者装备！！").c_str());
			JhHintBox* hint = JhHintBox::create(sexstr);
			Director::getInstance()->getRunningScene()->addChild(hint, 4);
			return false;
		}


		std::string othergf;
		if (m_select_udata->type == N_GONG)
		{
			if (g_hero->getAtrByType(H_WG)->count > 0)
			{
				othergf = g_hero->getAtrByType(H_WG)->strid;
			}
		}
		else if (m_select_udata->type == W_GONG)
		{
			if (g_hero->getAtrByType(H_NG)->count > 0)
			{
				othergf = g_hero->getAtrByType(H_NG)->strid;
			}
		}
		std::string comparegf;
		std::string descstr;

		if (m_select_udata->strid.compare(mdata.mastergf) == 0)
		{
			comparegf = othergf;
			descstr = StringUtils::format("%s%s%s%s", JhCommonFuncs::gbk2utf("学习").c_str(), "%s", JhCommonFuncs::gbk2utf("，无法使用").c_str(), mdata.name.c_str());

		}
		else if (othergf.compare(mdata.mastergf) == 0)
		{
			comparegf = m_select_udata->strid;
			descstr = StringUtils::format("%s%s%s%s", JhCommonFuncs::gbk2utf("学习").c_str(), mdata.name.c_str(), JhCommonFuncs::gbk2utf("，无法使用").c_str(), "%s");
		}
		if (comparegf.length() > 0)
		{
			for (unsigned int i = 0; i < mdata.vec_mutexgf.size(); i++)
			{
				if (mdata.vec_mutexgf[i].compare(comparegf) == 0)
				{
					std::string str = StringUtils::format(descstr.c_str(), JhGlobalData::map_allResource[mdata.vec_mutexgf[i]].cname.c_str());
					JhHintBox* hint = JhHintBox::create(str);
					Director::getInstance()->getRunningScene()->addChild(hint, 4);
					return false;
				}
			}
		}

	}

	return ret;
}

void JhHeroProperNode::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	Node* resItemNode = NULL;
	if (step == 3)
		nodes.push_back(propeImages[0]);
	else if (step == 4 || step == 6 || step == 8 || step == 10)
	{
		resItemNode = m_scrollView->getChildByName("resitem0");
		if (resItemNode != NULL)
		{
			if (resItemNode->getChildrenCount() > 0)
				nodes.push_back(resItemNode->getChildren().at(0));
		}
		if (JhNewerGuideLayer::checkifNewerGuide(step))
			m_scrollView->setEnabled(false);
	}
	else if (step == 5)
	{
		nodes.push_back(propeImages[1]);
	}
	else if (step == 7)
	{
		nodes.push_back(propeImages[2]);
	}
	else if (step == 9)
	{
		nodes.push_back(propeImages[3]);
	}
	else if (step == 11)
	{
		//nodes.push_back(heroselectbg->getChildByName("okbtn"));
	}
	if (step <= 11 && nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}

JhHeroProperNode* JhHeroProperNode::create(int content)
{
	JhHeroProperNode *pRet = new JhHeroProperNode();
	if (pRet && pRet->init(content))
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