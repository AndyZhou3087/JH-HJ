#include "JhHSLJRewardLayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhSoundManager.h"
#include "JhGameScene.h"
#include "JhWaitingProgress.h"
#include "JhHintBox.h"

JhHSLJRewardLayer::JhHSLJRewardLayer()
{
}


JhHSLJRewardLayer::~JhHSLJRewardLayer()
{
}


JhHSLJRewardLayer* JhHSLJRewardLayer::create(int goldcount)
{
	JhHSLJRewardLayer *pRet = new JhHSLJRewardLayer();
	if (pRet && pRet->init(goldcount))
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

bool JhHSLJRewardLayer::init(int goldcount)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhhsljRewardLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *okbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJRewardLayer::onGet, this));

	cocos2d::ui::Text* namelbl = (cocos2d::ui::Text*)csbnode->getChildByName("namelbl");
	std::string str = StringUtils::format("金元宝x%d", goldcount);
	namelbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	m_goldcount = goldcount;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhHSLJRewardLayer::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		JhServerDataSwap::init(this)->getHSLJRewardData();
	}
}

void JhHSLJRewardLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() + m_goldcount);
	this->removeFromParentAndCleanup(true);
}

void JhHSLJRewardLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	this->addChild(JhHintBox::create(JhCommonFuncs::gbk2utf("领取失败，请检查网络设置或者稍后重试！")));
}