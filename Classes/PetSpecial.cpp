#include "cocos2d.h"
#include "PetSpecial.h"
#include "Resource.h"
#include "Resource.h"
#include "global.h"

USING_NS_CC;

#define FRAME_TAG			150
#define TAG_FRAME_ACTION	151

PetSpecial::PetSpecial()
{

}

PetSpecial::~PetSpecial() {

}

/************************************************************************/
/* Pet normal                                                                     */
/************************************************************************/
PetNormal::PetNormal():
m_face(NULL)
{

}

PetNormal::~PetNormal() {

}

bool PetNormal::initSprite(EnumPetColor color) {
	if (!Pet::initSprite(color)) {
		return false;
	}

	m_special = Pet_Normal;

	m_face = Sprite::createWithSpriteFrameName(s_special_face);
	addChild(m_face);
	m_face->setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);
	m_face->setVisible(false);

	initSelectSprite();
	return true;
}

bool PetNormal::init() {
	return Pet::init();
}

bool PetNormal::onClear() {
	playClearEffect();
	return true;
}

void PetNormal::changeToNormal() {
	if (NULL == m_face) {
		return;
	}

	m_special = Pet_Normal;
	m_face->setVisible(false);
}

void PetNormal::changeToFace() {
	if (NULL == m_face) {
		return;
	}

	m_special = Pet_Face;
	m_face->setVisible(true);
}

void PetNormal::playClearEffect() {

}

void PetNormal::playFrameEffect() {
	Texture2D * txt2d = nullptr;
	float w = 0;
	float h = 0;

	auto frame = getChildByTag(FRAME_TAG);
	if (nullptr != frame) {
		auto action = frame->getActionByTag(TAG_FRAME_ACTION);
		if (nullptr != action && !action->isDone()) {
			return;
		}
	} else {
		txt2d = TextureCache::getInstance()->addImage(s_ani_frame);
		if (nullptr == txt2d) {
			return;
		}

		w = txt2d->getContentSize().width/s_ani_frame_n;
		h = txt2d->getContentSize().height;

		frame = Sprite::create(s_ani_frame, Rect(0,0,w,h));
		addChild(frame);
		frame->setTag(FRAME_TAG);
	}

	
	if (nullptr == txt2d) {
		txt2d = TextureCache::getInstance()->addImage(s_ani_frame);
		if (nullptr == txt2d) {
			return;
		}

		w = txt2d->getContentSize().width/s_ani_frame_n;
		h = txt2d->getContentSize().height;
	}

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_frame_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	frame->setVisible(true);
	frame->setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);
	ActionInterval *frameAction = RepeatForever::create(Animate::create(ani));
	frameAction->setTag(TAG_FRAME_ACTION);
	frame->runAction(frameAction);
}

void PetNormal::stopFrameEffect() {
	auto frame = getChildByTag(FRAME_TAG);
	if (nullptr != frame) {
		frame->stopAllActions();
		frame->setVisible(false);
	}
}

/************************************************************************/
/* Pet clear                                                                   */
/************************************************************************/
PetClear::PetClear() {

}

PetClear::~PetClear() {

}

bool PetClear::initSprite(EnumPetColor color) {
	if (!initWithSpriteFrameName(s_special_clear)) {
		return false;
	}

	m_special = Pet_Clear3x3;
	return true;
}

bool PetClear::init() {
	return Pet::init();
}

bool PetClear::onClear() {
	playClearEffect();
	return true;
}

void PetClear::playClearEffect() {

}

/************************************************************************/
/* Pet shade                                                                   */
/************************************************************************/
PetShade::PetShade():
m_shade(NULL)
{

}

PetShade::~PetShade() {

}

bool PetShade::initSprite(EnumPetColor color) {
	if (!Pet::initSprite(color)) {
		return false;
	}

	m_special = Pet_Shade;

	m_shade = Sprite::createWithSpriteFrameName(s_special_shade);
	addChild(m_shade);
	m_shade->setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);

	initSelectSprite();
	return true;
}

bool PetShade::init() {
	if (! Pet::init()) {
		return false;
	}
	
	return true;
}

bool PetShade::onClear() {
	if (m_shade->isVisible()) {
		m_shade->setVisible(false);
		return false;
	}
	
	return true;
}

void PetShade::playClearEffect() {
}

bool PetShade::hasShade() {
	if (nullptr != m_shade && m_shade->isVisible()) {
		return true;
	}

	return false;
}

/************************************************************************/
/* Pet face                                                                   */
/************************************************************************/
PetFace::PetFace():
m_face(NULL)
{

}

PetFace::~PetFace() {

}

bool PetFace::initSprite(EnumPetColor color) {
	if (!Pet::initSprite(color)) {
		return false;
	}

	m_special = Pet_Face;

	m_face = Sprite::createWithSpriteFrameName(s_special_face);
	addChild(m_face);
	m_face->setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);
	m_face->setVisible(true);

	initSelectSprite();
	return true;
}

bool PetFace::init() {
	if (! Pet::init()) {
		return false;
	}

	return true;
}

bool PetFace::onClear() {
	playClearEffect();
	return true;
}

