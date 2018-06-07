#include "PetMatrix.h"
#include "pet.h"
#include "PetSpecial.h"
#include "Resource.h"
#include "global.h"
#include "GameManager.h"
#include "global.h"
#include "GameData.h"
#include "Audio.h"
#include "GameUtil.h"
#include "HitManager.h"
#include "GameEvent.h"

USING_NS_CC;

#define MAX_BOMB_CLEAR_NUM	8

#define TIP_CLICK_Z_ORDER	11
#define CLEAR_Z_ORDER	10
#define FRAME_Z_ORDER	9
#define CROSS_Z_ORDER	8	
#define TIP_Z_ORDER		7

#define EFFECT_TOUCH_Z_ORDER	6
#define EFFECT_LINK_Z_ORDER		5

const int PetMatrix::GRID_NUM = ROW_NUM * COL_NUM;

PetMatrix::PetMatrix():
m_facePetNum(0),
m_tipRing(NULL),
m_bClearAll(false),
m_tipTouch(NULL)
{

}

PetMatrix::~PetMatrix() {
	
}

bool PetMatrix::init() {
	if (!Layer::init()) {
		return false;
	}

	initPetMatirx();
	return true;
}

void PetMatrix::initPetMatirx() {
	for (int i=0; i<ROW_NUM; i++) {
		for(int j=0; j<COL_NUM; j++) {
			m_pets[i][j] = nullptr;
		}
	}
}

PetMatrix *PetMatrix::createPetMatrix(const LevelData&levelData) {
	PetMatrix * matrix = PetMatrix::create();
	if (nullptr != matrix) {
		matrix->createPets(levelData);
	}

	return matrix;
}

void PetMatrix::createPets(const LevelData&levelData) {
	//随机数引擎
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> pet3ColorDis(0, 2);
	std::uniform_int_distribution<int> pet4ColorDis(0, 3);
	std::uniform_int_distribution<int> pet5ColorDis(0, 4);
	std::uniform_int_distribution<int> petAllColorDis(0, Max_Pet_Color - 1);


	std::uniform_int_distribution<int> * petColorDis = nullptr;
	if (levelData.iLevel < 3) {
		petColorDis = &pet3ColorDis;
	} else if (levelData.iLevel < 6) {
		petColorDis = &pet4ColorDis;
	} else if (levelData.iLevel < 9) {
		petColorDis = &pet5ColorDis;
	} else {
		petColorDis = &petAllColorDis;
	}

	m_facePetNum = levelData.iFace;
	// initialize numbers.
	std::vector<int> gridIndex;
	for (int i=0; i<GRID_NUM; i++) {
		gridIndex.push_back(i);
	}

	// do random shuffle.
	std::shuffle(gridIndex.begin(), gridIndex.end(), mt);
	
	int k=0;
	//clear
	for (int i=0; i<levelData.iClear3x3; i++) {
		int index = gridIndex[k++];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Clear3x3);
	}
	//shade
	for (int i=0; i<levelData.iShade; i++) {
		int index = gridIndex[k++];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Shade);
	}
	//face
	for (int i=0; i<levelData.iFace; i++) {
		int index = gridIndex[k++];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Face);
	}
	//ice
	for (int i=0; i<levelData.iIce; i++) {
		int index = gridIndex[k++];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Ice);
	}
	//change
	for (int i=0; i<levelData.iChange; i++) {
		int index = gridIndex[k++];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Change);
	}
	//everyok
	for (int i=0; i<levelData.iEveryok; i++) {
		int index = gridIndex[k++];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Everyok);
	}
	//normal
	for (; k<MAX_PET; k++) {
		int index = gridIndex[k];
		int color = (*petColorDis)(mt);
		createPet(index, color, Pet_Normal);
	}
}

void PetMatrix::createPet(int index, int color, EnumPetSpecial special) {
	int row = index/COL_NUM;
	int col = index%COL_NUM;

	Pet * pet = Pet::create((EnumPetColor)color, special);
	pet->setPos(row, col);
	pet->setAnchorPoint(Point(0.5,0.5));
	addChild(pet);
	
	pet->setPosition((col + 0.5) * Pet::getWidth(), (ROW_NUM - 1 - row + 0.5) * Pet::getHeight());
	m_pets[row][col] = pet;

	PetSpecial * petSpecial = dynamic_cast<PetSpecial *>(pet);
	if (NULL != petSpecial) {
		petSpecial->playRollEffect();
	}

	if (pet->isSpecialNormal()) {
		bool eyeAction = GameUtil::getRandomBoolean();
		float intrval = GameUtil::getRandomNum(3,8);
		float delay = GameUtil::getRandomNum(0,10) * 0.1;
		if (eyeAction) {
			pet->runAction(Sequence::create(
				DelayTime::create(delay),
				CallFunc::create([=](){
					pet->playEyeAction(intrval);
				}),
				NULL));
		} else {
			pet->runAction(Sequence::create(
				DelayTime::create(delay),
				CallFunc::create([=](){
					pet->playJellyAction(intrval);
			}),
				NULL));
		}
	}
}

