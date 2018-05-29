#include "HintBox.h"
#include "GlobalData.h"
#include "CommonFuncs.h"
#include "SoundManager.h"
bool HintBox::init(std::string text)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode;
	if (text == CommonFuncs::gbk2utf("逃跑成功！") || text.find(CommonFuncs::gbk2utf("已满员")) != std::string::npos || text.find(CommonFuncs::gbk2utf("需要申请者达到")) != std::string::npos || text.find(CommonFuncs::gbk2utf("匹配失败")) != std::string::npos)
	{
		LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
		this->addChild(color);
		csbnode = CSLoader::createNode("hintLayer2.csb");
	}
	else
	{
		csbnode = CSLoader::createNode("hintLayer.csb");
	}
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(HintBox::onOk, this));

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

HintBox* HintBox::create(std::string content)
{
	HintBox *pRet = new HintBox();
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

void HintBox::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void HintBox::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

