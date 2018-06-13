#include "JhNoticeLayer.h"
#include "JhSoundManager.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"

JhNoticeLayer::JhNoticeLayer()
{
}


JhNoticeLayer::~JhNoticeLayer()
{

}

JhNoticeLayer* JhNoticeLayer::create(std::string contentstr)
{
	JhNoticeLayer *pRet = new JhNoticeLayer();
	if (pRet && pRet->init(contentstr))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool JhNoticeLayer::init(std::string contentstr)
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("jhnoticeLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhNoticeLayer::onOk, this));

	//checkbox
	m_checkBox = (cocos2d::ui::CheckBox*)csbnode->getChildByName("check");
	m_checkBox->addEventListener(CC_CALLBACK_2(JhNoticeLayer::checkBoxCallback, this));

	cocos2d::ui::ScrollView* scrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	scrollview->setScrollBarEnabled(false);
	scrollview->setBounceEnabled(true);

	Label* contentlbl = Label::createWithTTF(JhGlobalData::noticecontent, "fonts/STXINGKA.TTF", 30);
	contentlbl->setAnchorPoint(Vec2(0, 1));
	contentlbl->setColor(Color3B(0, 0, 0));
	contentlbl->setHorizontalAlignment(TextHAlignment::LEFT);
	contentlbl->setLineBreakWithoutSpace(true);
	contentlbl->setMaxLineWidth(scrollview->getContentSize().width);
	scrollview->addChild(contentlbl);
	int innerheight = contentlbl->getStringNumLines() * 30;//contentlbl->getHeight();
	int contentheight = scrollview->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, innerheight));

	contentlbl->setPosition(Vec2(0, innerheight));

	JhGlobalData::setNoPopNoticeDay(-1);

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void JhNoticeLayer::checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type)
{
	switch(type)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		case cocos2d::ui::CheckBox::EventType::SELECTED://选中
			JhGlobalData::setNoPopNoticeDay(JhGlobalData::getDayOfYear());
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED://不选中
			JhGlobalData::setNoPopNoticeDay(-1);
			break;
		default:
			break;
	}
}

void JhNoticeLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