void PetFace::changeToNormal() {
	if (NULL == m_face) {
		return;
	}

	m_special = Pet_Normal;
	m_face->setVisible(false);
}

void PetFace::changeToFace() {
	if (NULL == m_face) {
		return;
	}

	m_special = Pet_Face;
	m_face->setVisible(true);
}

void PetFace::playClearEffect() {
}

/************************************************************************/
/* Pet ice                                                                   */
/************************************************************************/
PetIce::PetIce():
m_ice(NULL),
m_iceClosing(NULL),
m_closed(false)
{

}

PetIce::~PetIce() {

}

bool PetIce::initSprite(EnumPetColor color) {
	if (!Pet::initSprite(color)) {
		return false;
	}

	m_special = Pet_Ice;

	m_ice = Sprite::createWithSpriteFrameName(s_special_ice_closed);
	addChild(m_ice);
	m_ice->setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);
	m_ice->setVisible(false);

	m_iceClosing = Sprite::createWithSpriteFrameName(s_special_ice_closing);
	addChild(m_iceClosing);
	m_iceClosing->setPosition(getContentSize().width * 0.5, getContentSize().height * 0.5);
	m_iceClosing->setVisible(false);

	initSelectSprite();
	return true;
}

bool PetIce::init() {
	if (! Pet::init()) {
		return false;
	}

	return true;
}

bool PetIce::onClear() {
	playClearEffect();
	return true;
}


void PetIce::playClearEffect() {
}

void PetIce::changeIce() {
	if (m_closed) {
		m_closed = false;
		playUncloseEffect();
	} else {
		m_closed = true;
		playClosingEffect();
	}
}

void PetIce::playClosingEffect() {
	if (NULL == m_iceClosing || NULL == m_ice) {
		return;
	} 

	m_iceClosing->setOpacity(255);
	m_iceClosing->setVisible(true);

	ActionInterval *action = Spawn::create(
		ScaleTo::create(0.5, 0.5, 0.5, 1),
		FadeOut::create(0.5),
		NULL);

	m_iceClosing->runAction(Sequence::create(
		action,
		CallFunc::create(
			[=]() {
				m_ice->setVisible(true);
				m_ice->runAction(FadeIn::create(0.2));
			}
		),
		NULL));
}

void PetIce::playUncloseEffect() {
	if (NULL == m_iceClosing || NULL == m_ice) {
		return;
	}

	/*
	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_ice);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_ice_n;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.08);
	for (int i=0; i<s_ani_ice_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}*/

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.08);
	for (int i=0; i<10; i++) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("door_%d.png",i)->_string);
		ani->addSpriteFrame(frame);
	}

	Sprite * sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("door_0.png"));
	this->getParent()->addChild(sprite,200);
	sprite->setPosition(getPositionX(), getPositionY());
	sprite->setScale(0.6);
	sprite->runAction(Sequence::create(
		Spawn::create(
			Animate::create(ani),
			CallFunc::create(
				[=](){
				m_ice->setVisible(false);
				}),
			NULL
		),
		
		CallFunc::create([=](){	
			this->getParent()->removeChild(sprite,true);
			
		}),
		NULL));
}

/************************************************************************/
/* Pet change                                                                   */
/************************************************************************/
PetChange::PetChange():
	m_rollSprite(NULL)
{

}

PetChange::~PetChange() {

}

bool PetChange::initSprite(EnumPetColor color) {
	if (!Pet::initSprite(color)) {
		return false;
	}

	m_special = Pet_Change;

	initSelectSprite();
	return true;
}

bool PetChange::init() {
	if (! Pet::init()) {
		return false;
	}

	return true;
}

bool PetChange::onClear() {
	if (NULL != m_rollSprite) {
		m_rollSprite->stopAllActions();
		m_rollSprite->removeFromParentAndCleanup(true);
		m_rollSprite = NULL;
	}
	
	playClearEffect();
	return true;
}

void PetChange::changeColor(EnumPetColor color) {
	setSpriteFrame(Pet::getPetImageByCol(color));
	m_color = color;
}

void PetChange::playClearEffect() {
}

void PetChange::playRollEffect() {
	/*
	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_change);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_change_n;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i=0; i<s_ani_change_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}*/

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i=0; i<16; i++) {
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("color_%d.png",i)->_string);
		ani->addSpriteFrame(frame);
	}

	if (NULL == m_rollSprite) {
		m_rollSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("color_0.png"));
		this->getParent()->addChild(m_rollSprite,200);
		m_rollSprite->setPosition(getPosition());
		m_rollSprite->setScale(0.7);
	}
	
	ActionInterval * action = RepeatForever::create(
		Animate::create(ani)
		);

	m_rollSprite->runAction(action);
}

/************************************************************************/
/* Pet everyok                                                                  */
/************************************************************************/
PetEveryok::PetEveryok()
{

}

PetEveryok::~PetEveryok() {

}

bool PetEveryok::initSprite(EnumPetColor color) {
	if (!initWithSpriteFrameName(s_special_everyok)) {
		return false;
	}

	m_special = Pet_Everyok;
	return true;
}

bool PetEveryok::init() {
	return Pet::init();
}

bool PetEveryok::onClear() {
	return true;
}
