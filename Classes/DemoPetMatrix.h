#ifndef __demo_pet_matrix_h__
#define __demo_pet_matrix_h__

#include "PetMatrix.h"
#include "DemoPetData.h"

class DemoPetMatrix:public PetMatrix {
public:
	DemoPetMatrix();
	~DemoPetMatrix();

	CREATE_FUNC(DemoPetMatrix);
	static DemoPetMatrix *createPetMatrix(const std::vector<DemoPet>& demo);
	static DemoPetMatrix *createPetMatrix(const LevelData&levelData);
	void createPets(const std::vector<DemoPet>& demo);
	void createPets(const LevelData&levelData);
	void playLeftLinkEffect(int src, int dst, int row, bool bRed);
	void playRightLinkEffect(int src, int dst, int row, bool bRed);
	void playTopLinkEffect(int src, int dst, int col, bool bRed);
	void playBottomLinkEffect(int src, int dst, int col, bool bRed);

	void playShineEffect(cocos2d::Vector<cocos2d::Sprite *> vBall);
	
	void playTouchEffect(int row, int col);
	void stopTouchEffect();

	void markSprite(int row, int col, bool bYes);

	void clearMarkSprite();
	void clearAllBall();
	
	bool getClearablePets(std::vector<PetVec2>&cross, std::vector<PetVec2>&clearable, std::vector<PetVec2>&unClearable, PetVec2&touchable, EnumPetSpecial special = Pet_Normal);

private:
	void getPetAround(int row, int col, std::vector<PetVec2>&cross, std::vector<PetVec2>&clearable, std::vector<PetVec2>&unClearable);
	bool isVecContainItem(std::vector<PetVec2> &vPets, const PetVec2 &vPet);
	void clearBall(cocos2d::Vector<cocos2d::Sprite *> *vBall);

private:
	cocos2d::Vector<cocos2d::Sprite *> m_vLeftBalls;
	cocos2d::Vector<cocos2d::Sprite *> m_vRightBalls;
	cocos2d::Vector<cocos2d::Sprite *> m_vTopBalls;
	cocos2d::Vector<cocos2d::Sprite *> m_vBottomBalls;
	cocos2d::Vector<cocos2d::Sprite *> m_vMark;

};

#endif