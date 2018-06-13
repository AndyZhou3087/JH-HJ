#include "JhMixGFDoneLayer.h"
#include "JhGlobalData.h"
#include "JhConst.h"
JhMixGFDoneLayer::JhMixGFDoneLayer()
{

}


JhMixGFDoneLayer::~JhMixGFDoneLayer()
{
}

bool JhMixGFDoneLayer::init(int ret)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhmixDoneLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView* doneimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("doneimg");
	std::string str;
	if (ret == 0)
		str = StringUtils::format("images/mixdoneok.png");
	else
		str = StringUtils::format("images/mixdonefail.png");
	doneimg->loadTexture(str, cocos2d::ui::Widget::TextureResType::LOCAL);

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleOnce(schedule_selector(JhMixGFDoneLayer::removeself), 3.0f);

	return true;
}

JhMixGFDoneLayer* JhMixGFDoneLayer::create(int ret)
{
	JhMixGFDoneLayer *pRet = new JhMixGFDoneLayer();
	if (pRet && pRet->init(ret))
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

void JhMixGFDoneLayer::removeself(float dt)
{
	this->removeFromParentAndCleanup(true);
}