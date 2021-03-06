﻿#include "JhRechargeLayer.h"
#include "JhCommonFuncs.h"
#include "JhSoundManager.h"
#include "JhGlobalData.h"
#include "JhConst.h"
#include "JhWaitingProgress.h"
#include "JhGameScene.h"
#include "JhStorageRoom.h"

JhRechargeLayer::JhRechargeLayer()
{
	canGetHightAmount = 0;
}


JhRechargeLayer::~JhRechargeLayer()
{

}

bool JhRechargeLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhrechargeLayer.csb");
	this->addChild(m_csbnode);

	getbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onGet, this));

	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onClose, this));

	cocos2d::ui::ImageView* bg = (cocos2d::ui::ImageView*)m_csbnode->getChildByName("bg");
	bg->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onBGClick, this));

	getbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onGet, this));
	getbtn->setEnabled(false);

	datelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("time");

	progresstext = (cocos2d::ui::Text*)m_csbnode->getChildByName("progresstext");
	progressbar = (cocos2d::ui::LoadingBar*)m_csbnode->getChildByName("progressbar");

	descbg = (cocos2d::ui::Widget*)m_csbnode->getChildByName("descbg");
	descbg->setVisible(false);

	scrollView = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("ScrollView");

	JhWaitingProgress* waitbox = JhWaitingProgress::create("获取数据中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	JhServerDataSwap::init(this)->getRechargeData();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

JhRechargeLayer* JhRechargeLayer::create()
{
	JhRechargeLayer *pRet = new JhRechargeLayer();
	if (pRet && pRet->init())
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

void JhRechargeLayer::showdata()
{

	datelbl->setString(JhGlobalData::recharageData.rtime);

	splitRechargeAward();

	std::string str = StringUtils::format("%d元/%d元", JhGlobalData::recharageData.myrechage, maxamount);
	progresstext->setString(JhCommonFuncs::gbk2utf(str.c_str()));
	if (maxamount > 0)
	{
		float percent = JhGlobalData::recharageData.myrechage * 100 / maxamount;
		if (percent > 100.00f)
			percent = 100.00f;
		progressbar->setPercent(percent);
	}

	int itemheight = 155;
	int innerheight = itemheight * map_recharge.size();
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, innerheight));

	std::map<int, std::vector<std::string>>::iterator it;
	int mk = 0;
	//JhGlobalData::recharageData.myrechage = 200;
	for (it = map_recharge.begin(); it != map_recharge.end(); it++)
	{
		int amount = it->first;

		float percent = amount * 100 / maxamount;
		std::string itemboxstr = "ui/buildingbanner1.png";
		int itemboxy = 630;
		float scale = 0.6f;
		int countoffestx = 7;
		int countoffsety = 12;
		Color3B countcolor = Color3B::WHITE;

		//add
		Node* node = CSLoader::createNode("jhrechargeNode.csb");
		scrollView->addChild(node);
		node->setPosition(Vec2(297, innerheight - itemheight / 2 - mk * 153));
		//add
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)node->getChildByName("item");
		item->setColor(Color3B(224, 251, 224));
		cocos2d::ui::ImageView* boxItem = (cocos2d::ui::ImageView*)node->getChildByName("box");
		cocos2d::ui::Button* m_getbtn = (cocos2d::ui::Button*)node->getChildByName("actionbtn");
		m_getbtn->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onGet, this));
		m_getbtn->setTag(mk);
		m_getbtn->setEnabled(false);
		cocos2d::ui::Text* countlbl = (cocos2d::ui::Text*)node->getChildByName("count");
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)node->getChildByName("desc");
		cocos2d::ui::ImageView* icon = (cocos2d::ui::ImageView*)node->getChildByName("icon");
		//icon->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onBoxClick, this));
		//icon->setTag(amount);
		//icon->setTouchEnabled(true);
		showDescAward(amount, node);

		if (JhGlobalData::recharageData.myrechage >= amount)
		{
			if (JhGlobalData::recharageData.mygotton < amount)
			{
				itemboxstr = "ui/buildingbanner2.png";
				itemboxy = 620;
				scale = 0.7f;
				countoffestx = 20;
				countoffsety = 17;
				countcolor = Color3B::BLACK;
				for (unsigned int i = 0; i < map_recharge[it->first].size(); i++)
				{
					vec_myawrd.push_back(map_recharge[it->first][i]);
				}
				if (canGetHightAmount < amount)
					canGetHightAmount = amount;

				item->setColor(Color3B(255, 255, 255));
				//add
				m_getbtn->setEnabled(true);
				m_getbtn->setTitleText(JhCommonFuncs::gbk2utf("领取"));
			}
			else
			{
				/*cocos2d::ui::ImageView* getimg = cocos2d::ui::ImageView::create("images/rechargeget.png", cocos2d::ui::Widget::TextureResType::LOCAL);
				getimg->setPosition(Vec2(progressbar->getPositionX() + percent *progressbar->getContentSize().width / 100, itemboxy - 15));
				m_csbnode->addChild(getimg, 1);*/
				m_getbtn->setEnabled(false);
				m_getbtn->setTitleText(JhCommonFuncs::gbk2utf("已领取"));
				item->setColor(Color3B(255, 255, 255));

			}
		}
		/*cocos2d::ui::ImageView* boxItem = cocos2d::ui::ImageView::create(itemboxstr, cocos2d::ui::Widget::TextureResType::LOCAL);
		boxItem->addTouchEventListener(CC_CALLBACK_2(JhRechargeLayer::onBoxClick, this));
		boxItem->setTouchEnabled(true);
		boxItem->setTag(amount);
		boxItem->setPosition(Vec2(progressbar->getPositionX() + percent *progressbar->getContentSize().width / 100, itemboxy));
		m_csbnode->addChild(boxItem);*/


		std::string strid;
		int lv = 0;
		std::string countstr;
		if (map_recharge[it->first].size() > 1)
		{
			strid = "ui/gp1.png";
		}
		else
		{
			std::string resstr = map_recharge[it->first][0];
			if (resstr.find("a") != std::string::npos || resstr.find("e") != std::string::npos || resstr.find("x") != std::string::npos || resstr.find("w") != std::string::npos)
			{
				if (resstr.length() > 4)
				{
					strid = StringUtils::format("ui/%s.png", resstr.substr(resstr.length() - 4).c_str());
					lv = atoi(resstr.substr(0, resstr.length() - 4).c_str());
					countstr = StringUtils::format("Lv.%d", lv);
				}
				else
				{
					strid = StringUtils::format("ui/%s.png", resstr.c_str());;
				}
			}
			else
			{
				int intres = atoi(resstr.c_str());
				strid = StringUtils::format("ui/%d.png", intres / 1000);
				countstr = StringUtils::format("x%d", intres % 1000);
			}
		}

		/*cocos2d::ui::ImageView* res = cocos2d::ui::ImageView::create(strid, cocos2d::ui::Widget::TextureResType::PLIST);
		res->setPosition(Vec2(boxItem->getContentSize().width / 2, boxItem->getContentSize().height / 2 - 10));
		boxItem->addChild(res);
		res->setScale(scale);*/
		icon->loadTexture(strid, cocos2d::ui::Widget::TextureResType::PLIST);
		countlbl->setString(countstr);

		//if (countstr.length() > 0)
		//{
		//	Label * countlbl = Label::createWithTTF(countstr, "fonts/STXINGKA.TTF", 20);
		//	//countlbl->setAnchorPoint(Vec2(1, 0.5));
		//	countlbl->setPosition(Vec2(boxItem->getContentSize().width/2, countoffsety));
		//	countlbl->setColor(countcolor);
		//	boxItem->addChild(countlbl);
		//}

		/*Label * pricelbl = Label::createWithTTF(countstr, "fonts/STXINGKA.TTF", 22);
		std::string pricestr = StringUtils::format("满%d元", amount);
		pricelbl->setString(JhCommonFuncs::gbk2utf(pricestr.c_str()));
		pricelbl->setColor(Color3B(159, 116, 77));
		pricelbl->setPosition(Vec2(boxItem->getPositionX(), boxItem->getPositionY() - 50));
		scrollView->addChild(pricelbl);*/

		std::string pricestr = StringUtils::format("活动期间充值消费%d元", amount);
		desc->setString(JhCommonFuncs::gbk2utf(pricestr.c_str()));

		mk++;
	}

	if (vec_myawrd.size() > 0)
		getbtn->setEnabled(true);
	
}

