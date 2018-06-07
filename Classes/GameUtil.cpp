#include "cocos2d.h"
#include "GameUtil.h"

USING_NS_CC;

void GameUtil::initRandSeed() {
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	//都转化为毫秒
	unsigned long reed = nowTimeval.tv_sec*1000000 + nowTimeval.tv_usec;
	//srand()中传入一个随机数种子
	srand(reed);
}

time_t GameUtil::getNowTime()
{ 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return nowTimeval.tv_sec;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	struct tm* tm;
	time_t timep;
	time(&timep);
	return timep;
#endif
}

long long GameUtil::getNowTimeMs() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	return ((long long)(nowTimeval.tv_sec))*1000 + nowTimeval.tv_usec/1000;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec*1000 + tv.tv_usec/1000;
#endif
}

bool GameUtil::isBeforeToday(time_t sec) {
	struct tm *tm;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//win32平台
	time_t timep;  
	time(&timep);  
	tm = localtime(&timep);
#else  
	struct timeval nowTimeval;
	gettimeofday(&nowTimeval, NULL);
	tm = localtime(&nowTimeval.tv_sec);
#endif  

	struct tm * otherDay = gmtime(&sec);

	if (otherDay->tm_year < tm->tm_year) {
		return true;
	} else if (otherDay->tm_year > tm->tm_hour) {
		return false;
	}

	if (otherDay->tm_mon < tm->tm_mon) {
		return true;
	} else if (otherDay->tm_mon > tm->tm_mon) {
		return false;
	}

	if (otherDay->tm_mday < tm->tm_mday) {
		return true;
	} else if (otherDay->tm_mday > tm->tm_mday) {
		return false;
	}

	return false;
}

long long GameUtil::getTodayLeftSec() {
	long long nowSec = getNowTime();
	return (86400 - nowSec%86400);
}

bool GameUtil::getRandomBoolean(float rate) {

	int rate10 = (int)(rate*10.0);
	int randNum = rand();
	if (randNum%10 <= rate10) {
		return true;
	}
	else {
		return false;
	}
}

bool GameUtil::getRandomBoolean() {

	if (0 == rand()%2) {
		return true;
	}
	else {
		return false;
	}
}

int GameUtil::getRandomNum(int range) {

	if (range <= 0) {
		return 0;
	}

	return rand()%range;
}

int GameUtil::getRandomNum(int rangeStart, int rangeEnd) {

	if (rangeEnd < rangeStart) {
		CCASSERT(false, "get random fail");
		return 0;
	}

	if (rangeStart == rangeEnd) {
		return rangeStart;
	}

	int delta = rand()%(rangeEnd - rangeStart);
	return rangeStart + delta;
}

void GameUtil::shake(Node * node, float scaleLarge, float scaleSmall) {
	if (NULL == node) {
		return;
	}

	CCActionInterval * actionScaleLarge = CCScaleTo::create(0.1, scaleLarge, scaleLarge, 1);
	CCActionInterval * actionScaleSmall = CCScaleTo::create(0.1, scaleSmall, scaleSmall, 1);
	CCActionInterval * actionScaleNormal = CCScaleTo::create(0.1, 1, 1, 1);
	node->runAction(CCSequence::create(actionScaleLarge, actionScaleSmall, actionScaleNormal, NULL));
}

void GameUtil::shake(Node * node) {
	if (NULL == node) {
		return;
	}

	node->runAction(CCSequence::create(
		MoveBy::create(0.02, Vec2(0, 15)), 
		MoveBy::create(0.02, Vec2(0, -27)), 
		MoveBy::create(0.02, Vec2(0, 22)), 
		MoveBy::create(0.02, Vec2(0, -14)),
		MoveBy::create(0.02, Vec2(0, 4)),
		NULL));
}

bool GameUtil::isPhone() {
	static const Size size = Director::getInstance()->getVisibleSize();
	static const float rate = size.height/size.width;
	if (rate >= 1.49) {
		return true;
	} else {
		return false;
	}
}

void GameUtil::jump(cocos2d::Node *node, float dt, bool repeat, float intrval) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.1, 0.9, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.9, 1.1, 1)),
		MoveBy::create(0.2, Vec2(0,dt)),
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.2, 0.9, 1)),
		MoveBy::create(0.2, Vec2(0,-dt)),
		NULL),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		 node->runAction(RepeatForever::create(
			 Sequence::create(
			 action,
			 DelayTime::create(intrval),
			 NULL)
			 ));
	} else {
		node->runAction(action);
	}
	
}

