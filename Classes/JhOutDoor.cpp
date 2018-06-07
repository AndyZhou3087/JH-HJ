#include "JhOutDoor.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhMapLayer.h"
#include "JhStorageRoom.h"
#include "JhGameScene.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhActivitScene.h"
#include "JhMyMenu.h"
#include "JhResDetailsLayer.h"
#include "JhNewerGuideLayer.h"

JhOutDoor::JhOutDoor()
{
	lastSrollViewHeight = -1;
	lastrows = 0;
	m_isLongPress = false;
	m_longTouchNode = NULL;
}


JhOutDoor::~JhOutDoor()
{
	lastSrollViewHeight = -1;
}

bool JhOutDoor::init()
{
	m_csbnode = CSLoader::createNode("jhoutDoorLayer.csb");
	this->addChild(m_csbnode, 0, "csbnode");

	m_heroproper = JhHeroProperNode::create(2);
	m_heroproper->setPosition(Vec2(360, 790));
	m_csbnode->addChild(m_heroproper, 1, "JhHeroProperNode");

	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	updata();

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhOutDoor::onBack, this));


	m_outbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("outbtn");
	m_outbtn->addTouchEventListener(CC_CALLBACK_2(JhOutDoor::onOut, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	showNewerGuide(19);

	return true;
}



void JhOutDoor::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (int i = 0; i < JhMyPackage::getSize(); i++)
		{
			JhStorageRoom::add(JhMyPackage::vec_packages[i]);
		}
		JhMyPackage::takeoff();
		this->removeFromParentAndCleanup(true);
	}
}

void JhOutDoor::onOut(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_outbtn->setEnabled(false);
		this->removeFromParentAndCleanup(true);
		g_gameLayer->scheduleOnce(schedule_selector(JhGameScene::delayShowOutScence), 0.02f);
	}
}

void JhOutDoor::updata()
{
	//更新仓库栏
	updataStorageUI();
	//更新背包栏
	updataMyPackageUI();
}

void JhOutDoor::updataMyPackageUI()
{
	for (int i = 0; i < JhMyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		m_csbnode->removeChildByName(name);
	}

	int countindex = -1;
	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = JhMyPackage::vec_packages[i];

		bool isin = false;
		std::map<std::string, AllResource>::iterator resit;
		for (resit = JhGlobalData::map_allResource.begin(); resit != JhGlobalData::map_allResource.end(); resit++)
		{
			if (tmpdata.strid.compare(JhGlobalData::map_allResource[resit->first].strid) == 0)
			{
				isin = true;
				countindex++;
				break;
			}
		}
		if (!isin)
			continue;

		std::string countorlvstr = StringUtils::format("%d", tmpdata.count);
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.slv);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.lv+1);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhOutDoor::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(150 + countindex * 140, 520));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", countindex);
		m_csbnode->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", JhMyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		Label * reslbl = Label::createWithTTF(countorlvstr, "fonts/SIMHEI.TTF", 18);//Label::createWithSystemFont(str, "", 18);
		reslbl->enableOutline(Color4B(142, 101, 54, 255), 2);
		reslbl->setAnchorPoint(Vec2(1, 0.5));
		reslbl->setPosition(Vec2(box->getContentSize().width - 20, 5));
		box->addChild(reslbl);

		std::string mymixgf = JhGlobalData::getMixGF();
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if (mymixgf.length() > 0)
		{
			if (JhMyPackage::vec_packages[i].strid.compare(mdata.mastergf) == 0)
			{
				Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
				mixtag->setPosition(Vec2(box->getContentSize().width - 15, box->getContentSize().height - 15));
				box->addChild(mixtag);
			}
		}
	}
}