void PetMatrix::enter() {
	const Size size = Director::getInstance()->getVisibleSize();
	int petSoundCount = 0;
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			auto pet = m_pets[i][j];
			if (nullptr != pet) {
				if (pet->isSpecialNormal()) {
					const float originY = pet->getPositionY();
					const float delay = 0.1f * GameUtil::getRandomNum(1,5);
					pet->setPositionY(originY + size.height);
					pet->runAction(Sequence::create(
						DelayTime::create(delay),
						CallFunc::create([=](){ Audio::getInstance()->playFall(); }),
						EaseSineIn::create(MoveBy::create(0.5, Vec2(0, -size.height))),
						CallFunc::create([=](){ 
							GameUtil::jelly(pet); 
							if ((petSoundCount)%8 == 0) {
								Audio::getInstance()->playFall2();
							}
						}),
						NULL));
					petSoundCount++;
				} else {
					pet->setOpacity(0);
					pet->runAction(Sequence::create(
						FadeIn::create(0.5),
						NULL));
				}
				
			}
		}
	}

	runAction(Sequence::create(
		DelayTime::create(1),
		CallFunc::create([=](){ Audio::getInstance()->playFall3(); }),
		NULL));
}

bool PetMatrix::checkPets() {
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			auto pet = m_pets[i][j];
			if (nullptr == pet && crossCheck(i,j,false)) {
				return true;
			}
		}
	}

	return false;
}

PetVec2 PetMatrix::getTouchPetVec2(const cocos2d::Point&pos) {
	const Point localPos = convertToNodeSpace(pos);
	const int row = ROW_NUM - localPos.y / Pet::getHeight();
	const int col = localPos.x / Pet::getWidth();
	return PetVec2(row, col);
}

bool PetMatrix::onTouch(const Point&pos, bool&err) {
	clearTipEffect();
	err = false;
	const Point localPos = convertToNodeSpace(pos);
	const int row = ROW_NUM - localPos.y / Pet::getHeight();
	const int col = localPos.x / Pet::getWidth();

	bool valid = false;
	if (GameManager::getInstance()->isCrossSelected()) {
		valid = crossClear(row, col);
		if (valid) {
			GameManager::getInstance()->selectCross(false);
		}
	} else {
		valid = crossCheck(row, col, true);
	}

	if (!valid && nullptr == m_pets[row][col]) {
		GameData::getInstance()->addTime(-5);	
		playTouchErrorEffect(row, col);
		err = true;
	}
	
	changeFace();
	updatePets();

	return valid;
}

bool PetMatrix::crossClear(const int row, const int col) {
	if (NULL == m_pets || row >= ROW_NUM || col >= COL_NUM || row < 0 || col < 0) {
		return false;
	}

	if (nullptr != m_pets[row][col]) {
		return false;
	}

	//消除行
	int scoreSum = 0;
	for (int i=0; i<COL_NUM; i++) {
		Pet *pet = m_pets[row][i];
		if (nullptr != pet && !pet->isSpecialIce()) {
			PetSpecial * special = dynamic_cast<PetSpecial *>(pet);
			if (!special->hasShade()) {
				scoreSum += s_score_pet;
			}
			clearPet(pet);		
		}
	}

	//消除列
	for (int i=0; i<ROW_NUM; i++) {
		Pet *pet = m_pets[i][col];
		if (nullptr != pet && !pet->isSpecialIce()) {
			PetSpecial * special = dynamic_cast<PetSpecial *>(pet);
			if (!special->hasShade()) {
				scoreSum += s_score_pet;
			}
			clearPet(pet);
		}
	}

	//播放特效
	playCross2Effect(row, col, scoreSum);
	return true;
}

bool PetMatrix::crossCheck(const int row, const int col, bool clear) {
	return (crossCheckNum(row, col, clear) > 1);
}

int PetMatrix::crossCheckNum(const int row, const int col, bool clear) {
	if (NULL == m_pets || row >= ROW_NUM || col >= COL_NUM || row < 0 || col < 0) {
		return 0;
	}

	if (nullptr != m_pets[row][col]) {
		return 0;
	}

	int clearSum = 0;

	Vector<Pet *> vPetA(4);
	Vector<Pet *> vPetB(4);
	Vector<Pet *> vPetC(4);

	bool bExistEveryok = false;

	Pet *bottom = nullptr;
	for (int i=row+1; i<ROW_NUM; i++) {
		if (nullptr != m_pets[i][col]) {
			bottom = m_pets[i][col];
			vPetA.pushBack(bottom);
			if (bottom->isSpecialEveryok()) {
				bExistEveryok = true;
			}
			break;
		}
	}

	Pet *top = nullptr;
	for (int i=row-1; i>=0; i--) {
		if (nullptr != m_pets[i][col]) {
			top = m_pets[i][col];
			if (!petCheck(vPetA, top)) {
				petCheck(vPetB, top);
			}
			if (top->isSpecialEveryok()) {
				bExistEveryok = true;
			}


			break;
		}
	}

	Pet *left = nullptr;
	for (int i=col-1; i>=0; i--) {
		if (nullptr != m_pets[row][i]) {
			left = m_pets[row][i];
			if (!petCheck(vPetA, left) && !petCheck(vPetB, left)) {
				petCheck(vPetC, left);
			}
			if (left->isSpecialEveryok()) {
				bExistEveryok = true;
			}


			break;
		}
	}

	Pet *right = nullptr;
	for (int i=col+1; i<COL_NUM; i++) {
		if (nullptr != m_pets[row][i]) {
			right = m_pets[row][i];
			if (!petCheck(vPetA, right) && !petCheck(vPetB, right)) {
				petCheck(vPetC, right);
			}
			if (right->isSpecialEveryok()) {
				bExistEveryok = true;
			}


			break;
		}
	}

	if (bExistEveryok) {
		vPetA.clear();
		vPetB.clear();
		vPetC.clear();

		if (NULL != bottom) {
			vPetA.pushBack(bottom);
		}
		if (NULL != top) {
			vPetA.pushBack(top);
		}
		if (NULL != left) {
			vPetA.pushBack(left);
		}
		if (NULL != right) {
			vPetA.pushBack(right);
		}

		clearSum += vPetA.size();
		
		if (clear) {	
			HitManager::getInstance()->hit();
			clearSamePet(vPetA, row, col);
			vPetA.clear();
		}

		return clearSum;
	} else {
		if (clear) {
			if (vPetA.size() > 1 || vPetB.size() > 1 || vPetC.size() > 1) {
				HitManager::getInstance()->hit();
			}

			clearSum += clearSamePet(vPetA, row, col) + clearSamePet(vPetB, row, col) + clearSamePet(vPetC, row, col);
		} else {
			if (vPetA.size() >= 2) {
				clearSum += vPetA.size();
			}

			if (vPetB.size() >= 2) {
				clearSum += vPetB.size();
			}

			if (vPetC.size() >= 2) {
				clearSum += vPetC.size();
			}
		}

		vPetA.clear();
		vPetB.clear();
		vPetC.clear();
		return clearSum;
	}
}