void GameUtil::jellyJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.1, 0.9, 1),
		Spawn::create(
			EaseExponentialOut::create(ScaleTo::create(0.1, 0.9, 1.1, 1)),
			MoveBy::create(0.2, Vec2(0,dt)),
			NULL),
		Spawn::create(
			EaseExponentialIn::create(ScaleTo::create(0.1, 1.2, 0.9, 1)),
			MoveBy::create(0.2, Vec2(0,-dt)),
			NULL),
		ScaleTo::create(0.1, 0.95, 1.05, 1),
		ScaleTo::create(0.1, 1.05, 0.95, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}
		
		node->runAction(RepeatForever::create(
			Sequence::create(
			action,
			DelayTime::create(intrval),
			NULL)
			));
	} else {
		node->runAction(action);
	}
}

void GameUtil::petJump(cocos2d::Node *node, float dt, bool repeat, float intrval, int tag, ActionInterval *ac ) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		ScaleTo::create(0.2, 1.05, 0.95, 1),
		Spawn::create(
		EaseExponentialOut::create(ScaleTo::create(0.1, 0.95, 1.05, 1)),
		MoveBy::create(0.2, Vec2(0,dt)),
		ac,
		NULL),
		Spawn::create(
		EaseExponentialIn::create(ScaleTo::create(0.1, 1.1, 0.95, 1)),
		MoveBy::create(0.2, Vec2(0,-dt)),
		NULL),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}

		node->runAction(RepeatForever::create(
			Sequence::create(
			action,
			DelayTime::create(intrval),
			NULL)
			));
	} else {
		node->runAction(action);
	}
}

void GameUtil::jelly(Node *node, bool repeat, float intrval, bool delay, int tag) {
	if (nullptr == node) {
		return;
	}

	ActionInterval * action = Sequence::create(
		EaseSineIn::create(ScaleTo::create(0.08, 0.95, 1.05, 1)),
		EaseSineOut::create(ScaleTo::create(0.2, 1.15, 0.95, 1)),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 0.98, 1.08, 1),
		ScaleTo::create(0.1, 1.02, 0.98, 1),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	if (repeat) {
		if (0 != tag) {
			action->setTag(tag);
		}
		if (delay) {
			node->runAction(RepeatForever::create(
				Sequence::create(
				DelayTime::create(getRandomNum(1,10)*0.1),
				action,
				DelayTime::create(intrval),
				NULL)
				));
		} else {
			node->runAction(RepeatForever::create(
				Sequence::create(
				action,
				DelayTime::create(intrval),
				NULL)
				));
		}
		
	} else {
		if (delay) {
			node->runAction(Sequence::create(
				DelayTime::create(getRandomNum(1,10)*0.1),
				action,
				NULL));
		} else {
			node->runAction(action);
		}	
	}
}

void GameUtil::jumpDown(cocos2d::Node *node, float dt) {
	if (nullptr == node) {
		return;
	}

	const float originY = node->getPositionY();
	node->setPositionY(originY + dt);

	ActionInterval *action = Sequence::create(
			MoveBy::create(0.2, Vec2(0,-dt - 10)),
			MoveBy::create(0.2, Vec2(0,20)),
			MoveBy::create(0.1, Vec2(0, -18)),
			MoveBy::create(0.1, Vec2(0, 13)),
			MoveBy::create(0.1, Vec2(0, -5)),


			ScaleTo::create(0.1, 1.02, 0.98, 1),
			ScaleTo::create(0.1, 0.98, 1, 1),
			ScaleTo::create(0.1, 1.02, 0.98, 1),
			ScaleTo::create(0.1, 1, 1, 1),
		NULL);

	node->runAction(action);
}
HelpObj* HelpObj::m_instance = nullptr;
HelpObj* HelpObj::getInstance(){
	if(m_instance == nullptr){
		m_instance = new HelpObj();
	}
	return m_instance;
}
HelpObj::HelpObj()
{
}
HelpObj::~HelpObj() {
}
TestObj* TestObj::m_instance = nullptr;
TestObj* TestObj::getInstance(){
	if(m_instance == nullptr){
		m_instance = new TestObj();
	}
	return m_instance;
}
TestObj::TestObj()
{
}
TestObj::~TestObj() {
}
