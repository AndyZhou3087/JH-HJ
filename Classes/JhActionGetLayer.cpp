#include "JhActionGetLayer.h"
#include "json.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhMyPackage.h"
#include "JhConst.h"
#include "JhGameDataSave.h"
#include "JhGameScene.h"
#include "JhSoundManager.h"
#include "JhNewerGuideLayer.h"
#include "JhHomeHill.h"

JhActionGetLayer::JhActionGetLayer()
{
	repeatCount = 0;
}


JhActionGetLayer::~JhActionGetLayer()
{
}

bool JhActionGetLayer::init(int rid, std::vector<int> res_ids, int type, int actype)
{
	Node* csbnode = CSLoader::createNode("jhactionGetLayer.csb");
	this->addChild(csbnode);

	mtype = type;
	mrid = rid;
	m_actype = actype;

	//操作的文字显示控件1："采集", 2："砍伐", 3："挖掘"
	cocos2d::ui::Text* actiontext = (cocos2d::ui::Text*)csbnode->getChildByName("actiontext");
	actiontext->setString(acname[actype]);

	//返回按钮
	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(JhActionGetLayer::onBack, this));

	//1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮
	m_getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	m_getbtn->addTouchEventListener(CC_CALLBACK_2(JhActionGetLayer::onGet, this));
	std::string str = StringUtils::format("继续%s", acname[actype].c_str());
	m_getbtn->setTitleText(JhCommonFuncs::gbk2utf(str.c_str()));

	//全部拾取按钮
	m_getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	m_getallbtn->addTouchEventListener(CC_CALLBACK_2(JhActionGetLayer::onAllGet, this));

	setBtnStatus(false);

	rewardids = res_ids;

	//临时存放数据保持到本地文件
	loadTempData();

	//加载背包数据
	updataMyPackageUI();

	//显示操作文字说明
	addEventText();

	//点击后山列表中的操作获取一次资源
	this->scheduleOnce(schedule_selector(JhActionGetLayer::delayDoAction), 0.2f);
	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhActionGetLayer::doAction(float dt)
{
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}

	bool isget = false;
	for (unsigned int i = 0; i < rewardids.size(); i++)
	{
		unsigned int m = 0;
		for (m = 0; m < getResData.size(); m++)
		{
			//奖励栏中数据跟产出的ID比较，相同数量+1
			if (rewardids[i] == atoi(getResData[m].strid.c_str()))
			{
				getResData[m].count++;
				isget = true;
				break;
			}
		}
		//奖励栏中数据跟产出的ID比较，产出ID不在奖励栏内加到奖励栏中
		if (m == getResData.size())
		{
			PackageData data;
			std::string idstr = StringUtils::format("%d", rewardids[i]);
			data.strid = idstr;
			data.type = mtype - 1;
			data.count = 1;
			getResData.push_back(data);
			isget = true;
		}
	}
	if (isget)
		repeatCount++;

	if (repeatCount > 2)
	{
		int r = JhGlobalData::createRandomNum(100);
		if (r < 50)
			g_hero->setSpiritValue(g_hero->getSpiritValue() - 0.5f);
	}
	if (g_hero->getSpiritValue() < 0.0f)
		g_hero->setSpiritValue(0.0f);

	updataRewardUI();

	setBtnStatus(true);

	if (JhGlobalData::vec_resData[mrid].count <= 0)
	{
		m_getbtn->setEnabled(false);
	}

	if (JhNewerGuideLayer::checkifNewerGuide(24))
	{
		showNewerGuide(24);
	}
	else if (JhNewerGuideLayer::checkifNewerGuide(26))
	{
		showNewerGuide(26);
	}
	else if (JhNewerGuideLayer::checkifNewerGuide(29))
	{
		showNewerGuide(29);
	}
	else if (JhNewerGuideLayer::checkifNewerGuide(31))
	{
		showNewerGuide(31);
	}
}

void JhActionGetLayer::onRewardItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	//点击奖励栏的资源
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int size = getResData.size();

	int count = data->count - 1;
	if (count <= 0)//数量为0，全部加到背包中了，移除掉
	{
		std::vector<PackageData>::iterator it;
		for (it = getResData.begin(); it != getResData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata = *data;
				pdata.count = 1;

				if (JhMyPackage::add(pdata) == 0)
				{
					data->count--;
					getResData.erase(it);
				}
				break;
			}
		}
	}
	else//不为0，数量-1
	{
		PackageData pdata = *data;
		pdata.count = 1;

		if (JhMyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}

	for (int i = 0; i < size; i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}
	//保存临时数据
	saveTempData();

	//更新UI
	updata();
	if (JhNewerGuideLayer::checkifNewerGuide(25))
		showNewerGuide(25);
	else if (JhNewerGuideLayer::checkifNewerGuide(30))
		showNewerGuide(30);
}

