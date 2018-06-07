#include "BgLayerController.h"
#include "cocos2d.h"
#include "Resource.h"
#include "prefix.h"

#define BG_Z_ORDER				-10
#define GROUND_Z_ORDER			-9
#define LAYER_TOP_Z_ORDER		-8
#define LAYER_BOTTOM_Z_ORDER	-7


BgLayer::BgLayer():
m_bg(NULL),
m_bgGround(NULL),
m_top(NULL),
m_bottom(NULL)
{

}

BgLayer::~BgLayer() {

}

bool BgLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	initBg();
	
	const Size size = Director::getInstance()->getVisibleSize();
	m_top		= CSLoader::createNode("GameTopLayer.csb");
	addChild(m_top, LAYER_TOP_Z_ORDER);	
	m_top->ignoreAnchorPointForPosition(false);
	m_top->setAnchorPoint(Vec2(0.5, 1));
	m_top->setPosition(size.width * 0.5, size.height);

	m_bottom	= CSLoader::createNode("GameBottomLayer.csb");
	m_bottom->ignoreAnchorPointForPosition(false);
	m_bottom->setAnchorPoint(Vec2(0.5, 0));
	m_bottom->setPosition(size.width * 0.5, 0);
	addChild(m_bottom, LAYER_BOTTOM_Z_ORDER);
	return true;
}

void BgLayer::initBg() {
	const Size size = Director::getInstance()->getVisibleSize();
	m_bg = Sprite::create(s_bg_game);
	addChild(m_bg, BG_Z_ORDER);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	m_bgGround = Sprite::create(s_bg_ground);
	addChild(m_bgGround, GROUND_Z_ORDER);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create(s_bg_tree);
	addChild(tree);
	tree->setAnchorPoint(Vec2(1,0.5));
	tree->setPosition(size.width, 120);
}


BgLayerController::BgLayerController()
{

}

BgLayerController::~BgLayerController() {

}

void BgLayerController::initial() {

}