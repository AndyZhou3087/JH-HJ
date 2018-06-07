#include "ULoadGameLayerController.h"
#include "GameUtil.h"
#include "cocos2d.h"
#include "Pet.h"
USING_NS_CC;

ULoadGameLayerController::ULoadGameLayerController():
m_pot1(NULL),
m_pot2(NULL),
m_pot3(NULL),
m_loading(NULL)
{

}

ULoadGameLayerController::~ULoadGameLayerController() {

}

void ULoadGameLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}


	m_loading = dynamic_cast<Sprite*>(m_layer->getChildByName("loading"));
	m_pot1 = dynamic_cast<Sprite*>(m_layer->getChildByName("loading")->getChildByName("pot1"));
	m_pot2 = dynamic_cast<Sprite*>(m_layer->getChildByName("loading")->getChildByName("pot2"));
	m_pot3 = dynamic_cast<Sprite*>(m_layer->getChildByName("loading")->getChildByName("pot3"));

	initPet();
}

void ULoadGameLayerController::initPet() {
	if (nullptr == m_loading) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	//Ëæ»úÊýÒýÇæ
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> petAllColorDis(0, Max_Pet_Color - 1);

	// do random shuffle.
	std::vector<EnumPetColor> vPetCol;
	for (int i=0; i<Max_Pet_Color; i++) {
		vPetCol.push_back((EnumPetColor)i);
	}
	std::shuffle(vPetCol.begin(), vPetCol.end(), mt);

	const float y = m_loading->getContentSize().height + 5;
	const float margin = 80;
	const float petIntrval = (size.width - 160) * 0.2;
	const float loadPosX = m_loading->getPositionX() - m_loading->getContentSize().width * 0.5;
	for (int i=0; i<vPetCol.size(); i++) {
		Pet * pet = Pet::create(vPetCol.at(i), Pet_Normal);
		pet->setAnchorPoint(Point(0.5,0));
		m_loading->addChild(pet);
		pet->setPosition(margin + petIntrval *i - loadPosX, y);

		float intrval = GameUtil::getRandomNum(2,4);
		float delay = GameUtil::getRandomNum(0,10) * 0.1;
		pet->runAction(Sequence::create(
				DelayTime::create(delay),
				CallFunc::create([=](){
					pet->playJellyAction(intrval);
				}),
				NULL));
	}

	 
}

void ULoadGameLayerController::enter() {
	CenterLayerController::enter();
}

void ULoadGameLayerController::exit() {
	CenterLayerController::exit();
}


void ULoadGameLayerController::updatePot(int n) {
	if (nullptr != m_pot1) {
		m_pot1->setVisible(n>0);
	}

	if (nullptr != m_pot2) {
		m_pot2->setVisible(n>1);
	}

	if (nullptr != m_pot3) {
		m_pot3->setVisible(n>2);
	}
}