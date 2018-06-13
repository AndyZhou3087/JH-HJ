#include "GameLevelDemoLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GreyLayer.h"
#include "cocos2d.h"
#include "DemoPetData.h"
#include "DemoPetMatrix.h"
#include "BgLayerController.h"
#include "GameEvent.h"
USING_NS_CC;

#define MASK_RECT_TAG 100
#define MASK_Z_ORDER	10
#define ARROW_Z_ORDER	11


GameLevelDemoLayer::GameLevelDemoLayer() {

}

GameLevelDemoLayer::~GameLevelDemoLayer() {

}

bool GameLevelDemoLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	return true;
}

GameLevelDemoLayerController::GameLevelDemoLayerController():
m_eventLis(NULL),
m_mask(NULL),
m_arrow(NULL)
{

}

GameLevelDemoLayerController::~GameLevelDemoLayerController() {
	if (nullptr != m_layer && nullptr != m_eventLis)
		m_layer->getEventDispatcher()->removeEventListener(m_eventLis);
}

void GameLevelDemoLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_eventLis = EventListenerTouchOneByOne::create();
	m_eventLis->onTouchBegan = [this](Touch *touch ,Event *e)	
	{
		if (!isEnter()) {	
			//解除屏蔽底下的触摸事件
			if (nullptr != m_eventLis && m_eventLis->isSwallowTouches()) {
				m_eventLis->setSwallowTouches(false);
			}
			return false;
		}

		if (nullptr != touch) {
			Point pos = touch->getLocation();
			float diffx = abs(pos.x - m_maskPoint.x);
			float diffy = abs(pos.y - m_maskPoint.y);

			if (diffx <30 && diffy < 30) {
				return false;
			}
		}

		return true;	
	};

	m_eventLis->setSwallowTouches(false);
	m_layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_eventLis, m_layer);

}

Sprite*  GameLevelDemoLayerController::createMaskedSprite(const char* srcFile, const char* maskFile, const Point&maskPos) 
{ 
	if (nullptr == srcFile || nullptr == maskFile) {
		return nullptr;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	CCRenderTexture * rt = CCRenderTexture::create(size.width, size.height); 

	auto src = Sprite::create(srcFile);
	float scaleRate = size.height/src->getContentSize().height;
	src->setScaleY(scaleRate);
	src->setAnchorPoint(Vec2(0.5, 0));
	src->setPosition(size.width * 0.5, 0);
	ccBlendFunc blendFunc2 = { GL_ONE, GL_ZERO }; 
	src->setBlendFunc(blendFunc2);


	auto mask = Sprite::create(maskFile);
	mask->setAnchorPoint(Vec2(0.5, 0.5));
	mask->setPosition(maskPos);
	ccBlendFunc blendFunc3 = { GL_ZERO, GL_ONE_MINUS_SRC_ALPHA }; 
	mask->setBlendFunc(blendFunc3); 

	rt->begin(); 
	src->visit();
	mask->visit();
	rt->end(); 

	CCSprite * retval = CCSprite::createWithTexture(rt->getSprite()->getTexture()); 
	retval->setFlipY(true);
	return retval; 
} 


void GameLevelDemoLayerController::enter() {
	BottomLayerController::enter();

	//屏蔽底下的触摸事件
	if (nullptr != m_eventLis) {
		m_eventLis->setSwallowTouches(true);
	}
}

void GameLevelDemoLayerController::exit() {
	BottomLayerController::exit();
	stopArrowEffect();
}

void GameLevelDemoLayerController::enter(const Point&maskPoint) {
	if (nullptr == m_layer) {
		return;
	}

	m_maskPoint = maskPoint;
	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}

	m_mask = createMaskedSprite(s_bg_grey, "mask_80.png", maskPoint);
	m_layer->addChild(m_mask, MASK_Z_ORDER);
	m_mask->setAnchorPoint(Vec2(0.5, 0));
	m_mask->setPosition(Director::getInstance()->getVisibleSize().width * 0.5, 0);

	if (nullptr == m_arrow) {
		m_arrow = Sprite::createWithSpriteFrameName(s_icon_arrow2);
		m_layer->addChild(m_arrow, ARROW_Z_ORDER);
		m_arrow->setAnchorPoint(Vec2(0.5,0));
		
	}

	m_arrow->setPosition(maskPoint.x, maskPoint.y + 50);
	playArrowEffect();

	enter();
}

void GameLevelDemoLayerController::playArrowEffect() {
	if (nullptr == m_arrow) {
		return;
	}

	m_arrow->stopAllActions();
	m_arrow->runAction(RepeatForever::create(
		Sequence::create(
		MoveBy::create(0.3, Vec2(0, -10)),
		MoveBy::create(0.3, Vec2(0,10)),
		NULL
		)
		));
}

void GameLevelDemoLayerController::stopArrowEffect() {
	if (nullptr == m_arrow) {
		return;
	}

	m_arrow->stopAllActions();
}