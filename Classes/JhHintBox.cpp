#include "JhHintBox.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhSoundManager.h"
#include "JhConst.h"

bool JhHintBox::init(std::string text)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode;
	if (text == JhCommonFuncs::gbk2utf("逃跑成功！") || text.find(JhCommonFuncs::gbk2utf("已满员")) != std::string::npos || text.find(JhCommonFuncs::gbk2utf("需要申请者达到")) != std::string::npos || text.find(JhCommonFuncs::gbk2utf("匹配失败")) != std::string::npos)
	{
		LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
		this->addChild(color);
		csbnode = CSLoader::createNode("jhhintLayer2.csb");
	}
	else
	{
		csbnode = CSLoader::createNode("jhhintLayer.csb");
	}
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhHintBox::onOk, this));

	cocos2d::ui::Text* content = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	content->setString(text);
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

JhHintBox* JhHintBox::create(std::string content)
{
	JhHintBox *pRet = new JhHintBox();
	if (pRet && pRet->init(content))
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

void JhHintBox::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void JhHintBox::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