void JhActionGetLayer::onPackageItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	//点击背包栏
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = JhMyPackage::vec_packages[index];
	unsigned int i = 0;
	for (i = 0; i < getResData.size(); i++)
	{
		//奖励栏有背包中的资源，背包-1，奖励栏数据-1
		if (data.strid.compare(getResData[i].strid) == 0 && (getResData[i].type == FOOD || getResData[i].type == MEDICINAL || getResData[i].type == RES_1 || getResData[i].type == RES_2))
		{
			getResData[i].count++;
			break;
		}
	}

	//奖励栏没有背包中的资源，添加
	if (i == getResData.size())
	{
		data.count = 1;
		getResData.push_back(data);
	}

	//保存临时数据
	saveTempData();

	//背包 - 1,<0,会从背包中移除掉
	JhMyPackage::cutone(data.strid);

	//先移除奖励栏的物品在更新加载
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}
	//更新UI
	updata();
}

JhActionGetLayer* JhActionGetLayer::create(int rid, std::vector<int> res_ids, int type, int actype)
{
	JhActionGetLayer *pRet = new JhActionGetLayer();
	if (pRet && pRet->init(rid, res_ids, type, actype))
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

void JhActionGetLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhHomeHill* homehill = (JhHomeHill*)g_gameLayer->getChildByName("homehill");
		if (homehill != NULL)
		{
			if (JhNewerGuideLayer::checkifNewerGuide(28))
				homehill->showNewerGuide(28);
			else if (JhNewerGuideLayer::checkifNewerGuide(33))
				homehill->showNewerGuide(33);
		}
		this->removeFromParentAndCleanup(true);
	}
}

void JhActionGetLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);

	//1："继续采集", 2："继续砍伐", 3："继续挖掘"按钮
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		setBtnStatus(false);

		//更新后山资源列表中的数据，为0，就不出产出资源了，需等待资源恢复
		if (JhGlobalData::vec_resData[mrid].count > 0)
		{
			JhGlobalData::vec_resData[mrid].count--;
			std::string desc;
			delayDoAction(0);
			addEventText();
		}

		if (JhGlobalData::vec_resData[mrid].count <= 0)
		{
			m_getbtn->setEnabled(false);
		}
	}
}

void JhActionGetLayer::delayDoAction(float dt)
{
	if (m_actype == 1)
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_CAIJI);
	else if (m_actype == 2)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_KANSHU);
	}
	else if (m_actype == 3)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_WAJUE);
	}

	int resrid = atoi(JhGlobalData::vec_resData[mrid].strid.c_str());
	if (g_hero->getAtrByType((HeroAtrType)m_actype)->count > 0 || resrid == 69)//打水)//是否有工具m_actype：1："采集", 2："砍伐", 3："挖掘"
	{
		if (resrid != 69)
		{
			int r = JhGlobalData::createRandomNum(100);
			if (r < 80)
			{
				g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue--;
				std::string strid = g_hero->getAtrByType((HeroAtrType)m_actype)->strid;
				if (g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue <= 0)
				{

					std::string brokenstr = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("你的").c_str(), JhGlobalData::map_allResource[strid].cname.c_str(), JhCommonFuncs::gbk2utf("已损坏！！").c_str());
					g_uiScroll->addEventText(brokenstr, 25, Color3B(204, 4, 4));
					PackageData data;
					data.count = 0;
					g_hero->setAtrByType((HeroAtrType)m_actype, data);
				}
				else if (g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue == 20 || g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue == 10)
				{
					std::string descstr = StringUtils::format("%s%s%d", JhGlobalData::map_allResource[strid].cname.c_str(), JhCommonFuncs::gbk2utf("耐久度仅剩").c_str(), g_hero->getAtrByType((HeroAtrType)m_actype)->goodvalue);

					g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
				}
			}
		}
		doAction(0);
	}
	else
	{
		this->scheduleOnce(schedule_selector(JhActionGetLayer::doAction), 1.0f);
	}
}

void JhActionGetLayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	//全部拾取
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (unsigned int i = 0; i < getResData.size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			this->removeChildByName(name);
		}

		std::vector<PackageData>::iterator it;

		bool isfull = false;
		for (it = getResData.begin(); it != getResData.end();)
		{
			bool isOver = false;
			int count = it->count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = *it;
				data.count = 1;
				if (JhMyPackage::add(data) == 0)
				{
					if (--it->count <= 0)
					{
						it = getResData.erase(it);
						isOver = true;
						break;
					}
				}
				else
				{
					isfull = true;
					break;
				}
			}
			if (!isOver)
				it++;
			if (isfull)
				break;
		}
		saveTempData();
		updata();
		if (JhNewerGuideLayer::checkifNewerGuide(27))
			showNewerGuide(27);
		else if (JhNewerGuideLayer::checkifNewerGuide(32))
			showNewerGuide(32);
	}
}

void JhActionGetLayer::loadTempData()
{
	tempResData.clear();
	//临时存放的数据，保存在本地文件中，解析
	std::string datastr = JhGameDataSave::getInstance()->getTempStorage("m1-2");
	std::vector<std::string> vec_retstr;
	JhCommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		JhCommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;
		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = JhGlobalData::getResExType(data.strid);//atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		if (tmp.size() >= 9)
		{
			data.slv = atoi(tmp[7].c_str());
			data.tqu = atoi(tmp[8].c_str());
		}
		tempResData.push_back(data);
	}
}

