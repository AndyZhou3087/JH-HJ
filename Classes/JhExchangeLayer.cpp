#include "JhExchangeLayer.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhMyPackage.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhStorageRoom.h"
#include "JhSoundManager.h"
#include "JhNpcLayer.h"
#include "JhHintBox.h"
#include "JhMyMenu.h"
#include "JhResDetailsLayer.h"

JhExchangeLayer::JhExchangeLayer()
{
	lastMyGoodsSrollViewHeight = -1;
	lastNpcGoodsSrollViewHeight = -1;
	isExgOk = false;
	m_isLongPress = false;
	m_longTouchNode = NULL;
	clickwhere = 0;
	isWxbExg = false;
	isClickMyGoods = false;
}


JhExchangeLayer::~JhExchangeLayer()
{

}


JhExchangeLayer* JhExchangeLayer::create(std::string npcid)
{
	JhExchangeLayer *pRet = new JhExchangeLayer();
	if (pRet && pRet->init(npcid))
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

bool JhExchangeLayer::init(std::string npcid)
{

	Node* csbnode = CSLoader::createNode("jhexgLayer.csb");
	this->addChild(csbnode);

	m_npcid = npcid;

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhExchangeLayer::onBack, this));

	m_exgbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("exgbtn");
	m_exgbtn->addTouchEventListener(CC_CALLBACK_2(JhExchangeLayer::onExg, this));
	m_exgbtn->setEnabled(false);

	cocos2d::ui::ImageView* npchead = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcicon");
	std::string npcheadstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npchead->loadTexture(npcheadstr, cocos2d::ui::TextureResType::PLIST);
	npchead->setScale(0.6f);

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcname->setString(JhGlobalData::map_npcs[npcid].name);

	cocos2d::ui::Text* npcgoodstext = (cocos2d::ui::Text*)csbnode->getChildByName("npcgoodstext");
	std::string goodstextstr = StringUtils::format("%s%s", JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("的物品").c_str());
	npcgoodstext->setString(goodstextstr);

	m_npcWordLbl = (cocos2d::ui::Text*)csbnode->getChildByName("npcword");
	m_desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	m_npcGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("npcexgscroll");
	m_npcGoodsSrollView->setScrollBarEnabled(false);
	m_myGoodsSrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("myexgscroll");
	m_myGoodsSrollView->setScrollBarEnabled(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	this->scheduleOnce(schedule_selector(JhExchangeLayer::delayShowExgData), 0.2f);

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhExchangeLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhExchangeLayer::delayShowExgData(float dt)
{
	std::vector<std::string> exchgres;
	if (m_npcid.compare("n012") == 0)//韦小宝
	{
		std::string cfgstr = JhGlobalData::getExgCfgData();

		bool isrand = true;
		int liveday = g_nature->getPastDays();
		if (cfgstr.length() > 0)
		{
			std::vector<std::string> tmp;
			JhCommonFuncs::split(cfgstr, tmp, "-");

			if (liveday == atoi(tmp[0].c_str()))
			{
				isrand = false;
				for (int m = 1; m <= 3; m++)
					exchgres.push_back(tmp[m]);
			}
		}

		if (isrand)
		{
			m_desc->setVisible(true);
			randExchgRes(exchgres);
		}

		std::string savestr = StringUtils::format("%d-", liveday);

		for (unsigned int n = 0; n < exchgres.size(); n++)
		{
			savestr.append(exchgres[n]);
			savestr.append("-");
		}
		savestr = savestr.substr(0, savestr.length() - 1);
		JhGlobalData::setExgCfgData(savestr);
	}
	else
	{
		exchgres = JhGlobalData::map_npcs[m_npcid].exchgres;
	}

	for (unsigned int i = 0; i < exchgres.size(); i++)
	{
		int res = atoi(exchgres[i].c_str());

		if (res != 0)
		{
			PackageData data;
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
			npcGoodsData.push_back(data);
		}
		else
		{
			PackageData data;
			std::string strid = exchgres[i];
			data.strid = strid;
			data.count = 1;

			bool isfind = false;
			std::map<std::string, WG_NGData>::iterator it;
			for (it = JhGlobalData::map_wgngs.begin(); it != JhGlobalData::map_wgngs.end(); ++it)
			{
				WG_NGData gfdata = JhGlobalData::map_wgngs[it->first];
				if (exchgres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF_Equip(exchgres[i]) && JhGlobalData::tempHasGf_Equip(exchgres[i]).length() <= 0)
				{
					isfind = true;
					data.strid = gfdata.id;
					data.count = 1;
					data.type = gfdata.type - 1;
					data.extype = gfdata.extype;
					npcGoodsData.push_back(data);
					break;
				}
			}

			if (!isfind)
			{
				std::map<std::string, EquipData>::iterator ite;
				for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
				{
					EquipData edata = JhGlobalData::map_equips[ite->first];
					if (exchgres[i].compare(edata.id) == 0)
					{
						data.strid = edata.id;
						data.count = 1;
						data.type = edata.type - 1;
						data.extype = edata.extype;
						npcGoodsData.push_back(data);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		myGoodsData.push_back(JhMyPackage::vec_packages[i]);
	}
	m_desc->setVisible(false);
	updata();

}

void JhExchangeLayer::onNpcGoodsItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	isClickMyGoods = false;
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_longTouchNode = node;
		schedule(schedule_selector(JhExchangeLayer::longTouchUpdate), 1.0f);
		clickwhere = 4;
	}

	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		unschedule(schedule_selector(JhExchangeLayer::longTouchUpdate));
		if (!m_isLongPress)
		{
			PackageData* data = (PackageData*)node->getUserData();
			giveHero(data->strid);
			updata();
		}
		m_isLongPress = false;
		clickwhere = 0;
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		unschedule(schedule_selector(JhExchangeLayer::longTouchUpdate));
		m_isLongPress = false;
		clickwhere = 0;
	}
}

void JhExchangeLayer::updateMyGoods(PackageData data, int type)
{
	if (type == 0)
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
	else
	{
		unsigned int i = 0;
		for (i = 0; i < npcExgData.size(); i++)
		{
			if (data.strid.compare(npcExgData[i].strid) == 0 && (npcExgData[i].type == FOOD || npcExgData[i].type == MEDICINAL || npcExgData[i].type == RES_1 || npcExgData[i].type == RES_2))
			{
				if (npcExgData[i].count < 10)
				{
					npcExgData[i].count++;
					break;
				}
				else
					continue;
			}

		}
		if (i == npcExgData.size())
		{
			data.count = 1;
			npcExgData.push_back(data);
		}
	}

}

void JhExchangeLayer::updateNpcGoods(PackageData data, int type)
{
	if (type == 1)
	{
		unsigned int i = 0;
		for (i = 0; i < npcGoodsData.size(); i++)
		{
			if (data.strid.compare(npcGoodsData[i].strid) == 0 && (npcGoodsData[i].type == FOOD || npcGoodsData[i].type == MEDICINAL || npcGoodsData[i].type == RES_1 || npcGoodsData[i].type == RES_2))
			{
				npcGoodsData[i].count++;
				break;
			}
		}
		if (i == npcGoodsData.size())
		{
			data.count = 1;
			npcGoodsData.push_back(data);
		}
	}
	else
	{
		unsigned int i = 0;
		for (i = 0; i < myExgData.size(); i++)
		{
			if (data.strid.compare(myExgData[i].strid) == 0 && (myExgData[i].type == FOOD || myExgData[i].type == MEDICINAL || myExgData[i].type == RES_1 || myExgData[i].type == RES_2))
			{
				myExgData[i].count++;
				break;
			}
		}
		if (i == myExgData.size())
		{
			data.count = 1;
			myExgData.push_back(data);
		}
	}
}

void JhExchangeLayer::onMyGoodsItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{

	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		m_longTouchNode = node;
		schedule(schedule_selector(JhExchangeLayer::longTouchUpdate), 1.0f);
		clickwhere = 3;
		int gsize = myGoodsData.size();
		if (node->getTag() < gsize)
		{
			isClickMyGoods = true;
		}
		else
		{
			isClickMyGoods = false;
		}
	}

	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		unschedule(schedule_selector(JhExchangeLayer::longTouchUpdate));
		if (!m_isLongPress)
		{
			PackageData* data = (PackageData*)node->getUserData();
			giveNpc(data->strid);
			updata();
		}
		m_isLongPress = false;
		clickwhere = 0;
	}
	else if (type == ui::Widget::TouchEventType::CANCELED)
	{
		unschedule(schedule_selector(JhExchangeLayer::longTouchUpdate));
		m_isLongPress = false;
		clickwhere = 0;
	}
}