int PetMatrix::crossCheck(const int row, const int col, EnumPetSpecial special) {
	int petCount = 0;
	if (NULL == m_pets || row >= ROW_NUM || col >= COL_NUM || row < 0 || col < 0) {
		return 0;
	}

	if (nullptr != m_pets[row][col]) {
		return 0;
	}

	Pet *bottom = nullptr;
	for (int i=row+1; i<ROW_NUM; i++) {
		if (nullptr != m_pets[i][col]) {
			bottom = m_pets[i][col];
			if (bottom->getSpecial() == special) {
				petCount++;
			}
			break;
		}
	}

	Pet *top = nullptr;
	for (int i=row-1; i>=0; i--) {
		if (nullptr != m_pets[i][col]) {
			top = m_pets[i][col];
			if (top->getSpecial() == special) {
				petCount++;
			}
			break;
		}
	}

	Pet *left = nullptr;
	for (int i=col-1; i>=0; i--) {
		if (nullptr != m_pets[row][i]) {
			left = m_pets[row][i];		
			if (left->getSpecial() == special) {
				petCount++;
			}
			break;
		}
	}

	Pet *right = nullptr;
	for (int i=col+1; i<COL_NUM; i++) {
		if (nullptr != m_pets[row][i]) {
			right = m_pets[row][i];
			if (right->getSpecial() == special) {
				petCount++;
			}
			break;
		}
	}

	return petCount;
}

bool PetMatrix::petCheck(cocos2d::Vector<Pet*>&vPets, Pet *pet) {
	if (nullptr == pet) {
		return false;
	}

	if (vPets.size() <= 0) {
		vPets.pushBack(pet);
		return true;
	}

	for (auto item : vPets)
	{
		if (nullptr != item 
			&& (Pet_Color_Undefine != item->getColor() && item->getColor() == pet->getColor()
			|| (item->isSpecialClear3x3() && pet->isSpecialClear3x3()))) {
			vPets.pushBack(pet);
			return true;
		}
	}

	return false;
}

int PetMatrix::clearSamePet(cocos2d::Vector<Pet*>&vPets, int row, int col) {
	if (vPets.size() <= 1) {
		return 0;
	}

	const int hitCount = HitManager::getInstance()->getHit();
	Audio::getInstance()->playClearPet(hitCount);

	int score_pet = s_score_pet;
	if (4 == hitCount) {
		score_pet = 6;
	} else if (5 == hitCount) {
		score_pet = 7;
	} else if (6 == hitCount) {
		score_pet = 8;
	} else if (hitCount >= 7) {
		score_pet = 8;
	}

	int clearSum = 0;
	int addScore = 0;
	for (auto item : vPets)
	{
		if (nullptr != item) {
			clearPet(item, score_pet, (hitCount>=3));
			playTouchEffect(row, col);
			playLinkEffect(row, col, item->getPosRow(), item->getPosCol());
			clearSum++;
			addScore += score_pet;
			if (item->isSpecialClear3x3() || hitCount >= 4) {
				int num = clear3x3Pet(item);
				clearSum += num;
				addScore += num * s_score_pet;
			}
		}
	}

	vPets.clear();

	if (hitCount > 1) {
		const Point pos = Point((col + 0.5)*Pet::getWidth(), (ROW_NUM - 1 - row + 0.5) *Pet::getHeight());
		playContinueClickEffect(pos,hitCount);
	}

	if (hitCount >= 3) {
		playFrameEffect();
	} else {
		stopFrameEffect();
	}

	if (addScore > 0) {
		GameManager::getInstance()->addScore(addScore);	
	}

	return clearSum;
}

void PetMatrix::clearPet(Pet *pet, int petScore, bool bombEffect) {
	if (nullptr == pet) {
		return;
	}

	PetSpecial * special = dynamic_cast<PetSpecial*>(pet);
	if (NULL == special) {
		CCASSERT(false,"dynamic_cast petspecial error");
		return;
	}

	if (special->onClear()) {
		m_pets[pet->getPosRow()][pet->getPosCol()] = nullptr;
		if (bombEffect) {
			playBombEffect(pet, true);
			pet->runAction(FadeOut::create(0.5));
		} else {
			playClearEffect(pet, true);
		}	
	} else if (pet->isSpecialShade()){
		if (bombEffect) {
			playBombEffect(pet, false);
		} else {
			playClearEffect(pet, false);
		}
	}

	if (pet->isSpecialFace()) {
		m_facePetNum--;
	}

	const Point pos(pet->getPositionX(), pet->getPositionY() + pet->getContentSize().height * 0.5 + 10);
	if (GameData::getInstance()->getBossType() > 0) {
		playAttrackBossEffect(pos, petScore);
	} else {
		playAddScoreEffect(pos, petScore);
	}
}