void JhActionGetLayer::saveTempData()
{
	std::vector<PackageData> allResData = tempResData;

	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		int tmpsize = tempResData.size();
		int j = 0;
		for (j = 0; j < tmpsize; j++)
		{
			if (getResData[i].strid.compare(tempResData[j].strid) == 0 && (getResData[i].type == FOOD || getResData[i].type == MEDICINAL || getResData[i].type == RES_1 || getResData[i].type == RES_2))
			{
				allResData[j].count += getResData[i].count;
				break;
			}
		}
		if (j == tmpsize)
		{
			allResData.push_back(getResData[i]);
		}
	}
	JhGlobalData::map_tempGf_Equip["m1-2"].clear();
	std::string str;
	for (unsigned int i = 0; i < allResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", allResData[i].strid.c_str(), allResData[i].type, allResData[i].count, allResData[i].extype, allResData[i].lv, allResData[i].exp, allResData[i].goodvalue, allResData[i].slv, allResData[i].tqu);
		str.append(onestr);
		int tmptype = allResData[i].type;
		std::string tmpstrid = allResData[i].strid;
		if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
		{
			JhGlobalData::map_tempGf_Equip["m1-2"].push_back(tmpstrid);
		}
	}
	JhGameDataSave::getInstance()->setTempStorage("m1-2", str.substr(0, str.length() - 1));
}

void JhActionGetLayer::updata()
{
	//奖励栏资源UI更新
	updataRewardUI();

	//背包栏资源UI更新
	updataMyPackageUI();
}

void JhActionGetLayer::updataMyPackageUI()
{
	for (int i = 0; i < JhMyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}

	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = JhMyPackage::vec_packages[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
		}
		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhActionGetLayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(150 + i * 130, 425));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", JhMyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", JhMyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/SIMHEI.TTF", 20);
		reslbl->setPosition(Vec2(box->getContentSize().width - 20, 5));
		reslbl->enableOutline(Color4B(143, 85, 60, 255), 2);
		box->addChild(reslbl);
	}
}


void JhActionGetLayer::updataRewardUI()
{
	for (unsigned int i = 0; i < getResData.size(); i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		PackageData tmpdata = getResData[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhActionGetLayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&getResData[i]);
		boxItem->setPosition(Vec2(150 + i * 135, 625));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", getResData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", getResData[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/SIMHEI.TTF", 20);
		reslbl->setPosition(Vec2(box->getContentSize().width - 20, 5));
		reslbl->enableOutline(Color4B(143, 85, 60, 255), 2);
		box->addChild(reslbl);
	}
}

//退出也要保存下临时存放的数据
void JhActionGetLayer::onExit()
{
	saveTempData();
	Layer::onExit();
}

void JhActionGetLayer::setBtnStatus(bool enable)
{
	m_getbtn->setEnabled(enable);
	m_getallbtn->setEnabled(enable);
	m_backbtn->setEnabled(enable);
}

void JhActionGetLayer::addEventText()
{
	int resrid = atoi(JhGlobalData::vec_resData[mrid].strid.c_str());
	std::string desc;
	if (g_hero->getAtrByType((HeroAtrType)m_actype)->count > 0)
	{
		desc = JhCommonFuncs::gbk2utf(acdesc1[m_actype].c_str());
		desc.append(JhGlobalData::vec_resData[mrid].unitname);
		g_uiScroll->addEventText(desc);
	}
	else
	{
		desc = JhCommonFuncs::gbk2utf(acdesc[m_actype].c_str());
		if (resrid != 69)
			this->scheduleOnce(schedule_selector(JhActionGetLayer::showDotDot), 0.2f);
		g_uiScroll->addEventText(desc);
	}
	if (resrid == 69)//打水
	{
		desc = JhCommonFuncs::gbk2utf("你三下五除二从水井里打出");
		desc.append(JhGlobalData::vec_resData[mrid].unitname);
		g_uiScroll->addEventText(desc);
	}
}

void JhActionGetLayer::showDotDot(float dt)
{
	g_uiScroll->addEventText("..........");
	this->scheduleOnce(schedule_selector(JhActionGetLayer::addEventText2), 1.0f);
}

void JhActionGetLayer::addEventText2(float dt)
{
	std::string desc;
	desc = JhCommonFuncs::gbk2utf(acdesc2[m_actype].c_str());
	desc.append(JhGlobalData::vec_resData[mrid].unitname);
	g_uiScroll->addEventText(desc);
}

void JhActionGetLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 24 || step == 29)
	{
		nodes.push_back(this->getChildByName("resitem0")->getChildren().at(0));
	}
	else if (step == 25 || step == 30)
	{
		nodes.push_back(m_getbtn);
	}
	else if (step == 26 || step == 31)
	{
		nodes.push_back(m_getallbtn);
	}
	else if (step == 27 || step == 32)
	{
		nodes.push_back(m_backbtn);
	}
	g_gameLayer->showNewerGuide(step, nodes);
}