#include "DemoPetMatrix.h"
#include "cocos2d.h"
#include "Resource.h"
#include "global.h"
USING_NS_CC;

#define BALL_INTRVAL 30
#define RING_TAG	100
#define TOUCH_TAG	101

DemoPetMatrix::DemoPetMatrix()
{

}

DemoPetMatrix::~DemoPetMatrix() {
	m_vLeftBalls.clear();
	m_vRightBalls.clear();
	m_vTopBalls.clear();
	m_vBottomBalls.clear();
	m_vMark.clear();
}

DemoPetMatrix *DemoPetMatrix::createPetMatrix(const std::vector<DemoPet>& demo) {
	DemoPetMatrix * matrix = DemoPetMatrix::create();
	if (nullptr != matrix) {
		matrix->createPets(demo);
	}

	return matrix;
}

DemoPetMatrix *DemoPetMatrix::createPetMatrix(const LevelData&levelData) {
	DemoPetMatrix * matrix = DemoPetMatrix::create();
	if (nullptr != matrix) {
		matrix->createPets(levelData);
	}

	return matrix;
}

void DemoPetMatrix::createPets(const LevelData&levelData) {
	return PetMatrix::createPets(levelData);
}

void DemoPetMatrix::createPets(const std::vector<DemoPet>& demo) {
	for (int i=0; i<demo.size(); i++) {
		DemoPet demoPet = demo.at(i);
		if (demoPet.iCol >=0 && demoPet.iCol<COL_NUM 
			&& demoPet.iRow >= 0 && demoPet.iRow <ROW_NUM) {
				int index = demoPet.iRow * COL_NUM + demoPet.iCol;
				createPet(index, demoPet.eColor, demoPet.eType);
		}
	}
}

void DemoPetMatrix::playLeftLinkEffect(int src, int dst, int row, bool bRed) {
	if (src <= dst || row <0) {
		return;
	} 

	float y = (ROW_NUM - 1 - row + 0.5) *Pet::getHeight();
	float x1 = (src + 0.5) *Pet::getWidth();
	float x2 = (dst + 1) *Pet::getWidth();

	clearBall(&m_vLeftBalls);

	float start = x1 - 30;
	while (start > x2 + 5) {
		auto ball = Sprite::createWithSpriteFrameName((bRed)?s_icon_ball_red : s_icon_ball_yellow);
		addChild(ball);
		ball->setPosition(start, y);
		m_vLeftBalls.pushBack(ball);
		start -= BALL_INTRVAL;
	}
}

void DemoPetMatrix::playRightLinkEffect(int src, int dst, int row, bool bRed) {
	if (src >= dst || row <0) {
		return;
	} 

	float y = (ROW_NUM - 1 - row + 0.5) *Pet::getHeight();
	float x1 = (src + 0.5) *Pet::getWidth();
	float x2 = (dst) *Pet::getWidth();

	clearBall(&m_vRightBalls);

	float start = x1 + 30;
	while (start < x2 - 5) {
		auto ball = Sprite::createWithSpriteFrameName((bRed)?s_icon_ball_red : s_icon_ball_yellow);
		addChild(ball);
		ball->setPosition(start, y);
		m_vRightBalls.pushBack(ball);
		start += BALL_INTRVAL;
	}
}

void DemoPetMatrix::playTopLinkEffect(int src, int dst, int col, bool bRed) {
	if (src <= dst || col <0) {
		return;
	} 

	float x = (col + 0.5) *Pet::getWidth();
	float y1 = (ROW_NUM - 1 - src + 0.5) *Pet::getHeight();
	float y2 = (ROW_NUM - 1 - dst) *Pet::getHeight();

	clearBall(&m_vTopBalls);

	float start = y1 + 30;
	while (start < y2 - 5) {
		auto ball = Sprite::createWithSpriteFrameName((bRed)?s_icon_ball_red : s_icon_ball_yellow);
		addChild(ball);
		ball->setPosition(x, start);
		m_vTopBalls.pushBack(ball);
		start += BALL_INTRVAL;
	}
}

void DemoPetMatrix::playBottomLinkEffect(int src, int dst, int col, bool bRed) {
	if (src >= dst || col <0) {
		return;
	} 

	float x = (col + 0.5) *Pet::getWidth();
	float y1 = (ROW_NUM - 1 - src + 0.5) *Pet::getHeight();
	float y2 = (ROW_NUM - 1 - dst + 1) *Pet::getHeight();

	clearBall(&m_vBottomBalls);

	float start = y1 - 30;
	while (start > y2 + 5) {
		auto ball = Sprite::createWithSpriteFrameName((bRed)?s_icon_ball_red : s_icon_ball_yellow);
		addChild(ball);
		ball->setPosition(x, start);
		m_vBottomBalls.pushBack(ball);
		start -= BALL_INTRVAL;
	}
}

