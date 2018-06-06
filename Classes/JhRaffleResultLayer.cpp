#include "JhRaffleResultLayer.h"
#include "JhConst.h"
#include "SoundManager.h"
#include "JhAnalyticUtil.h"
#include "JhCommonFuncs.h"

JhRaffleResultLayer::JhRaffleResultLayer()
{
}


JhRaffleResultLayer::~JhRaffleResultLayer()
{

}

JhRaffleResultLayer* JhRaffleResultLayer::create()
{
	JhRaffleResultLayer *pRet = new JhRaffleResultLayer();
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

bool JhRaffleResultLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhraffleResultLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhRaffleResultLayer::onOK, this));

	cocos2d::ui::Text* stagelbl = (cocos2d::ui::Text*)csbnode->getChildByName("stagelbl");
	stagelbl->setString(JhGlobalData::myRaffleData.mywinstage);

	std::string str;
	cocos2d::ui::Text* wingoldlbl = (cocos2d::ui::Text*)csbnode->getChildByName("wingold");
	str = StringUtils::format("%d", JhGlobalData::myRaffleData.mywingold);
	wingoldlbl->setString(str);
	
	wingold = JhGlobalData::myRaffleData.mywingold;

	std::string rankstr[] = { "一", "二", "三" };
	int rankpercent[] = { 50, 30, 20 };

	int rank = JhGlobalData::myRaffleData.mywinrank;
	if (rank >= 1 && rank <= 3)
	{
		cocos2d::ui::Text* ranklbl = (cocos2d::ui::Text*)csbnode->getChildByName("ranklbl");
		str = StringUtils::format("%s", rankstr[rank-1].c_str());
		ranklbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));

		cocos2d::ui::Text* poolnumlbl = (cocos2d::ui::Text*)csbnode->getChildByName("poolnum");
		str = StringUtils::format("%d", JhGlobalData::myRaffleData.mywingold * 100 / rankpercent[rank-1]);
		poolnumlbl->setString(str);
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhRaffleResultLayer::onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() + wingold);
		this->removeFromParentAndCleanup(true);
	}
}