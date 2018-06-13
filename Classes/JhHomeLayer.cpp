#include "JhHomeLayer.h"
#include "json.h"
#include "JhBed.h"
#include "JhFurnace.h"
#include "JhForgingtable.h"
#include "JhCookTable.h"
#include "JhMedicineKit.h"
#include "JhWineTable.h"
#include "JhWineMaker.h"
#include "JhStorageRoom.h"
#include "JhFence.h"
#include "JhExerciseRoom.h"
#include "JhBuildingUILayer.h"
#include "JhStorageUILayer.h"
#include "JhOutDoor.h"
#include "JhGameScene.h"
#include "JhSoundManager.h"
#include "JhNewerGuideLayer.h"
#include "JhHintBox.h"
#include "JhCommonFuncs.h"
#include "JhBuyComfirmLayer.h"
#include "JhBookShelf.h"
#include "JhBookShelfLayer.h"

JhHomeLayer::JhHomeLayer()
{
}


JhHomeLayer::~JhHomeLayer()
{
}

bool JhHomeLayer::init()
{
	Node* csbnode = CSLoader::createNode("jhhomeLayer.csb");
	this->addChild(csbnode);

	JhBuilding* bed = JhBed::create();
	Vec_Buildings.push_back(bed);

	m_badfurnace = (cocos2d::ui::Widget*)csbnode->getChildByName("badfurnace");
	m_badfurnace->setLocalZOrder(1);

	m_badbookshelf = (cocos2d::ui::Widget*)csbnode->getChildByName("badbookshelf");
	m_badbookshelf->setLocalZOrder(1);

	MenuItemSprite* bedItem = MenuItemSprite::create(
		bed,
		bed,
		bed,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	bedItem->setName("bed");
	bedItem->setTag(0);
	bedItem->setPosition(Vec2(254, 81));
	bedItem->setOpacity(0);
	Menu* menu = Menu::create();
	menu->addChild(bedItem);
	menu->setPosition(Vec2(0, 0));
	csbnode->addChild(menu);

	JhBuilding* cooktable = JhCookTable::create();
	Vec_Buildings.push_back(cooktable);
	MenuItemSprite* cooktableItem = MenuItemSprite::create(
		cooktable,
		cooktable,
		cooktable,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	cooktableItem->setName("cooktable");
	cooktableItem->setTag(1);
	cooktableItem->setOpacity(0);
	cooktableItem->setAnchorPoint(Vec2(0.53, 0.16));
	cooktableItem->setPosition(Vec2(400, 603));
	menu->addChild(cooktableItem);

	JhBuilding* forgingtable = JhForgingtable::create();
	Vec_Buildings.push_back(forgingtable);
	MenuItemSprite* forgingtableItem = MenuItemSprite::create(
		forgingtable,
		forgingtable,
		forgingtable,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	forgingtableItem->setName("forgingtable");
	forgingtableItem->setOpacity(0);
	forgingtableItem->setTag(2);
	forgingtableItem->setAnchorPoint(Vec2(0.61, 0.25));
	forgingtableItem->setPosition(Vec2(579, 534));
	menu->addChild(forgingtableItem);

	JhBuilding* winemaker = JhWineMaker::create();
	Vec_Buildings.push_back(winemaker);
	MenuItemSprite* winemakerItem = MenuItemSprite::create(
		winemaker,
		winemaker,
		winemaker,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	winemakerItem->setName("winemaker");
	winemakerItem->setOpacity(0);
	winemakerItem->setTag(3);
	winemakerItem->setAnchorPoint(Vec2(0.27, 0.42));
	winemakerItem->setPosition(Vec2(31, 557));
	menu->addChild(winemakerItem);

	JhBuilding* medicinekit = JhMedicineKit::create();
	Vec_Buildings.push_back(medicinekit);
	MenuItemSprite* medicinekitItem = MenuItemSprite::create(
		medicinekit,
		medicinekit,
		medicinekit,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	medicinekitItem->setName("medicinekit");
	medicinekitItem->setOpacity(0);
	medicinekitItem->setTag(4);
	medicinekitItem->setAnchorPoint(Vec2(0.42, 0.24));
	medicinekitItem->setPosition(Vec2(612, 85));
	menu->addChild(medicinekitItem);


	JhBuilding* furnace = JhFurnace::create();
	Vec_Buildings.push_back(furnace);
	MenuItemSprite* furnaceItem = MenuItemSprite::create(
		furnace,
		furnace,
		furnace,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	furnaceItem->setName("furnace");
	furnaceItem->setTag(5);
	furnaceItem->setOpacity(0);
	furnaceItem->setPosition(Vec2(444, 199));
	menu->addChild(furnaceItem);

	JhBuilding* winetable = JhWineTable::create();
	Vec_Buildings.push_back(winetable);
	MenuItemSprite* winetableItem = MenuItemSprite::create(
		winetable,
		winetable,
		winetable,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	winetableItem->setName("winetable");
	winetableItem->setOpacity(0);
	winetableItem->setTag(6);
	winetableItem->setAnchorPoint(Vec2(0.51, 0.70));
	winetableItem->setPosition(Vec2(268, 437));
	menu->addChild(winetableItem);

	JhBuilding* exersiceroom = JhExerciseRoom::create();
	Vec_Buildings.push_back(exersiceroom);
	MenuItemSprite* exersiceroomItem = MenuItemSprite::create(
		exersiceroom,
		exersiceroom,
		exersiceroom,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	exersiceroomItem->setName("exersiceroom");
	exersiceroomItem->setOpacity(0);
	exersiceroomItem->setTag(7);
	exersiceroomItem->setAnchorPoint(Vec2(0.79, 0.45));
	exersiceroomItem->setPosition(Vec2(661, 359));
	menu->addChild(exersiceroomItem);

	JhBuilding* bookshelf = JhBookShelf::create();
	Vec_Buildings.push_back(bookshelf);
	MenuItemSprite* bookshelfItem = MenuItemSprite::create(
		bookshelf,
		bookshelf,
		bookshelf,
		CC_CALLBACK_1(JhHomeLayer::onclick, this));
	bookshelfItem->setName("bookshelf");
	bookshelfItem->setOpacity(0);
	bookshelfItem->setTag(8);
	bookshelfItem->setPosition(Vec2(674, 750));
	menu->addChild(bookshelfItem);

	loadJsonData();

	m_storageroom = JhStorageRoom::create();
	MenuItemSprite* storageroomItem = MenuItemSprite::create(
		m_storageroom,
		m_storageroom,
		m_storageroom,
		CC_CALLBACK_1(JhHomeLayer::onStorageRoom, this));
	storageroomItem->setPosition(Vec2(62, 354));
	menu->addChild(storageroomItem);

	m_fence = JhFence::create();
	MenuItemSprite* fenceItem = MenuItemSprite::create(
		m_fence,
		m_fence,
		m_fence,
		CC_CALLBACK_1(JhHomeLayer::onFence, this));
	fenceItem->setPosition(Vec2(165, 648));
	menu->addChild(fenceItem);

	updateBuilding();

	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		JhStorageRoom::add(JhMyPackage::vec_packages[i]);
	}
	JhMyPackage::takeoff();

	g_hero->setIsOut(false);

	g_uiScroll->setVisible(false);
	scrollbg->setVisible(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	JhSoundManager::getInstance()->playBackMusic(JhSoundManager::MUSIC_ID_HOME);

	this->scheduleOnce(schedule_selector(JhHomeLayer::delayShowNewerGuide), 0.2f);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (JhGlobalData::isOnline)
		JhServerDataSwap::init(NULL)->postOneData(JhGlobalData::getUId());
#endif

	return true;
}

void JhHomeLayer::onclick(Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	std::string nodename = node->getName();
	int nodetag = node->getTag();

	if (nodename.compare("bookshelf") == 0 && Vec_Buildings[nodetag]->data.level >= Vec_Buildings[nodetag]->data.maxlevel)
	{
		Layer* layer = JhBookShelfLayer::create();
		g_gameLayer->addChild(layer, 4);
	}
	else
	{
		Layer* layer = JhBuildingUILayer::create(Vec_Buildings[nodetag]);
		g_gameLayer->addChild(layer, 4, "builduilayer");
	}

}

void JhHomeLayer::loadJsonData()
{
	//建筑的JSON数据
	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < Vec_Buildings.size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		Vec_Buildings[i]->parseData(oneBuild);
	}
}

void JhHomeLayer::onStorageRoom(Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Layer* layer = JhStorageUILayer::create();
	g_gameLayer->addChild(layer, 10, "storageuilayer");
}

void JhHomeLayer::onFence(Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);

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

	Layer* layer = JhOutDoor::create();
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(layer, 10, "JhOutDoor");
}

void JhHomeLayer::updateBuilding()
{
	for (unsigned int i = 0; i < Vec_Buildings.size(); i++)
	{
		MenuItemSprite* item = (MenuItemSprite*)Vec_Buildings[i]->getParent();
		if (Vec_Buildings[i]->data.level >= 1)
		{
			item->setOpacity(255);
			if (i == 5)
				m_badfurnace->setVisible(false);
			else if (i == 8)
				m_badbookshelf->setVisible(false);
		}
	}
}

void JhHomeLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 0 || step == 43)
	{
		nodes.push_back(Vec_Buildings[0]->getParent());
	}
	else if (step == 18)
	{
		nodes.push_back(m_fence->getParent());
	}
	else if (step == 45)
	{
		nodes.push_back(m_storageroom->getParent());
	}
	else if (step > 55)
	{
		nodes.push_back(Vec_Buildings[step - 55]->getParent());
	}
	g_gameLayer->showNewerGuide(step, nodes);
}

void JhHomeLayer::delayShowNewerGuide(float dt)
{
	if (JhNewerGuideLayer::checkifNewerGuide(0))
		showNewerGuide(0);
	else if (JhNewerGuideLayer::checkifNewerGuide(43))
		showNewerGuide(43);
	else
	{
		std::vector<PackageData>::iterator it;
		//有足够资源引导建造
		for (unsigned int i = 1; i < Vec_Buildings.size(); i++)
		{
			int findcount = 0;
			int bulidressize = Vec_Buildings[i]->data.Res[0].size();
			for (int n = 0; n < bulidressize; n++)
			{
				int buildresid = Vec_Buildings[i]->data.Res[0][n] / 1000;
				int buildrescount = Vec_Buildings[i]->data.Res[0][n] % 1000;
				std::string strResid = StringUtils::format("%d", buildresid);
				for (int m = 0; m < RES_MAX; m++)
				{
					for (unsigned int k = 0; k < JhStorageRoom::map_storageData[m].size(); k++)
					{
						if (JhStorageRoom::map_storageData[m][k].strid.compare(strResid) == 0)
						{
							if (JhStorageRoom::map_storageData[m][k].count >= buildrescount)
							{
								findcount++;
							}
						}
					}
				}
			}
			if (findcount == bulidressize)
			{
				if (JhNewerGuideLayer::checkifNewerGuide(55 + i) && Vec_Buildings[i]->data.level <= 0)
				{
					showNewerGuide(55 + i);
					break;
				}
			}
		}
	}
}
