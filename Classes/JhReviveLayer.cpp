#include "JhReviveLayer.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhDeathLayer.h"
#include "JhSoundManager.h"
#include "JhShopLayer.h"
#include "JhConst.h"
#include "JhFightLayer.h"
#include "MD5.h"
#include "JhHintBox.h"
#include "JhAnalyticUtil.h"
#include "JhHelpMainLayer.h"

#ifdef UMENG_SHARE
#include "UmengShare/Common/CCUMSocialSDK.h"
USING_NS_UM_SOCIAL;
#include "iosfunc.h"
#endif

JhReviveLayer::JhReviveLayer()
{

}


JhReviveLayer::~JhReviveLayer()
{

}

bool JhReviveLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhreviveLayer.csb");
	this->addChild(m_csbnode);

	m_cancelbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("cacelbtn");
	m_cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onCancel, this));

	m_sharetext = (cocos2d::ui::Text*)m_csbnode->getChildByName("sharetext");

	cocos2d::ui::Button* revivebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("revivebtn");
	revivebtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onRevive, this));

	cocos2d::ui::Button* helpbtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("helpbtn");
	helpbtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onHelp, this));

	m_revivedesc = (cocos2d::ui::Text*)m_csbnode->getChildByName("revivecountlbl");

	cocos2d::ui::Text* revivetxtlbl = (cocos2d::ui::Text*)revivebtn->getChildByName("text");
	cocos2d::ui::Text* revivepricelbl = (cocos2d::ui::Text*)m_csbnode->getChildByName("price");
	cocos2d::ui::Widget* reviveicon = (cocos2d::ui::Widget*)m_csbnode->getChildByName("priceicon");

	revivecount = JhStorageRoom::getCountById("73");

	needgold = JhGlobalData::getReviveCount() * 20;

	for (unsigned int i = 0; i < JhMyPackage::vec_packages.size(); i++)
	{
		if (JhMyPackage::vec_packages[i].strid.compare("73") == 0)
		{
			revivecount += JhMyPackage::vec_packages[i].count;
			break;
		}
	}

	if (revivecount > 0)
	{
		m_revivedesc->setVisible(true);
		std::string strcount = StringUtils::format("复活药水x%d", revivecount);
		m_revivedesc->setString(JhCommonFuncs::gbk2utf(strcount.c_str()));
		revivepricelbl->setVisible(false);
		reviveicon->setVisible(false);
		//revivetxtlbl->setPositionY(35);
	}
	else
	{
		m_revivedesc->setVisible(false);
		this->schedule(schedule_selector(JhReviveLayer::checkGoldCount), 1);
		revivepricelbl->setVisible(true);
		std::string pricestr = StringUtils::format("%d", needgold);
		revivepricelbl->setString(pricestr);
		reviveicon->setVisible(true);
		//revivetxtlbl->setPositionY(40);
	}

#ifdef UMENG_SHARE
	m_closebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("closebtn");
	m_closebtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onCancel, this));

	m_sharebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("sharebtn");
	m_sharebtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onShare, this));
	checkShareReviveCount(0);
	this->schedule(schedule_selector(JhReviveLayer::checkShareReviveCount), 0.5f);
#else
	m_closebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("closebtn");
	m_closebtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onCancel, this));

	m_freeRevivebtn = (cocos2d::ui::Button*)m_csbnode->getChildByName("freerevivebtn");
	m_freeRevivebtn->addTouchEventListener(CC_CALLBACK_2(JhReviveLayer::onFreeRevive, this));

	checkFreeReviveCount(0);
	this->schedule(schedule_selector(JhReviveLayer::checkFreeReviveCount), 0.1f);
#endif

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