int PetMatrix::clear3x3Pet(Pet *pet) {
	if (nullptr == pet) {
		return 0;
	}

	//Audio::getInstance()->playClear(4);
	Audio::getInstance()->playBomb();

	const int row = pet->getPosRow();
	const int col = pet->getPosCol();

	int clearPetNum = 0;
	//左上
	if (row-1>=0 && col-1>=0) {
		Pet * item = m_pets[row-1][col-1];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}	
	}

	//中上
	if (row-1>=0 && col>=0) {
		Pet * item = m_pets[row-1][col];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	//右上
	if (row-1>=0 && col+1<COL_NUM) {
		Pet * item = m_pets[row-1][col+1];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	//左中
	if (row>=0 && col-1>=0) {
		Pet * item = m_pets[row][col-1];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	//右中
	if (row>=0 && col+1<COL_NUM) {
		Pet * item = m_pets[row][col+1];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	//左下
	if (row+1<ROW_NUM && col-1>=0) {
		Pet * item = m_pets[row+1][col-1];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	//中下
	if (row+1<ROW_NUM && col>=0) {
		Pet * item = m_pets[row+1][col];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	//右下
	if (row+1<ROW_NUM && col+1<COL_NUM) {
		Pet * item = m_pets[row+1][col+1];
		if (NULL != item && item->isVisible()) {
			clearPet(item, s_score_pet, true);
			clearPetNum++;
		}
	}

	return clearPetNum;
}

void PetMatrix::clearAllPet() {
	if (m_bClearAll) {
		return;
	}

	m_bClearAll = true;
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			auto pet = m_pets[i][j];
			if (nullptr != pet) {
				Audio::getInstance()->playBreak();
			}
			playClear2Effect(pet);	
		}
	}
}

void PetMatrix::playClearEffect(Pet *pet, bool clear) {
	if (NULL == pet) {
		return;
	}

	if (pet->isSpecialNormal()) {
		pet->select(true);
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_clear);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_clear_n;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_clear_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	Sprite * sprite = Sprite::create(s_ani_clear, Rect(0,0,w,h));
	addChild(sprite, CLEAR_Z_ORDER);
	sprite->setPosition(pet->getPositionX(),
							pet->getPositionY());
	sprite->setVisible(false);
	sprite->runAction(Sequence::create(
		DelayTime::create(0.2),
		CallFunc::create([=](){
			sprite->setVisible(true);
		}),
		Spawn::create(
			Animate::create(ani),
			CallFunc::create([=](){
				if (clear) {
					pet->setVisible(false);
				}
			}),
			NULL
		),	
		CallFunc::create([=](){	
			removeChild(sprite,true);
		}),
		NULL));
}

void PetMatrix::playClear2Effect(Pet *pet, bool clear) {
	if (NULL == pet) {
		return;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_clear2);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_clear2_n;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_clear2_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	Sprite * sprite = Sprite::create(s_ani_clear2, Rect(0,0,w,h));
	addChild(sprite, CLEAR_Z_ORDER);
	sprite->setPosition(pet->getPositionX(),
		pet->getPositionY());

	sprite->runAction(Sequence::create(
		Spawn::create(
		Animate::create(ani),
		CallFunc::create([=](){
			if (clear) {
				pet->runAction(
					Sequence::create(
						FadeOut::create(0.5),
						CallFunc::create([=](){
							pet->setVisible(false);
						}),
						NULL));
			}
	}),
		NULL
		),	
		CallFunc::create([=](){	
			removeChild(sprite,true);
	}),
		NULL));
}

void PetMatrix::playBombEffect(Pet *pet, bool clear) {
	if (NULL == pet) {
		return;
	}

	//播放特效声音
	Audio::getInstance()->playBomb();
	Audio::getInstance()->playClear(4);

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_bomb);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_bomb_w_n;
	float h = txt2d->getContentSize().height/s_ani_bomb_h_n;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.03);
	for (int i=0; i<s_ani_bomb_w_n; i++) {
		for (int j=0; j<s_ani_bomb_h_n; j++)
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, i*h, w, h));
	}

	Sprite * sprite = Sprite::create(s_ani_bomb, Rect(0,0,w,h));
	addChild(sprite, CLEAR_Z_ORDER);
	sprite->setPosition(pet->getPositionX(),
		pet->getPositionY());

	sprite->runAction(Sequence::create(
		Animate::create(ani),
		CallFunc::create([=](){	
			if (clear) {
				pet->setVisible(false);
			}
			removeChild(sprite,true);
		}),
		NULL));
}

void PetMatrix::updatePets() {
	
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			Pet * pet = m_pets[i][j];
			if (NULL != pet && pet->isVisible()) {
				//change ice
				if (pet->isSpecialIce()) {
					PetSpecial * special = dynamic_cast<PetSpecial *>(pet);
					if (NULL != special) {
						special->changeIce();
					} else {
						CCASSERT(false, "pet error");
					}
				}
				//change color
				if (pet->isSpecialChange()) {
					PetSpecial * special = dynamic_cast<PetSpecial *>(pet);
					if (NULL != special) {
						int color = pet->getColor();
						color = (color + 1)%Max_Pet_Color;
						special->changeColor((EnumPetColor)color);
					} else {
						CCASSERT(false, "pet error");
					}
				}
			}
		}
	}
}

