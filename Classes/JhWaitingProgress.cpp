#include "JhWaitingProgress.h"

#include "JhCommonFuncs.h"
bool JhWaitingProgress::init(std::string text)
{
    if (Layer::init()) 
	{
		LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 128));
		this->addChild(color);

		boxbg = Sprite::createWithSpriteFrameName("ui/waitingbg.png");
		boxbg->setPosition(Vec2(360, 640));
		addChild(boxbg);

		Sprite* progress = Sprite::createWithSpriteFrameName("ui/waitingprogress.png");
		progress->setPosition(Vec2(progress->getContentSize().width, boxbg->getContentSize().height/2));
		boxbg->addChild(progress);
		progress->runAction(RepeatForever::create(RotateTo::create(2, 720)));

		textstr = text;
		//this->scheduleOnce(schedule_selector(JhWaitingProgress::delayShowText), 0.01f);
		delayShowText(0);
    }

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void JhWaitingProgress::delayShowText(float dt)
{
	Label* textlbl = Label::createWithTTF(JhCommonFuncs::gbk2utf(textstr.c_str()), "fonts/STXINGKA.TTF", 25);
	textlbl->setPosition(Vec2(boxbg->getContentSize().width / 2, boxbg->getContentSize().height / 2));
	textlbl->setColor(Color3B(255, 255, 255));
	boxbg->addChild(textlbl);
}

JhWaitingProgress* JhWaitingProgress::create(std::string content)
{
	JhWaitingProgress *pRet = new JhWaitingProgress();
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