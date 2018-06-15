#include "JhStorageUILayer.h"
#include "json.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhConst.h"
#include "JhResDetailsLayer.h"
#include "JhSoundManager.h"
#include "JhShopLayer.h"
#include "JhMyMenu.h"
#include "JhNewerGuideLayer.h"
#include "JhHelpMainLayer.h"

const std::string name[] = { "【食物】", "【药材】", "【武器】", "【防具】", "【内功】", "【武功】", "【资源】", "【工具】", "【其他】"};

JhStorageUILayer::JhStorageUILayer()
{

}


JhStorageUILayer::~JhStorageUILayer()
{

}

bool JhStorageUILayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhbuidingUiLayer.csb");
	//m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);

	cocos2d::ui::Text* title = (cocos2d::ui::Text*)m_csbnode->getChildByName("title");
	title->setString(JhCommonFuncs::gbk2utf("仓库"));

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhStorageUILayer::onBack, this));

	cocos2d::ui::Widget* shopbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("shopbtn");
	shopbtn->setVisible(true);
	shopbtn->addTouchEventListener(CC_CALLBACK_2(JhStorageUILayer::onShop, this));

	cocos2d::ui::Widget* helpbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("helpbtn");
	helpbtn->setVisible(true);
	helpbtn->addTouchEventListener(CC_CALLBACK_2(JhStorageUILayer::onHelp, this));

	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	scrollview->setContentSize(Size(600, 750));
	scrollview->setPositionY(155);
	
	updateResContent();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	showNewerGuide(46);
	return true;
}

void JhStorageUILayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhStorageUILayer::updateResContent()
{
	scrollview->removeAllChildren();
	int textheigth = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		int count = getCountByType(i);
		if (count > 0)
		{
			int row = count % 4 == 0 ? count / 4 : (count / 4 + 1);
			typerow[i] = row;
			textheigth += 45;
		}
		else
		{
			typerow[i] = 0;
		}
	}

	int innerheight = scrollview->getInnerContainerSize().height;

	int dataheigth = 0;
	for (int i = 0; i < RES_MAX; i++)
	{
		dataheigth += typerow[i] * 130;
	}
	innerheight = textheigth + dataheigth;

	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(600, innerheight));


	for (int i = 0; i < RES_MAX; i++)
	{
		int icount = getCountByType(i);
		if (icount > 0)
		{
			int preheight = 0;
			for (int j = 0; j < i; j++)
			{
				if (getCountByType(j)>0)
				{
					preheight += 45;
					preheight += typerow[j] * 130;
				}

			}

			int mcountindex = -1;
			for (unsigned int m = 0; m < JhStorageRoom::map_storageData[i].size(); m++)
			{
				PackageData tmpdata = JhStorageRoom::map_storageData[i][m];
				std::map<std::string, AllResource>::iterator resit;
				for (resit = JhGlobalData::map_allResource.begin(); resit != JhGlobalData::map_allResource.end(); resit++)
				{
					if (tmpdata.strid.compare(JhGlobalData::map_allResource[resit->first].strid) == 0)
					{
						mcountindex++;
						break;
					}
				}
			}

			Sprite * sepline = Sprite::createWithSpriteFrameName("ui/storagesepline.png");
			sepline->setPosition(Vec2(sepline->getContentSize().width / 2 + 40, innerheight - 168 - preheight - mcountindex / 4 * 130));
			scrollview->addChild(sepline);
			Label* namelbl = Label::createWithTTF(JhCommonFuncs::gbk2utf( name[i].c_str()), "fonts/STXINGKA.TTF", 25);
			namelbl->setColor(Color3B(80, 86, 68));
			namelbl->setPosition(Vec2(80, innerheight - 10 - preheight));
			scrollview->addChild(namelbl);

			int countindex = -1;
			for (unsigned int m = 0; m < JhStorageRoom::map_storageData[i].size(); m++)
			{
				std::string boxstr = "ui/buildsmall.png";
				PackageData tmpdata = JhStorageRoom::map_storageData[i][m];

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
				
				Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
				//box->setPosition(Vec2(box->getContentSize().width/2 + 20 + m % 5 * 120, sepline->getPositionY() - 5 - 65 - m/5*130));
				//scrollview->addChild(box);

				MenuItemSprite* boxItem = MenuItemSprite::create(
					box,
					box,
					box,
					CC_CALLBACK_1(JhStorageUILayer::onclick, this));
				boxItem->setTag(0);
				boxItem->setUserData(&JhStorageRoom::map_storageData[i].at(m));
				boxItem->setPosition(Vec2(box->getContentSize().width / 2 + 20 + countindex % 4 * 140, sepline->getPositionY() + 70 + countindex / 4 * 130));
				JhMyMenu* menu = JhMyMenu::create();
				menu->addChild(boxItem);
				menu->setTouchlimit(scrollview);
				menu->setPosition(Vec2(0, 0));
				std::string namestr = StringUtils::format("resitem%d", i * 100 + countindex);
				scrollview->addChild(menu, 0, namestr);

				std::string str = StringUtils::format("ui/%s.png", JhStorageRoom::map_storageData[i].at(m).strid.c_str());
				Sprite * res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
				box->addChild(res);


				Label * reslbl = Label::createWithTTF(countorlvstr, "fonts/SIMHEI.TTF", 22);//Label::createWithSystemFont(str, "", 18);
				reslbl->setAnchorPoint(Vec2(1, 0.5));
				reslbl->enableOutline(Color4B(133, 90, 39, 255), 2);
				reslbl->setPosition(Vec2(box->getContentSize().width - 10, 20));
				box->addChild(reslbl);

				std::string mymixgf = JhGlobalData::getMixGF();
				MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
				if (mymixgf.length() > 0)
				{
					if (JhStorageRoom::map_storageData[i].at(m).strid.compare(mdata.mastergf) == 0)
					{
						Sprite * mixtag = Sprite::createWithSpriteFrameName("ui/mixtag.png");
						mixtag->setPosition(Vec2(box->getContentSize().width - 15, box->getContentSize().height - 15));
						box->addChild(mixtag);
					}
				}
			}
		}
	}
}

void JhStorageUILayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhStorageUILayer::onclick(Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	JhResDetailsLayer* layer = JhResDetailsLayer::create(data);
	this->addChild(layer);

}

int JhStorageUILayer::getCountByType(int type)
{
	//int test[] = { 5, 0, 6, 2, 0, 0, 7, 3 };
	//return test[type];
	return JhStorageRoom::map_storageData[type].size();
}

void JhStorageUILayer::onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->addChild(JhShopLayer::create());
	}
}

void JhStorageUILayer::onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->addChild(JhHelpMainLayer::create());
	}
}

void JhStorageUILayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 46)
	{
		Node* itemnode = scrollview->getChildByName("resitem0");
		if (itemnode != NULL)
			nodes.push_back(itemnode->getChildren().at(0));
	}
	else if (step == 47)
	{
		Node* itemnode = scrollview->getChildByName("resitem100");
		if (itemnode != NULL)
			nodes.push_back(itemnode->getChildren().at(0));
	}
	if (nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}