void PetMatrix::changeFace() {
	if (m_facePetNum < 0) {
		CCASSERT(false, "face pet num can't less 0");
		return;
	}

	if (0 == m_facePetNum) {
		return;
	}

	Vector<Pet *> vNormalPets;
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			Pet * pet = m_pets[i][j];
			if (NULL != pet && pet->isVisible()) {
				if (pet->isSpecialFace()) {
					PetSpecial * special = dynamic_cast<PetSpecial *>(pet);
					if (NULL != special) {
						special->changeToNormal();
					} else {
						CCASSERT(false, "pet error");
					}
				} else if (pet->isSpecialNormal()) {
					vNormalPets.pushBack(pet);
				}
			}
		}
	}

	if (vNormalPets.size() > 0) {
		std::random_device rd;
		std::mt19937 mt(rd());

		// do random shuffle.
		std::shuffle(vNormalPets.begin(), vNormalPets.end(), mt);

		for (int i=0; i<m_facePetNum && i<vNormalPets.size(); i++) {
			Pet * pet = vNormalPets.at(i);
			PetSpecial * special = dynamic_cast<PetSpecial *>(pet);
			if (NULL != special) {
				special->changeToFace();
			} else {
				CCASSERT(false, "pet error");
			}
		}

		vNormalPets.clear();
	}
}

void PetMatrix::playCrossEffect(int row, int col, int score) {
	if (score > 0) {
		GameManager::getInstance()->addScore(score);
	}	
	//播放特效声音
	Audio::getInstance()->playCross();
	Audio::getInstance()->playClear(4);

	Point pos;
	pos.y = (ROW_NUM - 1 - row + 0.5) * Pet::getHeight();
	pos.x = (col + 0.5) * Pet::getWidth();

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_cross);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_cross_n;
	float h = txt2d->getContentSize().height;

	Animation * aniCol = Animation::create();
	aniCol->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_cross_n; i++) {
		aniCol->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	Animation * aniRow = Animation::create();
	aniRow->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_cross_n; i++) {
		aniRow->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	Sprite * spriteCol = Sprite::create(s_ani_cross, Rect(0,0,w,h));
	addChild(spriteCol, CROSS_Z_ORDER);
	spriteCol->setPosition(pos.x,  Pet::getHeight() * ROW_NUM * 0.5);

	Sprite * spriteRow = Sprite::create(s_ani_cross, Rect(0,0,w,h));
	addChild(spriteRow, CROSS_Z_ORDER);
	spriteRow->setRotation(90);
	spriteRow->setPosition(getContentSize().width * 0.5,  pos.y);

	spriteCol->runAction(Sequence::create(
		Animate::create(aniCol),
		CallFunc::create([=](){
			removeChild(spriteCol);	
		}),
		NULL));

	spriteRow->runAction(Sequence::create(
			Animate::create(aniRow),
			CallFunc::create([=](){
				removeChild(spriteRow);
		}),
			NULL));
}

