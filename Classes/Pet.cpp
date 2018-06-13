#include "Pet.h"
#include "Resource.h"
#include "PetSpecial.h"
#include "GameUtil.h"
USING_NS_CC;

#define ACTION_EYE_TAG		100
#define ACTION_JELLY_TAG	101

Pet::Pet():
m_color(Pet_Color_Undefine),
m_special(Pet_Normal),
m_row(-1),
m_col(-1),
m_selectSprite(NULL),
m_eyeOpen(NULL),
m_eyeClose(NULL)
{

}

Pet::~Pet() {

}

bool Pet::init() {
	if (!Sprite::init()) {
		return false;
	}

	setCascadeOpacityEnabled(true);
	return true;
}

void Pet::initSelectSprite() {
	m_selectSprite = Sprite::createWithSpriteFrameName(getPetSelImageByCol(m_color));
	addChild(m_selectSprite);
	m_selectSprite->setPosition(Pet::getWidth() * 0.5, Pet::getHeight() * 0.5);
	m_selectSprite->setVisible(false);
}

Pet *Pet::create(EnumPetColor color, EnumPetSpecial special) {
	Pet *pet = NULL;
	switch (special) {
		case Pet_Normal:
			pet = PetNormal::create(color);
			break;
		case Pet_Clear3x3:
			pet = PetClear::create(color);
			break;
		case Pet_Shade:
			pet = PetShade::create(color);
			break;
		case Pet_Face:
			pet = PetFace::create(color);
			break;
		case Pet_Ice:
			pet = PetIce::create(color);
			break;
		case Pet_Change:
			pet = PetChange::create(color);
			break;
		case Pet_Everyok:
			pet = PetEveryok::create(color);
			break;
	}

	return pet;
}

bool Pet::initSprite(EnumPetColor color) {
	if (!initWithSpriteFrameName(getPetImageByCol(color))) {
		return false;
	}

	m_color = color;
	m_eyeOpen = Sprite::createWithSpriteFrameName(getEyeOpenImageByCol(color));
	addChild(m_eyeOpen);
	m_eyeOpen->setPosition(Pet::getWidth() * 0.5, Pet::getHeight() * 0.6);

	m_eyeClose = Sprite::createWithSpriteFrameName(getEyeCloseImageByCol(color));
	addChild(m_eyeClose);
	m_eyeClose->setPosition(Pet::getWidth() * 0.5, Pet::getHeight() * 0.6);
	m_eyeClose->setVisible(false);

	return true;
}

const char * Pet::getPetImageByCol(EnumPetColor color) {
	switch (color) {
		case Pet_White:
			return s_pet_white;
		case Pet_Green:
			return s_pet_green;
		case Pet_Yellow:
			return s_pet_yellow;
		case Pet_Red:
			return s_pet_red;
		case Pet_Purple:
			return s_pet_purple;
		case Pet_Blue:
			return s_pet_blue;		
	}

	CCASSERT(false, "color error");
	return NULL;
}

const char * Pet::getPetSelImageByCol(EnumPetColor color) {
	switch (color) {
	case Pet_White:
		return s_pet_white_s;
	case Pet_Green:
		return s_pet_green_s;
	case Pet_Yellow:
		return s_pet_yellow_s;
	case Pet_Red:
		return s_pet_red_s;
	case Pet_Purple:
		return s_pet_purple_s;
	case Pet_Blue:
		return s_pet_blue_s;		
	}

	CCASSERT(false, "color error");
	return NULL;
}

const char * Pet::getPetImageBySpecial(EnumPetSpecial special) {
	switch (special) {
	case Pet_Clear3x3:
		return s_pet_white;	
	case Pet_Shade:
		return s_pet_white;
	case Pet_Face:
		return s_pet_white;
	case Pet_Ice:
		return s_pet_white;
	case Pet_Change:
		return s_pet_white;
	case Pet_Everyok:
		return s_pet_white;
		
	}

	CCASSERT(false, "color error");
	return NULL;
}

const char *Pet::getEyeOpenImageByCol(EnumPetColor color) {
	switch (color) {
	case Pet_White:
		return s_eye_white_o;
	case Pet_Green:
		return s_eye_green_o;
	case Pet_Yellow:
		return s_eye_yellow_o;
	case Pet_Red:
		return s_eye_red_o;
	case Pet_Purple:
		return s_eye_purple_o;
	case Pet_Blue:
		return s_eye_blue_o;		
	}

	CCASSERT(false, "color error");
	return NULL;
}

const char *Pet::getEyeCloseImageByCol(EnumPetColor color) {
	switch (color) {
	case Pet_White:
		return s_eye_white_c;
	case Pet_Green:
		return s_eye_green_c;
	case Pet_Yellow:
		return s_eye_yellow_c;
	case Pet_Red:
		return s_eye_red_c;
	case Pet_Purple:
		return s_eye_purple_c;
	case Pet_Blue:
		return s_eye_blue_c;		
	}

	CCASSERT(false, "color error");
	return NULL;
}

int Pet::getWidth() {
	return 63;
}

int Pet::getHeight() {
	return 63;
}

void Pet::select(bool selected) {
	if (nullptr == m_selectSprite) {
		return;
	}

	m_selectSprite->setVisible(selected);
}

void Pet::openEye() {
	if (nullptr != m_eyeOpen) {
		m_eyeOpen->setVisible(true);
	}

	if (nullptr != m_eyeClose) {
		m_eyeClose->setVisible(false);
	}
}

void Pet::closeEye() {
	if (nullptr != m_eyeOpen) {
		m_eyeOpen->setVisible(false);
	}

	if (nullptr != m_eyeClose) {
		m_eyeClose->setVisible(true);
	}
}

void Pet::changeEye() {
	if (nullptr != m_eyeOpen) {
		if (m_eyeOpen->isVisible()) {
			closeEye();
		} else {
			openEye();
		}
	}
}

void Pet::playEyeAction(float intrval) {
	stopActionByTag(ACTION_EYE_TAG);
	ActionInterval *action = RepeatForever::create(
											Sequence::create(
											CallFunc::create([=](){
												openEye();
												closeEye();}),
											DelayTime::create(0.4),
											CallFunc::create([=](){
												openEye();}),
											DelayTime::create(intrval),
											NULL)
											);
	action->setTag(ACTION_EYE_TAG);
	runAction(action);
}

void Pet::playJellyAction(float intrval) {
	stopActionByTag(ACTION_EYE_TAG);
	stopActionByTag(ACTION_JELLY_TAG);
	ActionInterval *eyeAction = Sequence::create(
		CallFunc::create([=](){
			openEye();
			closeEye();}),
		DelayTime::create(0.2),
		CallFunc::create([=](){
			openEye();}),NULL);
	GameUtil::petJump(this, 8, true, intrval, ACTION_JELLY_TAG, eyeAction);
}

void Pet::stopJellyAction() {
	stopActionByTag(ACTION_JELLY_TAG);
}