void JhExchangeLayer::giveNpc(std::string strid)
{
	
	PackageData* data = NULL;
	int size0 = myGoodsData.size();
	bool ismine = true;
	int datatag = 0;

	if (m_isLongPress)
	{

		if (m_longTouchNode != NULL)
		{
			datatag = m_longTouchNode->getTag();
			if (datatag >= size0)
				ismine = false;
		}

		if (ismine)
		{
			for (unsigned int i = 0; i < myGoodsData.size(); i++)
			{
				if (myGoodsData[i].strid.compare(strid) == 0)
				{
					datatag = i;
					data = &myGoodsData[i];
					break;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < npcExgData.size(); i++)
			{
				if (npcExgData[i].strid.compare(strid) == 0)
				{
					datatag = i + size0;
					data = &npcExgData[i];
					break;
				}
			}
		}
	}
	else
	{
		if (m_longTouchNode != NULL)
		{
			datatag = m_longTouchNode->getTag();
			data = (PackageData*)m_longTouchNode->getUserData();
			if (datatag >= size0)
				ismine = false;
		}
	}

	//韦小宝只要银子
	if (ismine && m_npcid.compare("n012") == 0 && strid.compare("80") != 0)
	{
		isWxbExg = true;
		return;
	}

	if (data != NULL && data->strid.length() > 0)
	{
		PackageData pdata = *data;

		if (data->count - 1 <= 0)
		{
			if (datatag >= size0)
			{
				if (npcExgData.size() > 0)
				{
					npcExgData.erase(npcExgData.begin() + (datatag - size0));
					updateNpcGoods(pdata, 1);
				}
			}
			else
			{
				if (myGoodsData.size() > 0)
				{
					myGoodsData.erase(myGoodsData.begin() + datatag);
					updateNpcGoods(pdata, 0);
				}
			}
		}
		else
		{
			if (datatag >= size0)
			{
				updateNpcGoods(pdata, 1);
			}
			else
			{
				updateNpcGoods(pdata, 0);
			}
			data->count--;
		}
	}
}

void JhExchangeLayer::giveHero(std::string strid)
{
	int size0 = npcGoodsData.size();

	int datatag = 0;
	PackageData* data;
	if (m_longTouchNode != NULL)
	{
		datatag = m_longTouchNode->getTag();
		data = (PackageData*)m_longTouchNode->getUserData();
	}
	PackageData pdata = *data;

	if (data != NULL && data->strid.length() > 0)
	{
		if (data->count - 1 <= 0)
		{
			if (datatag >= size0)
			{
				if (myExgData.size() > 0)
				{
					myExgData.erase(myExgData.begin() + (datatag - size0));
					updateMyGoods(pdata, 0);
				}
			}
			else
			{
				if (npcGoodsData.size() > 0)
				{
					npcGoodsData.erase(npcGoodsData.begin() + datatag);
					updateMyGoods(pdata, 1);
				}
			}

		}
		else
		{
			if (datatag >= size0)
			{
				updateMyGoods(pdata, 0);
			}
			else
			{
				updateMyGoods(pdata, 1);
			}
			data->count--;
		}
	}
}

int JhExchangeLayer::getCountByResId(std::string strid, int inwhere)
{
	int count = 0;

	if (inwhere == 0)
	{
		bool ismine = true;
		int size0 = myGoodsData.size();
		if (m_longTouchNode != NULL)
		{
			int tag = m_longTouchNode->getTag();
			if (tag >= size0)
			{
				ismine = false;
			}
		}

		if (ismine)
		{
			for (unsigned int i = 0; i < myGoodsData.size(); i++)
			{
				if (myGoodsData[i].strid.compare(strid) == 0)
				{
					count += myGoodsData[i].count;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < npcExgData.size(); i++)
			{
				if (npcExgData[i].strid.compare(strid) == 0)
				{
					count += npcExgData[i].count;
				}
			}
		}
	}
	else if (inwhere == 1)
	{
		bool isnpc = true;
		int size0 = npcGoodsData.size();
		if (m_longTouchNode != NULL)
		{
			int tag = m_longTouchNode->getTag();
			if (tag >= size0)
			{
				isnpc = false;
			}
		}

		if (isnpc)
		{
			for (unsigned int i = 0; i < npcGoodsData.size(); i++)
			{
				if (npcGoodsData[i].strid.compare(strid) == 0)
				{
					count += npcGoodsData[i].count;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < myExgData.size(); i++)
			{
				if (myExgData[i].strid.compare(strid) == 0)
				{
					count += myExgData[i].count;
				}
			}
		}
	}
	return count;
}

void JhExchangeLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhExchangeLayer::onExg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (isExgOk)
		{
			m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("下次再来吧！！"));
			m_npcWordLbl->setTextColor(Color4B(0, 0, 0, 255));
			return;
		}
		int size0 = myGoodsData.size();
		int size1 = npcExgData.size();
		int size = size0 + size1;
		if (size > 5)
		{
			JhHintBox* hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("背包放不下了哦！！"));
			this->addChild(hintbox);
		}
		else
		{
			JhMyPackage::vec_packages.clear();

			for (int i = 0; i < size0; i++)
			{
				JhMyPackage::vec_packages.push_back(myGoodsData[i]);
			}
			for (int i = 0; i < size1; i++)
			{
				JhMyPackage::vec_packages.push_back(npcExgData[i]);
			}
			JhMyPackage::save();
			isExgOk = true;
			this->removeFromParentAndCleanup(true);
		}
	}
}
void JhExchangeLayer::checkValue()
{
	if (isWxbExg)
	{
		m_npcWordLbl->setVisible(true);
		m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("拿走这些破东西，记住小宝我只要银子！！"));					
		m_npcWordLbl->setTextColor(Color4B(204, 4, 4, 255));
		isWxbExg = false;
		return;
	}

	int myval = 0;
	int npcval = 0;

	for (unsigned int i = 0; i < myExgData.size(); i++)
	{
		std::string resid = myExgData[i].strid;
		std::vector<std::string> vec_npcs = JhGlobalData::map_allResource[resid].npc;
		int val = JhGlobalData::map_allResource[resid].val * myExgData[i].count;
		for (unsigned int j = 0; j < vec_npcs.size(); j++)
		{
			if (m_npcid.compare(vec_npcs[j]) == 0)
			{
				float fval = val *  JhGlobalData::map_allResource[resid].npcval[j];
				val = int(fval + 0.5);
				break;
			}
		}
		myval += val;
	}

	for (unsigned int i = 0; i < npcExgData.size(); i++)
	{
		std::string resid = npcExgData[i].strid;
		int val = JhGlobalData::map_allResource[resid].val * npcExgData[i].count;
		npcval += val;
	}

	if (npcExgData.size() <= 0 && myExgData.size() <= 0)
	{
		m_npcWordLbl->setVisible(false);
	}
	else
	{
		m_npcWordLbl->setVisible(true);
		if (npcval > myval)
		{
			m_exgbtn->setEnabled(false);
			if (myval > npcval * 2 / 3)
			{
				m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("还差了一点啊，再加点吧！"));
				m_npcWordLbl->setTextColor(Color4B(204, 4, 4, 255));
			}
			else if (myval <= 0)
			{
				m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("你在跟我开玩笑？想白拿？"));
				m_npcWordLbl->setTextColor(Color4B(204, 4, 4, 255));
			}
			else
			{
				m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("开什么玩笑，这点破东西也想跟我换？走开走开！"));
				m_npcWordLbl->setTextColor(Color4B(204, 4, 4, 255));
			}
		}
		else
		{
			m_exgbtn->setEnabled(true);
			if ((myval - npcval) > npcval / 3)
			{
				m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("痛快！痛快！果然是个痛快人！"));
				m_npcWordLbl->setTextColor(Color4B(27, 141, 0, 255));
			}
			else
			{
				m_npcWordLbl->setString(JhCommonFuncs::gbk2utf("这是个公平的交易"));
				m_npcWordLbl->setTextColor(Color4B(0, 0, 0, 180));
			}
		}
	}
}