void PetMatrix::playCross2Effect(int row, int col, int score) {
	//先加分 再播特效
	if (score > 0) {
		GameManager::getInstance()->addScore(score);
	}
	//播放特效声音
	Audio::getInstance()->playCross();
	Audio::getInstance()->playClear(4);
	const Size size = Director::getInstance()->getVisibleSize();

	Point pos;
	pos.y = (ROW_NUM - 1 - row + 0.5) * Pet::getHeight();
	pos.x = (col + 0.5) * Pet::getWidth();

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_cross2);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_cross2_n;
	float h = txt2d->getContentSize().height;

	///////////////////////////top//////////////////////////////////
	Animation * aniTop = (row > 0)?Animation::create():nullptr;
	Sprite * spriteTop = nullptr;
	if (nullptr != aniTop) {
		aniTop->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_cross2_n; i++) {
			aniTop->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		spriteTop = Sprite::create(s_ani_cross2, Rect(0,0,w,h));
		addChild(spriteTop, CROSS_Z_ORDER);
		spriteTop->setRotation(-180);
		spriteTop->setPosition(pos);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////bottom//////////////////////////////////
	Animation * aniBottom = (row < ROW_NUM)?Animation::create():nullptr;
	Sprite * spriteBottom = nullptr;
	if (nullptr != aniBottom) {
		aniBottom->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_cross2_n; i++) {
			aniBottom->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		spriteBottom = Sprite::create(s_ani_cross2, Rect(0,0,w,h));
		addChild(spriteBottom, CROSS_Z_ORDER);
		spriteBottom->setPosition(pos);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////left//////////////////////////////////
	Animation * aniLeft = (col > 0)?Animation::create():nullptr;
	Sprite * spriteLeft = nullptr;
	if (nullptr != aniLeft) {
		aniLeft->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_cross2_n; i++) {
			aniLeft->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		spriteLeft = Sprite::create(s_ani_cross2, Rect(0,0,w,h));
		addChild(spriteLeft, CROSS_Z_ORDER);
		spriteLeft->setRotation(90);
		spriteLeft->setPosition(pos);
	}
	///////////////////////////////////////////////////////////////
	///////////////////////////right//////////////////////////////////
	Animation * aniRight = (col < COL_NUM)?Animation::create():nullptr;
	Sprite * spriteRight = nullptr;
	if (nullptr != aniRight) {
		aniRight->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_cross2_n; i++) {
			aniRight->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		spriteRight = Sprite::create(s_ani_cross2, Rect(0,0,w,h));
		addChild(spriteRight, CROSS_Z_ORDER);
		spriteRight->setRotation(-90);
		spriteRight->setPosition(pos);
	}
	///////////////////////////////////////////////////////////////
	if (nullptr != spriteTop) {
		spriteTop->runAction(Sequence::create(
			Spawn::create(
				MoveBy::create(1.2, Vec2(0, size.height)),
				Animate::create(aniTop),
				NULL
			),	
			CallFunc::create([=](){
				removeChild(spriteTop);
				}),
			NULL));
	}

	if (nullptr != spriteBottom) {
		spriteBottom->runAction(Sequence::create(
			Spawn::create(
				MoveBy::create(1.2, Vec2(0, -size.height)),
				Animate::create(aniBottom),
				NULL
			),	
			CallFunc::create([=](){
				removeChild(spriteBottom);
				}),
			NULL));
	}

	if (nullptr != spriteLeft) {
		spriteLeft->runAction(Sequence::create(
			Spawn::create(
				MoveBy::create(1.2, Vec2(-size.width, 0)),
				Animate::create(aniLeft),
				NULL
			),	
			CallFunc::create([=](){
				removeChild(spriteLeft);
			}),
			NULL));
	}

	if (nullptr != spriteRight) {
		spriteRight->runAction(Sequence::create(
			Spawn::create(
				MoveBy::create(1.2, Vec2(size.width, 0)),
				Animate::create(aniRight),
				NULL
			),	
			CallFunc::create([=](){
				removeChild(spriteRight);
			}),
			NULL));
	}
}

bool PetMatrix::bombClear() {
	Vector<Pet *> vNormalPets;
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			Pet *pet = m_pets[i][j];
			if (nullptr != pet) {
				if (pet->isSpecialNormal()) {
					vNormalPets.pushBack(pet);
				} else if (pet->isSpecialShade()) {
					auto special = dynamic_cast<PetSpecial *>(pet);
					if (nullptr != special && !special->hasShade()) {
						vNormalPets.pushBack(pet);
					}
				} else if (pet->isSpecialIce()) {
					auto special = dynamic_cast<PetSpecial *>(pet);
					if (nullptr != special && !special->isIceClosed()) {
						vNormalPets.pushBack(pet);
					}
				}
			}
		}
	}

	if (vNormalPets.size() <= 0) {
		return false;
	}

	std::random_device rd;
	std::mt19937 mt(rd());

	// do random shuffle.
	std::shuffle(vNormalPets.begin(), vNormalPets.end(), mt);

	int scoreSum = 0;
	for (int i=0; i<vNormalPets.size() && i<MAX_BOMB_CLEAR_NUM; i++) {
		Pet * pet = vNormalPets.at(i);
		scoreSum += s_score_pet;
		clearPet(pet, s_score_pet, true);
	}

	vNormalPets.clear();

	Audio::getInstance()->playBomb();
	GameManager::getInstance()->addScore(scoreSum);

	return true;
}

bool PetMatrix::tipPets() {
	int clearNum = 0;
	int row = -1;
	int col	= -1;

	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			if (nullptr == m_pets[i][j]) {
				int n = crossCheckNum(i, j, false);
				if (n > clearNum) {
					row = i;
					col = j;
					clearNum = n;
				}
			}
		}
	}

	if (clearNum > 0 && row>=0 && col>=0) {
		playTipEffect(row, col);
		return true;
	}

	return false;
}

void PetMatrix::playTipEffect(int row, int col) {
	if (nullptr != m_tipRing) {
		removeChild(m_tipRing, true);
		m_tipRing = nullptr;
	}

	if (nullptr != m_tipTouch) {
		removeChild(m_tipTouch, true);
		m_tipTouch = nullptr;
	}

	//tipRing
	{
		m_tipRing = Sprite::createWithSpriteFrameName(s_tip_ring);
		addChild(m_tipRing, TIP_Z_ORDER);
		m_tipRing->setPosition((col + 0.5) * Pet::getWidth(), (ROW_NUM - 1 - row + 0.5) *Pet::getHeight());

		Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_ring);
		if (nullptr == txt2d) {
			return;
		}

		float w = txt2d->getContentSize().width/s_ani_ring_n;
		float h = txt2d->getContentSize().height;

		Animation * animation = Animation::create();
		animation->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_ring_n; i++) {
			animation->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		m_tipRing->runAction(RepeatForever::create(Animate::create(animation)));
	}
	

	//tipTouch
	{
		Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_touch);
		if (nullptr == txt2d) {
			return;
		}

		float w = txt2d->getContentSize().width/s_ani_touch_n;
		float h = txt2d->getContentSize().height;

		Animation *aniTouch = Animation::create();
		aniTouch->setDelayPerUnit(0.2);
		for (int i=0; i<s_ani_touch_n; i++) {
			aniTouch->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
		}

		m_tipTouch = Sprite::create(s_ani_ring, Rect(0,0,w,h));
		addChild(m_tipTouch, TIP_Z_ORDER);
		m_tipTouch->setAnchorPoint(Vec2(0,0.8));
		m_tipTouch->setPosition(m_tipRing->getPosition());
		m_tipTouch->runAction(RepeatForever::create(Animate::create(aniTouch)));
	}
	
}

void PetMatrix::clearTipEffect() {
	if (nullptr != m_tipRing) {
		m_tipRing->stopAllActions();
		removeChild(m_tipRing, true);
		m_tipRing = nullptr;
	}

	if (nullptr != m_tipTouch) {
		m_tipTouch->stopAllActions();
		removeChild(m_tipTouch, true);
		m_tipTouch = nullptr;
	}
}