JhReviveLayer* JhReviveLayer::create()
{
	JhReviveLayer *pRet = new JhReviveLayer();
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

#ifdef UMENG_SHARE
void JhReviveLayer::onShare(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string sharetxt[] = { "良心独立游戏！大侠也需要吃饭！装备全靠打！快来助我一统江湖！", "欲练此功，必先自宫？！大侠还请三思啊！！！", "出门只有一匹马！武功全靠打！快来跟我刷武功！", "谁是天下第一？是我！我可以打十个！不服来战！" };
		int textsize = sizeof(sharetxt) / sizeof(sharetxt[0]);
		int r = JhGlobalData::createRandomNum(textsize);
		CCUMSocialSDK *sdk = CCUMSocialSDK::create();
//		vector<int>* platforms = new vector<int>();
//		//platforms->push_back(QZONE);
//		//platforms->push_back(QQ);
//		//platforms->push_back(WEIXIN);
//		platforms->push_back(WEIXIN_CIRCLE);
//		sdk->openShare(platforms, "武林群侠传", sharetxt[r].c_str(), "Icon-87.png",
//		"https://itunes.apple.com/cn/app/%E6%AD%A6%E6%9E%97%E7%BE%A4%E4%BE%A0%E4%BC%A0-%E9%AB%98%E8%87%AA%E7%94%B1%E5%BA%A6%E6%AD%A6%E4%BE%A0%E5%85%BB%E6%88%90%E6%B8%B8%E6%88%8F/id1243387739?mt=8",share_selector(JhReviveLayer::shareCallback));
        sdk->directShare(WEIXIN_CIRCLE, "武林群侠传", sharetxt[r].c_str(),"https://itunes.apple.com/cn/app/%E6%AD%A6%E6%9E%97%E7%BE%A4%E4%BE%A0%E4%BC%A0-%E9%AB%98%E8%87%AA%E7%94%B1%E5%BA%A6%E6%AD%A6%E4%BE%A0%E5%85%BB%E6%88%90%E6%B8%B8%E6%88%8F/id1243387739?mt=8","Icon-87.png",share_selector(JhReviveLayer::shareCallback));
        //UMShare();
	}
}
/*
* 分享回调
* @param platform 要分享到的目标平台
* @param stCode 返回码, 200代表分享成功, 100代表开始分享
* @param errorMsg 分享失败时的错误信息,android平台没有错误信息
*/
void JhReviveLayer::shareCallback(int platform, int stCode, string& errorMsg) {

	log("#### share callback!!!!!! stCode = %d", stCode);

	string result = "";
	if (stCode == 200) {
		result = "分享成功";

		reviveOk();
		JhGlobalData::setShareDay(JhGlobalData::getDayOfYear());
		JhGlobalData::setFreeReviveCount(JhGlobalData::getFreeReviveCount() - 1);
		JhServerDataSwap::init()->updateFreeReviveCount();
	}
	else if (stCode == -1) {
		result = "分享取消";
	}
	else {
		result = "分享失败";
	}

}
#endif

void JhReviveLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		if (g_gameLayer != NULL)
		{
			g_gameLayer->removeChildByName("fightlayer");
			g_gameLayer->saveAllData();
		}
		JhDeathLayer* layer = JhDeathLayer::create();
		Director::getInstance()->getRunningScene()->addChild(layer);
	}
}

void JhReviveLayer::onFreeRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		reviveOk();

		JhGlobalData::setShareDay(JhGlobalData::getDayOfYear());

		JhGlobalData::setFreeReviveCount(JhGlobalData::getFreeReviveCount() - 1);
		JhServerDataSwap::init()->updateFreeReviveCount();
#ifdef ANALYTICS
		JhAnalyticUtil::onEvent("freerevive");
#endif
	}
}

void JhReviveLayer::onRevive(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (revivecount > 0)
		{
			doRevive();
		}
		else
		{
			int rcount = JhGlobalData::getReviveCount();
			int revivegold = rcount * 20;
			int mygold = JhGlobalData::getMyGoldCount();
			if (mygold >= revivegold)
			{
				if (JhGlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					JhGlobalData::dataIsModified = true;
					JhGlobalData::setMyGoldCount(0);
					JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}

				JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - revivegold);
				if (++rcount > 6)
					rcount = 6;
				JhGlobalData::setReviveCount(rcount);
				int usegold = JhGlobalData::getUseGold() + revivegold;
				JhGlobalData::setUseGold(usegold);
				reviveOk();
#ifdef ANALYTICS
				JhAnalyticUtil::onEvent("goldrevive");
#endif
			}
			else
				Director::getInstance()->getRunningScene()->addChild(JhShopLayer::create(), 1000);
		}
	}
}