void DemoPetMatrix::clearBall(cocos2d::Vector<cocos2d::Sprite *> * vBall) {
	if (nullptr == vBall) {
		return;
	}

	for (auto ball:*vBall) {
		ball->stopAllActions();
		removeChild(ball, true);
	}

	vBall->clear();
}

void DemoPetMatrix::playShineEffect(cocos2d::Vector<cocos2d::Sprite *> vBall) {
	float delayIntrval = 0.8;
	int i=0;
	for (auto ball:vBall) {
		ball->runAction(RepeatForever::create(
			Blink::create(0.5,1))
			);
	}
}

void DemoPetMatrix::playTouchEffect(int row, int col) {
	if (row < 0 || row >= ROW_NUM) {
		return;
	}

	if (col < 0 || col >= COL_NUM) {
		return;
	}

	removeChildByTag(RING_TAG);
	getParent()->removeChildByTag(TOUCH_TAG);

	const Point touchPos = Point((col + 0.5)*Pet::getWidth(), (ROW_NUM - 1 - row + 0.5)*Pet::getHeight());
	Animation *aniTouch = Animation::create();
	Sprite *touchSprite = nullptr;

	{
		Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_touch);
		if (nullptr == txt2d) {
			return;
		}

		float w = txt2d->getContentSize().width/s_ani_touch_n;
		float h = txt2d->getContentSize().height;


		aniTouch->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_touch_n; i++) {
			aniTouch->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		touchSprite = Sprite::create(s_ani_ring, Rect(0,0,w,h));
		touchSprite->setTag(TOUCH_TAG);
		getParent()->addChild(touchSprite, 12);
		touchSprite->setAnchorPoint(Vec2(0,0.5));
		
		touchSprite->setPosition(touchPos + getPosition());
	}

	Animation *aniRing = Animation::create();
	Sprite *ringSprite = nullptr;
	{
		Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_ring);
		if (nullptr == txt2d) {
			return;
		}

		float w = txt2d->getContentSize().width/s_ani_ring_n;
		float h = txt2d->getContentSize().height;


		aniRing->setDelayPerUnit(0.1);
		for (int i=0; i<s_ani_ring_n; i++) {
			aniRing->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		ringSprite = Sprite::create(s_ani_ring, Rect(0,0,w,h));
		ringSprite->setTag(RING_TAG);
		addChild(ringSprite, 10);
		ringSprite->setPosition(touchPos);
	}
	
	
	if (nullptr != touchSprite) {
		touchSprite->runAction(RepeatForever::create(
			Sequence::create(
				Animate::create(aniTouch),
				DelayTime::create(0.5)
				,NULL)
			));
	}

	if (nullptr != ringSprite) {
		ringSprite->runAction(RepeatForever::create(
			Sequence::create(
			DelayTime::create(0.4),
			Animate::create(aniRing),
			DelayTime::create(0.2)
			,NULL)
			));
	}
}

void DemoPetMatrix::stopTouchEffect() {
	auto ring = getChildByTag(RING_TAG);
	if (nullptr != ring) {
		ring->stopAllActions();
		removeChildByTag(RING_TAG);
	}

	auto touch = getParent()->getChildByTag(TOUCH_TAG);
	if (nullptr != touch) {
		touch->stopAllActions();
		getParent()->removeChildByTag(TOUCH_TAG);
	}
}

void DemoPetMatrix::markSprite(int row, int col, bool bYes) {
	if (row < 0 || row >= ROW_NUM) {
		return;
	}

	if (col < 0 || col >= COL_NUM) {
		return;
	}

	
	
	const Point markPos = Point((col + 0.5)*Pet::getWidth(), (ROW_NUM - 1 - row + 0.5)*Pet::getHeight());

	auto mark = Sprite::createWithSpriteFrameName((bYes)?s_mark_yes:s_mark_no);
	addChild(mark);
	mark->setAnchorPoint(Vec2(0.1, 0.6));
	mark->setPosition(markPos);
	m_vMark.pushBack(mark);
}

void DemoPetMatrix::clearMarkSprite() {
	for (int i=0; i<m_vMark.size(); i++) {
		auto mark = m_vMark.at(i);
		removeChild(mark);
	}

	m_vMark.clear();
}