void PetMatrix::playLinkEffect(int srcRow, int srcCol, int dstRow, int dstCol) {
	if (srcRow < 0 || srcRow >= ROW_NUM) {
		return;
	}

	if (dstRow < 0 || dstRow >= ROW_NUM) {
		return;
	}

	if (srcCol < 0 || srcCol >= COL_NUM) {
		return;
	}

	if (dstCol < 0 || dstCol >= COL_NUM) {
		return;
	}

	//同行
	if (srcRow == dstRow && srcCol != dstCol) {
		Sprite * light = Sprite::createWithSpriteFrameName(s_tip_light);
		float w = abs(srcCol - dstCol) * Pet::getWidth();
		float scale = w/light->getContentSize().width;
		light->setScaleX(scale);
		addChild(light, EFFECT_LINK_Z_ORDER);
		light->setAnchorPoint(Vec2(0,0.5));
		int starCol = MIN(srcCol, dstCol);
		light->setPosition((starCol + 0.5) * Pet::getWidth(), (ROW_NUM -1 - srcRow + 0.5) * Pet::getHeight());
		light->runAction(
			Sequence::create(
			FadeIn::create(0.1),
			FadeOut::create(0.1),
			CallFunc::create([=](){
				removeChild(light);
			}),
			NULL
			)
		);
	}

	//同列
	if (srcCol == dstCol && srcRow != dstRow) {
		Sprite * light = Sprite::createWithSpriteFrameName(s_tip_light);
		float w = abs(srcRow - dstRow) * Pet::getHeight();
		float scale = w/light->getContentSize().width;
		light->setScaleX(scale);
		light->setAnchorPoint(Vec2(0, 0.5));
		light->setRotation(-90);
		addChild(light, EFFECT_LINK_Z_ORDER);		
		int starRow = MAX(srcRow, dstRow);
		light->setPosition((srcCol + 0.5) * Pet::getWidth(), (ROW_NUM - 1 - starRow + 0.5) * Pet::getHeight());
		light->runAction(
			Sequence::create(
			FadeIn::create(0.1),
			FadeOut::create(0.4),
			CallFunc::create([=](){
				removeChild(light);
		}),
			NULL
			)
			);
	}
}

void PetMatrix::playTouchEffect(int row, int col) {
	if (row < 0 || row >= ROW_NUM) {
		return;
	}

	if (col < 0 || col >= COL_NUM) {
		return;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_ring);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_ring_n;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_ring_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	Sprite * sprite = Sprite::create(s_ani_ring, Rect(0,0,w,h));
	addChild(sprite, EFFECT_TOUCH_Z_ORDER);
	sprite->setPosition((col + 0.5)*Pet::getWidth(), (ROW_NUM - 1 - row + 0.5) *Pet::getHeight());
	sprite->runAction(Sequence::create(
		Animate::create(ani),
		CallFunc::create([=](){	
			removeChild(sprite,true);
		}),
		NULL));
}

void PetMatrix::playTouchErrorEffect(int row, int col) {
	if (row < 0 || row >= ROW_NUM) {
		return;
	}

	if (col < 0 || col >= COL_NUM) {
		return;
	}

	Audio::getInstance()->playToucherror();
	Sprite * sprite = Sprite::createWithSpriteFrameName(s_icon_touch_error);
	addChild(sprite, EFFECT_TOUCH_Z_ORDER);
	const Point pos((col + 0.5)*Pet::getWidth(), (ROW_NUM - 1 - row + 0.5) *Pet::getHeight());
	sprite->setPosition(pos);
	sprite->runAction(Sequence::create(
		FadeOut::create(1),
		CallFunc::create([=](){	
			removeChild(sprite,true);
		}),
		NULL));

	const Point errPos(pos.x, pos.y + Pet::getHeight() * 0.5 + 5);

	playMinusTimeEffect(errPos);
}

void PetMatrix::playFrameEffect() {
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			auto pet = m_pets[i][j];
			if (nullptr != pet && pet->isVisible()) {
				auto special = dynamic_cast<PetSpecial*>(pet);
				if (nullptr != special) {
					special->playFrameEffect();
				}
			} 
		}
	}

	ActionInterval * checkHit = Sequence::create(
		DelayTime::create(1.5),
		CallFunc::create([=](){
			const int hit = HitManager::getInstance()->getHit();
			if (hit < 3) {
				stopFrameEffect();
			}
		}),
		NULL);
	
	runAction(checkHit);
}

void PetMatrix::stopFrameEffect() {
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			auto pet = m_pets[i][j];
			if (nullptr != pet && pet->isVisible()) {
				auto special = dynamic_cast<PetSpecial*>(pet);
				if (nullptr != special) {
					special->stopFrameEffect();
				}
			} 
		}
	}
}

void PetMatrix::playAddScoreEffect(const Point&pos, int score) {
	Sprite * sprite = nullptr;
	if (5 == score) {
		sprite = Sprite::createWithSpriteFrameName(s_tip_add_5);
	} else if (6 == score) {
		sprite = Sprite::createWithSpriteFrameName(s_tip_add_6);
	} else if (7 == score) {
		sprite = Sprite::createWithSpriteFrameName(s_tip_add_7);
	} else if (8 == score) {
		sprite = Sprite::createWithSpriteFrameName(s_tip_add_8);
	} else {
		return;
	}
	
	addChild(sprite, 100);
	sprite->setGlobalZOrder(100);
	const Size size = Director::getInstance()->getVisibleSize();
	ccBezierConfig config;
	config.endPosition = Point(Pet::getWidth() * COL_NUM * 0.5, size.height - getPositionY() - 30);

	if (pos.x < Pet::getWidth() * COL_NUM * 0.5) {
		config.controlPoint_1 = Point(pos.x + 100, pos.y + 50);
		config.controlPoint_2 = Point(pos.x + 100, pos.y + 100);
	} else {
		config.controlPoint_1 = Point(pos.x - 100, pos.y + 50);
		config.controlPoint_2 = Point(pos.x - 100, pos.y + 100);
	}

	sprite->setPosition(pos);
	sprite->runAction(Sequence::create(
		ScaleTo::create(0.1, 1.5, 1.5, 1),
		DelayTime::create(0.2),
		Spawn::create(
			//MoveBy::create(0.5,Vec2(0, 20)),
			ScaleTo::create(0.8, 1, 1, 1),
			BezierTo::create(0.8, config),
			FadeOut::create(1),NULL),		
		CallFunc::create([=](){
			removeChild(sprite,true);
		}),
		NULL));
}

