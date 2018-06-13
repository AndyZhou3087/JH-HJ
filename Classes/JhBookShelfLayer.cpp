#include "JhBookShelfLayer.h"
#include <algorithm>
#include "json.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhSoundManager.h"
#include "JhStorageRoom.h"
#include "JhMyMenu.h"
#include "JhGlobalData.h"
#include "JhGameScene.h"
#include "JhBookDetailsLayer.h"

const std::string name[] = { "武功", "内功", "武器", "防具"};

JhBookShelfLayer::JhBookShelfLayer()
{
	lastCategoryindex = 0;
}


JhBookShelfLayer::~JhBookShelfLayer()
{

}

bool JhBookShelfLayer::init()
{

	m_csbnode = CSLoader::createNode("jhbookshelfLayer.csb");
	m_csbnode->setPosition(Vec2(0, -90));
	this->addChild(m_csbnode);


	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhBookShelfLayer::onBack, this));


	scrollview = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	Node* categoryBtnNode = m_csbnode->getChildByName("tagnode");
	for (int i = 0; i < categoryBtnNode->getChildrenCount(); i++)
	{
		std::string btnstr = StringUtils::format("btn%d", i + 1);
		cocos2d::ui::Button* btn = (cocos2d::ui::Button*)categoryBtnNode->getChildByName(btnstr);
		btn->setTag(i+1);
		btn->addTouchEventListener(CC_CALLBACK_2(JhBookShelfLayer::onCategory, this));
		vec_categoryBtn.push_back(btn);
	}
	vec_categoryBtn[0]->setBright(false);


	loadBookData();
	updateContent(1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

void JhBookShelfLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhBookShelfLayer::updateContent(int category)
{
	scrollview->removeAllChildrenWithCleanup(true);
	int textheigth = 0;
	int dataheigth = 0;
	int innerheight = scrollview->getInnerContainerSize().height;
	int atype[] = { W_GONG, N_GONG, WEAPON, PROTECT_EQU };
	int itemheight = 170;
	std::vector<int> vec_type;
	/*if (category == 0)
	{
		int size = sizeof(atype) / sizeof(atype[0]);
		for (int i = 0; i < size; i++)
		{
			vec_type.push_back(atype[i]);
			int count = getCountByType(atype[i]);
			if (count > 0)
			{
				int row = count % 4 == 0 ? count / 4 : (count / 4 + 1);
				typerow[i] = row;
				textheigth += 50;
			}
		}
	}
	else
	{*/
		vec_type.push_back(atype[category - 1]);
		int count = getCountByType(atype[category - 1]);
		if (count > 0)
		{
			int row = count % 4 == 0 ? count / 4 : (count / 4 + 1);
			typerow[0] = row;
		}
	//}

	for (unsigned int i = 0; i < vec_type.size(); i++)
	{
		dataheigth += typerow[i] * itemheight;
	}

	innerheight = textheigth + dataheigth;

	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, innerheight));

	for (int i = 0; i < 4; i++)
	{
		if (i + 1 == category)
		{
			vec_categoryBtn[i]->getChildByName("text")->setVisible(true);
		}
		else
		{
			vec_categoryBtn[i]->getChildByName("text")->setVisible(false);
		}
	}
	/*if (category == 0)
	{
		for (unsigned int i = 0; i < vec_type.size(); i++)
		{
			int icount = getCountByType(vec_type[i]);
			if (icount > 0)
			{
				int preheight = 0;
				for (unsigned int j = 0; j < i; j++)
				{
					if (getCountByType(vec_type[j])>0)
					{
						preheight += 50;
						preheight += typerow[j] * itemheight;
					}

				}
				Sprite * sepline = Sprite::createWithSpriteFrameName("ui/storagesepline.png");
				sepline->setPosition(Vec2(sepline->getContentSize().width / 2 + 40, innerheight - 40 - preheight));
				std::string sepname = StringUtils::format("sep%d", i);
				scrollview->addChild(sepline, 0, sepname);
				Label* namelbl = Label::createWithTTF(JhCommonFuncs::gbk2utf(name[i].c_str()), "fonts/STXINGKA.TTF", 22);
				namelbl->setColor(Color3B(0, 0, 0));
				namelbl->setPosition(Vec2(25, 25));
				sepline->addChild(namelbl);
			}
		}
	}*/
	for (unsigned int i = 0; i < vec_type.size(); i++)
	{
		for (unsigned int m = 0; m < map_bookData[vec_type[i]].size(); m++)
		{

			std::string boxstr = "";
			std::string datastrid = map_bookData[vec_type[i]][m].strid;
			if (vec_type[i] == WEAPON || vec_type[i] == PROTECT_EQU)
			{
				boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[datastrid].qu);
			}
			else if (vec_type[i] == N_GONG || vec_type[i] == W_GONG)
			{
				boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[datastrid].qu);
			}

			if (boxstr.length() > 0)
			{
				Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

				MenuItemSprite* boxItem = MenuItemSprite::create(
					box,
					box,
					box,
					CC_CALLBACK_1(JhBookShelfLayer::onclick, this));
				boxItem->setTag(0);
				boxItem->setUserData(&map_bookData[vec_type[i]][m]);
				std::string sepname = StringUtils::format("sep%d", i);
				Node* sepline = scrollview->getChildByName(sepname);
				if (sepline != NULL)
					boxItem->setPosition(Vec2(box->getContentSize().width / 2 + 50 + m % 4 * 140, sepline->getPositionY() - itemheight/2 - m / 4 * itemheight));
				else
					boxItem->setPosition(Vec2(box->getContentSize().width / 2 + 30 + m % 4 * 140, innerheight + 10 - itemheight/2 - m / 4 * itemheight));

				JhMyMenu* menu = JhMyMenu::create();
				menu->addChild(boxItem);
				menu->setTouchlimit(scrollview);
				menu->setPosition(Vec2(0, 0));

				scrollview->addChild(menu);

				std::string str = StringUtils::format("ui/%s.png", datastrid.c_str());
				Sprite * res = Sprite::createWithSpriteFrameName(str);
				res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
				box->addChild(res);

				Label *namelbl = Label::createWithTTF(JhGlobalData::map_allResource[datastrid].cname, "fonts/SIMHEI.TTF", 20);
				namelbl->enableOutline(Color4B(86, 86, 86, 255), 2);
				namelbl->setColor(Color3B(255, 255, 255));
				namelbl->setPosition(Vec2(box->getContentSize().width / 2, -17));
				box->addChild(namelbl);

	
				if (g_hero->getMeHas(datastrid) == NULL)
				{
					std::string mixid = JhGlobalData::getMixGF();

					bool isinmixSec = false;
					if (mixid.length() > 0)
					{
						for (unsigned int n = 0; n < JhGlobalData::map_MixGfData[mixid].vec_secgf.size(); n++)
						{
							if (datastrid.compare(JhGlobalData::map_MixGfData[mixid].vec_secgf[n]) == 0)
							{
								isinmixSec = true;
								break;
							}
						}
					}
					if (!isinmixSec)
					{
						JhCommonFuncs::changeGray(box);
						JhCommonFuncs::changeGray(res);
						namelbl->setOpacity(180);
					}
				}
			}
		}
	}
}

void JhBookShelfLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhBookShelfLayer::onclick(Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	BookData* bdata = (BookData*)node->getUserData();
	this->addChild(JhBookDetailsLayer::create(bdata));
}

int JhBookShelfLayer::getCountByType(int type)
{
	int count = 0;
	return map_bookData[type].size();
}

void JhBookShelfLayer::loadBookData()
{
	std::map<std::string, WG_NGData>::iterator it;
	for (it = JhGlobalData::map_wgngs.begin(); it != JhGlobalData::map_wgngs.end(); ++it)
	{
		WG_NGData gfdata = JhGlobalData::map_wgngs[it->first];
		std::string strid = gfdata.id;
		BookData bdata;
		bdata.strid = strid;
		bdata.type = gfdata.type - 1;
		bdata.maxvalue = gfdata.vec_bns[gfdata.maxlv - 1];
		map_bookData[gfdata.type - 1].push_back(bdata);
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = JhGlobalData::map_equips[ite->first];
		BookData bdata;
		bdata.strid = edata.id;
		int type = edata.type - 1;
		if (type == WEAPON)
			bdata.maxvalue = edata.atk;
		else
			bdata.maxvalue = edata.df;
		bdata.type = type;
		map_bookData[type].push_back(bdata);
	}

	std::map<int, std::vector<BookData>>::iterator bit;
	for (bit = map_bookData.begin(); bit != map_bookData.end(); ++bit)
	{
		sort(map_bookData[bit->first].begin(), map_bookData[bit->first].end(), larger_callback);
	}

}

bool JhBookShelfLayer::larger_callback(BookData a, BookData b)
{
	if (a.maxvalue > b.maxvalue)
		return true;
	else
		return false;
}


void JhBookShelfLayer::onCategory(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		if (lastCategoryindex == node->getTag())
			return;

		lastCategoryindex = node->getTag();

		for (unsigned int i = 0; i < vec_categoryBtn.size(); i++)
		{
			if (node->getTag() != i + 1)
			{
				vec_categoryBtn[i]->setBright(true);
			}
			else
			{
				vec_categoryBtn[i]->setBright(false);
			}
		}
		updateContent(node->getTag());
	}
}