void JhReviveLayer::reviveOk()
{
	if (g_gameLayer != NULL)
	{
		g_gameLayer->removeChildByName("revivelayer");
		if (g_hero != NULL && !g_hero->getIsWDChallenge())
			g_gameLayer->removeChildByName("fightlayer");

		g_gameLayer->heroRevive();

		if (g_gameLayer->getChildByName("challengecountlayer") == NULL)
		{
			JhFightLayer* fightlayer = (JhFightLayer*)g_gameLayer->getChildByName("fightlayer");
			if (fightlayer != NULL)
				fightlayer->showChallengeCountLayer(true);
		}
	}
}
void JhReviveLayer::doRevive()
{
	reviveOk();
	for (unsigned int i = 0; i < JhMyPackage::vec_packages.size(); i++)
	{
		if (JhMyPackage::vec_packages[i].strid.compare("73") == 0)
		{
			JhMyPackage::cutone("73");
			return;
		}
	}

	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = JhStorageRoom::map_storageData.begin(); it != JhStorageRoom::map_storageData.end(); ++it)
	{
		for (unsigned int i = 0; i < JhStorageRoom::map_storageData[it->first].size(); i++)
		{
			if (JhStorageRoom::map_storageData[it->first][i].strid.compare("73") == 0)
			{
				JhStorageRoom::use("73");
				return;
			}
		}
	}
}

void JhReviveLayer::checkGoldCount(float dt)
{
	if (JhGlobalData::getMyGoldCount() < needgold)
	{
		m_revivedesc->setVisible(true);
		m_revivedesc->setString(JhCommonFuncs::gbk2utf("金元宝不足"));
	}
	else
		m_revivedesc->setVisible(false);
}

void JhReviveLayer::checkShareReviveCount(float dt)
{
	if (JhGlobalData::getDayOfYear() != JhGlobalData::getShareDay() && JhGlobalData::getFreeReviveCount() > 0)
	{
		m_closebtn->setVisible(true);
		m_cancelbtn->setVisible(false);
		m_sharetext->setVisible(true);
		m_sharebtn->setVisible(true);
	}
	else
	{
		m_closebtn->setVisible(false);
		m_cancelbtn->setVisible(true);
		m_sharebtn->setVisible(false);
		m_sharetext->setVisible(false);
	}
}
void JhReviveLayer::checkFreeReviveCount(float dt)
{
	if (JhGlobalData::getDayOfYear() != JhGlobalData::getShareDay() && JhGlobalData::getFreeReviveCount() > 0)
	{
		m_closebtn->setVisible(true);
		m_cancelbtn->setVisible(false);
		m_sharetext->setString(JhCommonFuncs::gbk2utf("（每天免费复活一次！）"));
		m_sharetext->setTextColor(Color4B(0, 0, 0, 255));
		m_sharetext->setVisible(true);
		m_freeRevivebtn->setVisible(true);
	}
	else
	{
		m_closebtn->setVisible(false);
		m_cancelbtn->setVisible(true);
		m_freeRevivebtn->setVisible(false);

		if (JhGlobalData::isOnline)
		{
			int difsec = JhGlobalData::getTomorrowZeroTimeDif();
			std::string rtext = StringUtils::format("%02d:%02d:%02d后可免费复活一次！", difsec / 3600, difsec % 3600 / 60, difsec % 3600 % 60);
			m_sharetext->setVisible(true);
			m_sharetext->setString(JhCommonFuncs::gbk2utf(rtext.c_str()));
			m_sharetext->setTextColor(Color4B(204, 4, 4, 255));
		}
	}
}

void JhReviveLayer::onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhHelpMainLayer* layer = JhHelpMainLayer::create();
		this->addChild(layer);
	}
}