void DemoPetMatrix::clearAllBall() {
	clearBall(&m_vLeftBalls);
	clearBall(&m_vRightBalls);
	clearBall(&m_vTopBalls);
	clearBall(&m_vBottomBalls);
}

bool DemoPetMatrix::getClearablePets(std::vector<PetVec2>&cross, std::vector<PetVec2>&clearable, std::vector<PetVec2>&unClearable, PetVec2&touchable, EnumPetSpecial special) {
	int clearNum = 0;
	int row = -1;
	int col	= -1;
	const int require = (special == Pet_Everyok)?1:2;

	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			if (nullptr == m_pets[i][j]) {
				int n = crossCheckNum(i, j, false);		
				if (n > clearNum && crossCheck(i, j, special) >= require) {
					row = i;
					col = j;
					clearNum = n;
				}
			}
		}
	}

	if (clearNum > 0 && row>=0 && col>=0) {
		touchable.iRow = row;
		touchable.iCol = col;
		getPetAround(row, col, cross, clearable, unClearable);
		return true;
	}

	return false;
}

void DemoPetMatrix::getPetAround(int row, int col, std::vector<PetVec2>&cross, std::vector<PetVec2>&clearable, std::vector<PetVec2>&unClearable) {
	if (row <0 || row >= ROW_NUM) {
		return;
	}

	if (col <0 || col >= COL_NUM) {
		return;
	}

	cross.clear();
	cross.push_back(PetVec2(row,col));
	clearable.clear();
	unClearable.clear();
	std::vector<PetVec2> temp;

	int emptySum = 0;
	//left
	for (int i=col-1; i>=0; i--) {
		auto pet = m_pets[row][i];
		cross.push_back(PetVec2(row,i));
		if (nullptr != pet) {
			emptySum = 0;
			temp.push_back(PetVec2(row,i));
			break;
		} else {
			emptySum++;
		}
	}

	while (emptySum-->0) {
		cross.pop_back();
	}

	emptySum = 0;
	//right
	for (int i=col+1; i<COL_NUM; i++) {
		auto pet = m_pets[row][i];
		cross.push_back(PetVec2(row,i));
		if (nullptr != pet) {
			emptySum = 0;
			temp.push_back(PetVec2(row,i));
			break;
		} else {
			emptySum++;
		}
	}

	while (emptySum-->0) {
		cross.pop_back();
	}

	emptySum = 0;
	//top
	for (int i=row-1; i>=0; i--) {
		auto pet = m_pets[i][col];
		cross.push_back(PetVec2(i,col));
		if (nullptr != pet) {
			emptySum = 0;
			temp.push_back(PetVec2(i,col));
			break;
		} else {
			emptySum++;
		}
	}

	while (emptySum-->0) {
		cross.pop_back();
	}

	emptySum = 0;
	//bottom
	for (int i=row+1; i<ROW_NUM; i++) {
		auto pet = m_pets[i][col];
		cross.push_back(PetVec2(i,col));
		if (nullptr != pet) {
			emptySum = 0;
			temp.push_back(PetVec2(i,col));
			break;
		} else {
			emptySum++;
		}
	}

	while (emptySum-->0) {
		cross.pop_back();
	}

	bool existEveryOk = false;
	for (int i = 0; i<temp.size(); i++) {
		if (temp.at(i).isValid()) {
			auto pet = m_pets[temp.at(i).iRow][temp.at(i).iCol];
			if (pet->isSpecialEveryok()) {
				existEveryOk = true;
				break;
			}
		}
	}

	if (existEveryOk) {
		clearable.clear();
		unClearable.clear();
		clearable.assign(temp.begin(), temp.end());
	} else {
		for (int i = 0; i<temp.size(); i++) {
			bool same = false;
			for (int j=0; j<temp.size(); j++) {
				if (i==j || !temp.at(i).isValid() || !temp.at(j).isValid()) {
					continue;
				}
				auto petA = m_pets[temp.at(i).iRow][temp.at(i).iCol];
				auto petB = m_pets[temp.at(j).iRow][temp.at(j).iCol];
				if (nullptr != petA && nullptr != petB && petA->getColor() == petB->getColor()) {
					clearable.push_back(temp.at(i));
					same = true;
					break;
				}
			}

			if (!same) {
				unClearable.push_back(temp.at(i));
			}
		}
	}
	

	temp.clear();
}

bool DemoPetMatrix::isVecContainItem(std::vector<PetVec2> &vPets, const PetVec2 &vPet) {
	if (vPets.empty()) {
		return false;
	}

	for (int i=0; i<vPets.size(); i++) {
		if (vPets.at(i).equal(vPet)) {
			return true;
		}
	}

	return false;
}