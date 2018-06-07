#ifndef __pet_h__
#define __pet_h__

#include "cocos2d.h"

enum EnumPetColor {
	Pet_Color_Undefine = -1,
	Pet_White = 0,
	Pet_Green,
	Pet_Yellow,
	Pet_Red,
	Pet_Purple,
	Pet_Blue,
	Max_Pet_Color,
};

enum EnumPetSpecial {
	Pet_Normal = 0,
	Pet_Clear3x3,
	Pet_Shade,
	Pet_Face,
	Pet_Ice,
	Pet_Change,
	Pet_Everyok,
	Max_Pet_Special,
};

struct PetVec2 {
	int iRow;
	int iCol;
	PetVec2(int r=0, int c=0) {
		iRow = r;
		iCol = c;
	}

	bool equal(const PetVec2&other) {
		return (other.iRow == iRow && other.iCol == iCol);
	}

	bool isValid() {
		return (iRow >=0 || iCol >= 0);
	}
};

class Pet:public cocos2d::Sprite {
public:
	Pet();
	virtual ~Pet();
	CREATE_FUNC(Pet);
	static Pet *create(EnumPetColor color, EnumPetSpecial special);
	virtual bool init() override;
	virtual bool initSprite(EnumPetColor color);
	virtual void initSelectSprite();

	static const char *getPetImageByCol(EnumPetColor color);
	static const char *getPetImageBySpecial(EnumPetSpecial special);
	static const char *getPetSelImageByCol(EnumPetColor color);
	static const char *getEyeOpenImageByCol(EnumPetColor color);
	static const char *getEyeCloseImageByCol(EnumPetColor color);


	inline EnumPetColor getColor() { return m_color; }
	inline EnumPetSpecial getSpecial() { return m_special; }

	static int getWidth();
	static int getHeight();

	inline void setPos(int row, int col) { m_row = row; m_col = col; }
	inline int getPosRow() { return m_row; }
	inline int getPosCol() { return m_col; }

	inline bool isSpecialClear3x3() { return (Pet_Clear3x3 == m_special); }
	inline bool isSpecialShade() { return (Pet_Shade == m_special); }
	inline bool isSpecialNormal() { return (Pet_Normal == m_special); }
	inline bool isSpecialFace() { return (Pet_Face == m_special);  }
	inline bool isSpecialIce() { return (Pet_Ice == m_special); }
	inline bool isSpecialChange() {return (Pet_Change == m_special); }
	inline bool isSpecialEveryok() {return (Pet_Everyok == m_special); }

	void select(bool selected);
	
	void openEye();
	void closeEye();
	void changeEye();

	void playEyeAction(float intrval);
	void playJellyAction(float intrval);
	void stopJellyAction();


protected:
	cocos2d::Sprite *m_selectSprite;
	cocos2d::Sprite *m_eyeOpen;
	cocos2d::Sprite *m_eyeClose;
	EnumPetColor m_color;
	EnumPetSpecial m_special;
	int m_row;
	int m_col;
};
#endif