void JhOutDoor::updataStorageUI()
{
	for (unsigned int i = 0; i < allStorageData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		scrollview->removeChildByName(name);
	}

	int typecount = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		typecount += JhStorageRoom::map_storageData[i].size();
	}

	int row = typecount % 4 == 0 ? typecount / 4 : (typecount / 4 + 1);

	int innerheight = scrollview->getInnerContainerSize().height;
	if (lastSrollViewHeight < 0 || lastrows < row)
	{
		innerheight = row * 130;
		int contentheight = scrollview->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastSrollViewHeight = innerheight;
		scrollview->setInnerContainerSize(Size(570, innerheight));
	}
	lastrows = row;

	allStorageData.clear();
	for (int i = 0; i < RES_MAX; i++)
	{
		for (unsigned int m = 0; m < JhStorageRoom::map_storageData[i].size(); m++)
		{
			allStorageData.push_back(&JhStorageRoom::map_storageData[i][m]);
		}
	}

	int countindex = -1;
	for (unsigned int i = 0; i < allStorageData.size(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = *allStorageData[i];

		bool isin = false;
		std::map<std::string, AllResource>::iterator resit;
		for (resit = JhGlobalData::map_allResource.begin(); resit != JhGlobalData::map_allResource.end(); resit++)
		{
			if (tmpdata.strid.compare(JhGlobalData::map_allResource[resit->first].strid) == 0)
			{
				isin = true;
				countindex++;
				break;
			}
		}
		if (!isin)
			continue;

		std::string countorlvstr = StringUtils::format("%d", tmpdata.count);

		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.slv);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
			countorlvstr = StringUtils::format("Lv%d", tmpdata.lv + 1);
		}
		std::string name = StringUtils::format("resitem%d", countindex);
		cocos2d::ui::ImageView* boxItem = cocos2d::ui::ImageView::create(boxstr, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addTouchEventListener(CC_CALLBACK_2(JhOutDoor::onStorageItem, this));
		boxItem->setTouchEnabled(true);
		scrollview->addChild(boxItem, 0, name);
		boxItem->setUserData(allStorageData[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + countindex % 4 * 140, innerheight - boxItem->getContentSize().height / 2 - countindex / 4 * 130));

		std::string str = StringUtils::format("ui/%s.png", allStorageData[i]->strid.c_str());

		cocos2d::ui::ImageView* res = cocos2d::ui::ImageView::create(str, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addChild(res, 0, "res");
		res->setPosition(Vec2(boxItem->getContentSize().width / 2, boxItem->getContentSize().height / 2));

		Label * reslbl = Label::createWithTTF(countorlvstr, "fonts/SIMHEI.TTF", 18);
		reslbl->enableOutline(Color4B(142, 101, 54, 255), 2);
		boxItem->addChild(reslbl, 0, "lbl");
		reslbl->setAnchorPoint(Vec2(1, 0.5f));

		reslbl->setPosition(Vec2(boxItem->getContentSize().width - 20, 5));

		std::string mymixgf = JhGlobalData::getMixGF();
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if (mymixgf.length() > 0)
		{
			if (allStorageData[i]->strid.compare(mdata.mastergf) == 0)
			{
				Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
				mixtag->setPosition(Vec2(boxItem->getContentSize().width - 15, boxItem->getContentSize().height - 15));
				boxItem->addChild(mixtag, 0, "mixtag");
			}
		}
	}
}

void JhOutDoor::longTouchUpdate(float delay){
	m_isLongPress = true;
	if (m_longTouchNode != NULL){
		std::string name = m_longTouchNode->getName();
		//if (name.find("resitem") != std::string::npos)
		{
			unschedule(schedule_selector(JhOutDoor::longTouchUpdate));
			JhResDetailsLayer::whereClick = 2;
			PackageData* data = (PackageData*)m_longTouchNode->getUserData();
			this->addChild(JhResDetailsLayer::create(data));
		}
	}
}

void JhOutDoor::onStorageItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_longTouchNode = node;
		schedule(schedule_selector(JhOutDoor::longTouchUpdate), 1.0f);

	}

	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		unschedule(schedule_selector(JhOutDoor::longTouchUpdate));
		if (!m_isLongPress)
		{
			takeout((PackageData*)node->getUserData());
			updata();
		}
		m_isLongPress = false;
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		unschedule(schedule_selector(JhOutDoor::longTouchUpdate));
		m_isLongPress = false;
	}
}

void JhOutDoor::takeout(PackageData* pdata)
{
	PackageData* data = pdata;

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = JhStorageRoom::map_storageData[data->type].begin(); it != JhStorageRoom::map_storageData[data->type].end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0 && it->goodvalue == data->goodvalue)
			{
				PackageData pdata = *data;
				pdata.count = 1;
				if (JhMyPackage::add(pdata) == 0)
				{
					JhStorageRoom::use(pdata);
				}
				break;
			}
		}
	}
	else
	{
		PackageData pdata = *data;
		pdata.count = 1;
		if (JhMyPackage::add(pdata) == 0)
		{
			JhStorageRoom::use(data->strid);
		}
	}
	m_heroproper->refreshCarryData();
}

void JhOutDoor::onPackageItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = JhMyPackage::vec_packages[index];
	data.count = 1;
	JhStorageRoom::add(data);
	JhMyPackage::cutone(data);
	m_heroproper->refreshCarryData();
	updata();
}


void JhOutDoor::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 19)
	{
		nodes.push_back(m_outbtn);
	}
	g_gameLayer->showNewerGuide(step, nodes);
}
