#ifndef __pet_special_h__
#define __pet_special_h__

#include "Pet.h"
#include "cocos2d.h"

#define CREATE_PET_FUNC(__RT_TYPE__) \
	static __RT_TYPE__* create(EnumPetColor color) \
{ \
	__RT_TYPE__ *pRet = __RT_TYPE__::create(); \
	if(nullptr != pRet){ \
		pRet->initSprite(color); \
		return pRet; \
	} \
	\
	CC_SAFE_DELETE(pRet); \
	return nullptr; \
}


class PetSpecial {
public:
	PetSpecial();
	virtual ~PetSpecial();

	virtual bool onClear(){ return false; }
	virtual void changeToNormal(){ CCASSERT(false, "can't change to normal"); }
	virtual void changeToFace(){ CCASSERT(false, "can't change to face"); }
	virtual void changeIce(){}
	virtual void changeColor(EnumPetColor color){}
	virtual void playRollEffect(){}
	virtual void playFrameEffect() {}
	virtual void stopFrameEffect() {}
	virtual bool hasShade() { return false; }
	virtual bool isIceClosed() { return false; }
};


class PetNormal:public PetSpecial,public Pet {
public:
	PetNormal();
	virtual ~PetNormal();
	CREATE_FUNC(PetNormal);
	CREATE_PET_FUNC(PetNormal);

	virtual bool initSprite(EnumPetColor color) override;
	virtual bool init();

	virtual bool onClear();
	virtual void changeToNormal();
	virtual void changeToFace();
	
	virtual void playClearEffect();
	virtual void playFrameEffect();
	virtual void stopFrameEffect();

private:
	Sprite *m_face;
};

class PetClear:public PetSpecial,public Pet {
public:
	PetClear();
	virtual ~PetClear();
	CREATE_FUNC(PetClear);
	CREATE_PET_FUNC(PetClear);

	virtual bool initSprite(EnumPetColor color);
	virtual bool init();
	virtual bool onClear();
	virtual void changeToNormal() {}
	virtual void changeToFace() {}
	virtual void playClearEffect();
};

class PetShade:public PetSpecial,public Pet {
public:
	PetShade();
	virtual ~PetShade();
	CREATE_FUNC(PetShade);
	CREATE_PET_FUNC(PetShade);

	virtual bool initSprite(EnumPetColor color);
	virtual bool init();
	virtual bool onClear();
	virtual void playClearEffect();
	virtual bool hasShade();

private:
	Sprite *m_shade;
};

class PetFace:public PetSpecial,public Pet {
public:
	PetFace();
	virtual ~PetFace();
	CREATE_FUNC(PetFace);
	CREATE_PET_FUNC(PetFace);

	virtual bool initSprite(EnumPetColor color);
	virtual bool init();
	virtual bool onClear();
	virtual void changeToNormal();
	virtual void changeToFace();
	virtual void playClearEffect();

private:
	Sprite *m_face;
};

class PetIce:public PetSpecial,public Pet {
public:
	PetIce();
	virtual ~PetIce();
	CREATE_FUNC(PetIce);
	CREATE_PET_FUNC(PetIce);

	virtual bool initSprite(EnumPetColor color);
	virtual bool init();
	virtual bool onClear();
	virtual void playClearEffect();
	virtual void changeIce();
	virtual bool isIceClosed() { return m_closed; }

private:
	void playClosingEffect();
	void playUncloseEffect();

private:
	Sprite *m_ice;
	Sprite *m_iceClosing;
	bool m_closed;
};

class PetChange:public PetSpecial,public Pet {
public:
	PetChange();
	virtual ~PetChange();
	CREATE_FUNC(PetChange);
	CREATE_PET_FUNC(PetChange);

	virtual bool initSprite(EnumPetColor color);
	virtual bool init();
	virtual bool onClear();
	virtual void changeColor(EnumPetColor color);
	virtual void playClearEffect();
	virtual void playRollEffect();

private:
	Sprite *m_rollSprite;
};

class PetEveryok:public PetSpecial,public Pet {
public:
	PetEveryok();
	virtual ~PetEveryok();
	CREATE_FUNC(PetEveryok);
	CREATE_PET_FUNC(PetEveryok);

	virtual bool initSprite(EnumPetColor color) override;
	virtual bool init();
	virtual bool onClear();
};

#endif