void PetMatrix::playAttrackBossEffect(const Point&pos, int score) {
	Sprite * sprite  = Sprite::createWithSpriteFrameName(s_icon_bullet);
	addChild(sprite, 100);
	sprite->setGlobalZOrder(100);
	const Size size = Director::getInstance()->getVisibleSize();
	ccBezierConfig config;
	config.endPosition = Point(Pet::getWidth() * COL_NUM * 0.5, size.height - getPositionY() - 30);

	if (pos.x < Pet::getWidth() * COL_NUM * 0.5) {
		config.controlPoint_1 = Point(pos.x + 100, pos.y + 50);
		config.controlPoint_2 = Point(pos.x + 100, pos.y + 100);
	} else {
		config.controlPoint_1 = Point(pos.x - 100, pos.y + 50);
		config.controlPoint_2 = Point(pos.x - 100, pos.y + 100);
	}

	sprite->setPosition(pos);
	sprite->runAction(Sequence::create(
		ScaleTo::create(0.1, 1.5, 1.5, 1),
		DelayTime::create(0.4),
		Spawn::create(
		BezierTo::create(0.4, config),
		ScaleTo::create(0.4, 1, 1, 1),
		FadeOut::create(0.4),
		CallFunc::create([=](){
			Audio::getInstance()->playBullet();
		}),
		NULL),		
		CallFunc::create([=](){
			GameEvent::throwEvent(Event_Attrack_Boss, NULL);
			removeChild(sprite,true);
		}),
		NULL));
}

void PetMatrix::playMinusTimeEffect(const Point&pos) {
	Sprite * sprite = Sprite::createWithSpriteFrameName(s_tip_minus_5);
	addChild(sprite, EFFECT_TOUCH_Z_ORDER);
	sprite->setPosition(pos);
	sprite->runAction(Sequence::create(
		Spawn::create(
			MoveBy::create(0.5,Vec2(0, 20)),
			FadeOut::create(1),NULL),
		CallFunc::create([=](){	
			removeChild(sprite,true);
	}),
		NULL));
}


void PetMatrix::getCrossPets(std::vector<PetVec2>&clearable, PetVec2&touchable) {
	int maxRow = 0;
	int maxCol = 0;
	int maxPet = 0;
	for (int i=0; i<ROW_NUM; i++) {
		for (int j=0; j<COL_NUM; j++) {
			if (nullptr == m_pets[i][j]) {
				int n = getPetCross(i, j);
				if (n > maxPet) {
					maxRow = i;
					maxCol = j;
					maxPet = n;
				}
			}
		}
	}

	touchable.iRow = maxRow;
	touchable.iCol = maxCol;

	clearable.clear();
	for (int i=0; i<COL_NUM; i++) {
		auto pet = m_pets[maxRow][i];
		clearable.push_back(PetVec2(maxRow, i));
	}

	for (int i=0; i<ROW_NUM; i++) {
		if (i != maxRow) {
			clearable.push_back(PetVec2(i, maxCol));
		}
	}
}

int PetMatrix::getPetCross(int row, int col) {
	int sum = 0;
	for (int i=0; i<COL_NUM; i++) {
		auto pet = m_pets[row][i];
		if (nullptr != pet) {
			sum++;
		}
	}

	for (int i=0; i<ROW_NUM; i++) {
		auto pet = m_pets[i][col];
		if (nullptr != pet) {
			sum++;
		}
	}

	return sum;
}

void PetMatrix::playContinueClickEffect(const Point&pos, int num) {
	auto clickSprite = Sprite::createWithSpriteFrameName(s_tip_click);
	addChild(clickSprite, TIP_CLICK_Z_ORDER);

	auto number = LabelAtlas::create(String::createWithFormat("/%d",num)->_string, "number_click.png", 60, 70, '.');
	clickSprite->addChild(number);
	clickSprite->setCascadeOpacityEnabled(true);
	number->ignoreAnchorPointForPosition(false);
	number->setAnchorPoint(Vec2(0, 0.5));
	number->setPosition(clickSprite->getContentSize().width + 5, clickSprite->getContentSize().height * 0.5);

	clickSprite->setAnchorPoint(Vec2(0,0.5));
	clickSprite->setPosition((Pet::getWidth()* COL_NUM - number->getContentSize().width - clickSprite->getContentSize().width - 5)* 0.5, 
							Pet::getHeight() * ROW_NUM * 0.5);

	clickSprite->runAction(Sequence::create(
		DelayTime::create(0.2),
		Spawn::create(
		MoveBy::create(0.5,Vec2(0, 30)),
		FadeOut::create(0.8),NULL),		
		CallFunc::create([=](){	
			removeChild(clickSprite,true);
	}),
		NULL));
	
}