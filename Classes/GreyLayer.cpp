#include "GreyLayer.h"
#include "Resource.h"
USING_NS_CC;

GreyLayer::GreyLayer():
m_bg(NULL),
m_eventLis(NULL)
{

}

GreyLayer::~GreyLayer() {
	if (nullptr != m_eventLis)
		_eventDispatcher->removeEventListener(m_eventLis);
}

bool GreyLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	initBg();
	return true;
}

void GreyLayer::initBg() {
	const Size size = Director::getInstance()->getVisibleSize();

	m_bg = Sprite::create(s_bg_grey);
	addChild(m_bg);
	float scaleX = size.width/m_bg->getContentSize().width;
	float scaleY = size.height/m_bg->getContentSize().height;
	m_bg->setScale(scaleX, scaleY);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	m_eventLis = EventListenerTouchOneByOne::create();
	m_eventLis->onTouchBegan = [](Touch * ,Event *)	
	{	
		return true;	
	};

	m_eventLis->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_eventLis,this);
}

void GreyLayer::swallowTouches(bool swallowed) {
	if (nullptr == m_eventLis) {
		return;
	}

	m_eventLis->setSwallowTouches(swallowed);
}

GreyLayerController::GreyLayerController()
{

}

GreyLayerController::~GreyLayerController() {

}

void GreyLayerController::initial() {

}

void GreyLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPosition(Vec2(size.width * 0.5, 0));

	m_bEnter = true;

	//屏蔽底下的触摸事件
	setTouchable(true);
}

void GreyLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPositionY(size.height * 2);
	m_bEnter = false;

	//解除屏蔽底下的触摸事件
	setTouchable(false);
}

void GreyLayerController::setTouchable(bool enable) {
	auto layer = dynamic_cast<GreyLayer*>(m_layer);
	if (nullptr != layer) {
		layer->swallowTouches(enable);
	}
}