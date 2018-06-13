#ifndef __pet_matrix_h__
#define __pet_matrix_h__

#include "cocos2d.h"
#include "LevelData.h"
#include "pet.h"

class PetMatrix:public cocos2d::Layer {
public:
	PetMatrix();
	~PetMatrix();
	CREATE_FUNC(PetMatrix);
	static PetMatrix *createPetMatrix(const LevelData&levelData);

	virtual bool init() override;
	void enter();
	void initSpecial();
	void createPets(const LevelData&levelData);
	void createPet(int index, int color, EnumPetSpecial special);
	void initPetMatirx();
	bool onTouch(const cocos2d::Point&pos, bool&err);
	bool checkPets();
	void clearAllPet();
	bool bombClear();
	bool tipPets();
	void clearTipEffect();
	PetVec2 getTouchPetVec2(const cocos2d::Point&pos);
	void getCrossPets(std::vector<PetVec2>&clearable, PetVec2&touchable);

	static const int ROW_NUM = 8;
	static const int COL_NUM = 7;
	static const int MAX_PET = 40;
	static const int GRID_NUM;

protected:
	bool crossCheck(const int row, const int col, bool clear);
	int crossCheck(const int row, const int col, EnumPetSpecial special);
	int crossCheckNum(const int row, const int col, bool clear);
	bool crossClear(const int row, const int col);
	
	bool petCheck(cocos2d::Vector<Pet*>&vPets, Pet *pet);
	int clearSamePet(cocos2d::Vector<Pet*>&vPets, int row, int col);
	void clearPet(Pet *pet, int petScore = 5, bool bombEffect = false);
	int clear3x3Pet(Pet *pet);
	void playClearEffect(Pet *pet, bool clear = true);//消除特效
	void playClear2Effect(Pet *pet, bool clear = true);//切换特效
	void playBombEffect(Pet *pet, bool clear = true);//炸弹特效
	void playCrossEffect(int row, int col, int score);
	void playCross2Effect(int row, int col, int score);
	void playTipEffect(int row, int col);
	void playLinkEffect(int srcRow, int srcCol, int dstRow, int dstCol);
	void playTouchEffect(int row, int col);
	void playTouchErrorEffect(int row, int col);
	void playFrameEffect();
	void stopFrameEffect();

	void playAddScoreEffect(const cocos2d::Point&pos, int score);
	void playAttrackBossEffect(const cocos2d::Point&pos, int score);
	void playMinusTimeEffect(const cocos2d::Point&pos);
	void playContinueClickEffect(const cocos2d::Point&pos, int num);
	
	int getPetCross(int row, int col);

	void updatePets();
	void changeFace();

protected:
	//cocos2d::SpriteBatchNode *m_batchNode;
	Pet * m_pets[ROW_NUM][COL_NUM];
	int m_facePetNum;
	cocos2d::Sprite *m_tipRing;
	cocos2d::Sprite *m_tipTouch;
	bool m_bClearAll;
};

#endif