void JhRechargeLayer::showDescAward(int amount, Node* node)
{
	//node->removeAllChildren();
	//node->setVisible(true);
	std::vector<std::string> vec_res = map_recharge[amount];
	int rescount = vec_res.size();
	int startx[] = { -50, 180, 150, 90, -110 };
	int spacex[] = { 85, 185, 150, 125, 85 };
	for (int i = 0; i < rescount; i++)
	{
		std::string boxstr = "ui/buildsmall.png";
		std::string resid;
		std::string strcount;
		if (vec_res[i].find("a") != std::string::npos || vec_res[i].find("e") != std::string::npos)
		{
			resid = vec_res[i].substr(vec_res[i].length() - 4);
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[resid].qu);

			int lv = atoi(vec_res[i].substr(0, vec_res[i].length() - 4).c_str());
			if (lv > 0)
				strcount = StringUtils::format("强化+%d", lv);
		}
		else if (vec_res[i].find("x") != std::string::npos || vec_res[i].find("w") != std::string::npos)
		{
			resid = vec_res[i].substr(vec_res[i].length() - 4);
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[resid].qu);

			int lv = atoi(vec_res[i].substr(0, vec_res[i].length() - 4).c_str());
			if (lv > 0)
				strcount = StringUtils::format("强化+%d", lv);
		}
		else
		{
			resid = StringUtils::format("%d", atoi(vec_res[i].c_str()) / 1000);
			int rcount = atoi(vec_res[i].c_str()) % 1000;
			strcount = StringUtils::format("x%d", rcount);
		}

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(startx[rescount - 1] + i*spacex[rescount - 1], -10));
		box->setScale(0.6f);
		node->addChild(box);

		std::string str = StringUtils::format("ui/%s.png", resid.c_str());
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);

		/*Label * namelbl = Label::createWithTTF(JhGlobalData::map_allResource[resid].cname, "fonts/SIMHEI.TTF", 18);
		namelbl->setColor(Color3B(0, 0, 0));
		namelbl->setPosition(Vec2(box->getPositionX(), -55));
		node->addChild(namelbl);*/

		if (strcount.length() > 0)
		{
			Label * coutlbl = Label::createWithTTF(JhCommonFuncs::gbk2utf(strcount.c_str()), "fonts/SIMHEI.TTF", 18);
			coutlbl->setAnchorPoint(Vec2(1, 0.5f));
			coutlbl->setColor(Color3B(255, 255, 255));
			coutlbl->enableOutline(Color4B(85, 58, 11, 255), 2);
			coutlbl->setPosition(Vec2(box->getPositionX() + 30, -35));
			node->addChild(coutlbl);
		}
	}


}