void JhExchangeLayer::updata()
{
	updataMyGoodsUI();
	updataNpcGoodsUI();
	checkValue();
}

void JhExchangeLayer::updataMyGoodsUI()
{
	int size0 = myGoodsData.size();
	int size1 = npcExgData.size();
	int size = size0 + size1;

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
	for (unsigned int i = 0; i < npcExgData.size(); i++)
	{
		allMydata.push_back(&npcExgData[i]);
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

		std::string name = StringUtils::format("pitem%d", i);
		cocos2d::ui::ImageView* boxItem = cocos2d::ui::ImageView::create(boxstr, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addTouchEventListener(CC_CALLBACK_2(JhExchangeLayer::onMyGoodsItem, this));
		boxItem->setTouchEnabled(true);
		boxItem->setTag(i);
		m_myGoodsSrollView->addChild(boxItem, 0, name);
		boxItem->setUserData(allMydata[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());

		cocos2d::ui::ImageView* res = cocos2d::ui::ImageView::create(str, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addChild(res, 0, "res");
		res->setPosition(Vec2(boxItem->getContentSize().width / 2, boxItem->getContentSize().height / 2));

		str = StringUtils::format("%d", tmpdata->count);

		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);
		boxItem->addChild(reslbl, 0, "lbl");
		reslbl->setPosition(Vec2(boxItem->getContentSize().width - 25, 25));

		if (i >= size0)
		{
			Sprite * whos = Sprite::createWithSpriteFrameName("ui/atrselected.png");
			whos->setPosition(Vec2(boxItem->getContentSize().width - whos->getContentSize().width / 2, boxItem->getContentSize().height - whos->getContentSize().height / 2));
			boxItem->addChild(whos);
		}
	}
}

void JhExchangeLayer::updataNpcGoodsUI()
{
	int size0 = npcGoodsData.size();
	int size1 = myExgData.size();
	int size = size0 + size1;

	m_npcGoodsSrollView->removeAllChildrenWithCleanup(true);

	int row = size % 5 == 0 ? size / 5 : (size / 5 + 1);

	int innerheight = m_npcGoodsSrollView->getInnerContainerSize().height;
	if (lastNpcGoodsSrollViewHeight < 0)
	{
		innerheight = row * 140;
		int contentheight = m_npcGoodsSrollView->getContentSize().height;
		if (innerheight < contentheight)
			innerheight = contentheight;
		lastNpcGoodsSrollViewHeight = innerheight;
		m_npcGoodsSrollView->setInnerContainerSize(Size(m_npcGoodsSrollView->getContentSize().width, innerheight));
	}

	std::vector<PackageData*> allNpcdata;
	for (unsigned int i = 0; i < npcGoodsData.size(); i++)
	{
		allNpcdata.push_back(&npcGoodsData[i]);
	}
	for (unsigned int i = 0; i < myExgData.size(); i++)
	{
		allNpcdata.push_back(&myExgData[i]);
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

		std::string name = StringUtils::format("pitem%d", i);
		cocos2d::ui::ImageView* boxItem = cocos2d::ui::ImageView::create(boxstr, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addTouchEventListener(CC_CALLBACK_2(JhExchangeLayer::onNpcGoodsItem, this));
		boxItem->setTouchEnabled(true);
		boxItem->setTag(i);
		m_npcGoodsSrollView->addChild(boxItem, 0, name);
		boxItem->setUserData(allNpcdata[i]);
		boxItem->setPosition(Vec2(boxItem->getContentSize().width / 2 + 10 + i % 5 * 125, innerheight - boxItem->getContentSize().height / 2 - i / 5 * 140));

		std::string str = StringUtils::format("ui/%s.png", tmpdata->strid.c_str());

		cocos2d::ui::ImageView* res = cocos2d::ui::ImageView::create(str, cocos2d::ui::Widget::TextureResType::PLIST);
		boxItem->addChild(res, 0, "res");
		res->setPosition(Vec2(boxItem->getContentSize().width / 2, boxItem->getContentSize().height / 2));

		str = StringUtils::format("%d", tmpdata->count);

		Label * reslbl = Label::createWithTTF(str, "fonts/STXINGKA.TTF", 18);
		boxItem->addChild(reslbl, 0, "lbl");
		reslbl->setPosition(Vec2(boxItem->getContentSize().width - 25, 25));


		if (i >= size0)
		{
			Sprite * whos = Sprite::createWithSpriteFrameName("ui/settingselect.png");
			whos->setPosition(Vec2(boxItem->getContentSize().width - whos->getContentSize().width / 2, boxItem->getContentSize().height - whos->getContentSize().height / 2));
			boxItem->addChild(whos);
		}
	}
}

void JhExchangeLayer::onExit()
{
	Layer::onExit();
}

void JhExchangeLayer::randExchgRes(std::vector<std::string> &vec_exchgres)
{

	int dqu = 5;
	if (g_hero->getLVValue() < 50)
	{
		dqu = 3;
	}
	else if (g_hero->getLVValue() < 80)
	{
		dqu = 4;
	}

	std::map<int,std::vector<std::string>> map_temp;
	std::map<std::string, WG_NGData>::iterator it;
	for (it = JhGlobalData::map_wgngs.begin(); it != JhGlobalData::map_wgngs.end(); ++it)
	{
		WG_NGData gfdata = JhGlobalData::map_wgngs[it->first];
		if (!g_hero->checkifHasGF_Equip(gfdata.id))
		{
			if (gfdata.qu <= dqu)
				map_temp[gfdata.qu].push_back(gfdata.id);
		}
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = JhGlobalData::map_equips[ite->first];
		if (!g_hero->checkifHasGF_Equip(edata.id))
		{
			if (edata.qu <= dqu)
				map_temp[edata.qu].push_back(edata.id);
		}
	}
	std::map<int, std::vector<std::string>> map_res;
	std::map<int, std::vector<std::string>>::iterator rit;
	for (rit = map_temp.begin(); rit != map_temp.end(); ++rit)
	{
		for (unsigned int i = 0; i < map_temp[rit->first].size(); i++)
		{
			std::string tmpstr = map_temp[rit->first][i];
			if (JhGlobalData::tempHasGf_Equip(tmpstr).length() <= 0)
				map_res[rit->first].push_back(tmpstr);
		}
	}

	std::vector<int> vec_resid;
	for (int i = 1; i <= 23; i++)
	{
		vec_resid.push_back(i);
	}
	vec_resid.push_back(80);

	std::vector<std::string> vec_res;
	for (unsigned int i = 0; i < vec_resid.size(); i++)
	{
		std::string str = StringUtils::format("%d", vec_resid[i] * 1000 + 10);
		map_res[1].push_back(str);
		vec_res.push_back(str);
	}

	int randnum[] = {40,70,85,95,100};


	for (int i = 0; i < 3; i++)
	{
		int r = JhGlobalData::createRandomNum(100);
		int m = 0;
		for (m = 0; m < 5; m++)
		{
			if (r < randnum[m])
			{
				int size = map_res[m + 1].size();
				if (size > 0)
				{
					int r1 = JhGlobalData::createRandomNum(size);
					vec_exchgres.push_back(map_res[m + 1][r1]);
					map_res[m + 1].erase(map_res[m + 1].begin() + r1);
				}
				else
				{
					int size0 = map_res[1].size();
					int r1 = JhGlobalData::createRandomNum(size0);
					vec_exchgres.push_back(map_res[1][r1]);
				}
				break;
			}

		}
	}
}

void JhExchangeLayer::longTouchUpdate(float delay){
	m_isLongPress = true;
	if (m_longTouchNode != NULL){

		PackageData* data = (PackageData*)m_longTouchNode->getUserData();
		if (isClickMyGoods)
		{
			for (unsigned int i = 0; i < myGoodsData.size(); i++)
			{
				if (data->strid.compare(myGoodsData[i].strid) == 0)
				{
					m_longTouchNode->setUserData(&myGoodsData[i]);
					m_longTouchNode->setTag(i);
					break;
				}
			}
		}

		unschedule(schedule_selector(JhExchangeLayer::longTouchUpdate));
		JhResDetailsLayer::whereClick = clickwhere;

		this->addChild(JhResDetailsLayer::create(data));
	}
}