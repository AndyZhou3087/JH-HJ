#ifndef __menu_controller_h__
#define __menu_controller_h__

#include "cocos2d.h"
USING_NS_CC;

class MenuController {
public:
	MenuController();
	virtual ~MenuController();

	virtual void enter();
	virtual void exit();
	inline bool isEnter() { return m_bEnter; }
	void regMenu(Node *m);
	inline void regHomeMenu(MenuController * home) { m_homeMenu = home; }

	virtual void initial();

	virtual void playEffectNode();
	virtual void stopEffectNode();

protected:
	Node *m_Menu;
	bool m_bEnter;

	MenuController *m_homeMenu;

	Node *m_effectNode1;
	Node *m_effectNode2;
	Node *m_effectNode3;
	Node *m_effectNode4;
};

#endif