void JhRechargeLayer::onBoxClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		//showDescAward(node->getTag());
	}
}

void JhRechargeLayer::splitRechargeAward()
{
	//JhGlobalData::recharageData.rewardstr = "6-10005,11005,20005,21005,22005;88-12020;168-82020;368-15e010;498-15a020";
	std::vector<std::string> vec_tmp;
	JhCommonFuncs::split(JhGlobalData::recharageData.rewardstr, vec_tmp, ";");
	for (unsigned int i = 0; i < vec_tmp.size(); i++)
	{
		std::vector<std::string> vec_tmp1;
		JhCommonFuncs::split(vec_tmp[i], vec_tmp1, "-");
		int price = atoi(vec_tmp1[0].c_str());
		std::vector<std::string> vec_tmp2;
		JhCommonFuncs::split(vec_tmp1[1], vec_tmp2, ",");
		map_recharge[price] = vec_tmp2;
		if (i == vec_tmp.size() - 1)
			maxamount = price;
	}
}

int JhRechargeLayer::getRechargeMaxAmount()
{
	if (JhGlobalData::recharageData.rewardstr.length() > 0)
	{
		std::vector<std::string> vec_tmp;
		JhCommonFuncs::split(JhGlobalData::recharageData.rewardstr, vec_tmp, ";");
		for (unsigned int i = 0; i < vec_tmp.size(); i++)
		{
			std::vector<std::string> vec_tmp1;
			JhCommonFuncs::split(vec_tmp[i], vec_tmp1, "-");
			int price = atoi(vec_tmp1[0].c_str());
			if (i == vec_tmp.size() - 1)
				return price;
		}
	}
	return 0;
}

void JhRechargeLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btn = (Node*)pSender;
		getbtn->setEnabled(false);
		int tag = btn->getTag();
		for (int i = 0; i < vec_myawrd.size(); i++)
		{
			std::string boxstr = "ui/buildsmall.png";
			std::string resid;
			int lv = 0;
			int count = 0;
			bool isAdd = false;
			PackageData pdata;
			if (vec_myawrd[i].find("a") != std::string::npos || vec_myawrd[i].find("e") != std::string::npos)
			{
				resid = vec_myawrd[i].substr(vec_myawrd[i].length() - 4);

				lv = atoi(vec_myawrd[i].substr(0, vec_myawrd[i].length() - 4).c_str());
				count = 1;
				PackageData* mydata = g_hero->getGF_Equip(resid);
				if (mydata == NULL && JhGlobalData::tempHasGf_Equip(resid).length() <= 0)
				{
					isAdd = true;
					pdata.slv = lv;
				}
				else if (mydata != NULL)
				{
					if (mydata->slv < lv)
						mydata->slv = lv;
				}
			}
			else if (vec_myawrd[i].find("x") != std::string::npos || vec_myawrd[i].find("w") != std::string::npos)
			{
				resid = vec_myawrd[i].substr(vec_myawrd[i].length() - 4);

				lv = atoi(vec_myawrd[i].substr(0, vec_myawrd[i].length() - 4).c_str());

				count = 1;
				PackageData* mydata = g_hero->getGF_Equip(resid);
				if (mydata == NULL && JhGlobalData::tempHasGf_Equip(resid).length() <= 0)
				{
					isAdd = true;
					pdata.lv = lv - 1;
				}
				else if (mydata != NULL)
				{
					if (mydata->lv < lv - 1)
						mydata->lv = lv - 1;
				}
			}
			else
			{
				resid = StringUtils::format("%d", atoi(vec_myawrd[i].c_str()) / 1000);
				count = atoi(vec_myawrd[i].c_str()) % 1000;
				isAdd = true;
			}
			if (isAdd)
			{

				pdata.strid = resid;
				pdata.count = count;
				pdata.extype = JhGlobalData::getResExType(resid);
				pdata.type = JhGlobalData::getResType(resid);
				JhStorageRoom::add(pdata);
			}

		}
		JhServerDataSwap::init(NULL)->postMyRecharge(canGetHightAmount, 1);
		this->removeFromParentAndCleanup(true);
	}
}

void JhRechargeLayer::onBGClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (descbg->isVisible())
			descbg->setVisible(false);
	}
}

void JhRechargeLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhRechargeLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	showdata();
}

void JhRechargeLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}