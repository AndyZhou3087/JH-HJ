#include "JhChallengeCountLayer.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhSoundManager.h"
#include "JhHintBox.h"
#include "JhFightLayer.h"
#include "JhHeroStateUILayer.h"
#include "JhConst.h"

int usecount[] = { 1, 1, 3, 3, 5, 5, 7, 7, 8, 8, 10, 10, 13, 13, 15, 15, 18, 18, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };
JhChallengeCountLayer::JhChallengeCountLayer()
{
	lastclickindex = -1;
}


JhChallengeCountLayer::~JhChallengeCountLayer()
{
}

bool JhChallengeCountLayer::init(int* wincount, int winnpccount, bool isRevive)
{
	m_wincount = wincount;
	m_winnpcount = winnpccount;
	m_isRevive = isRevive;
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhchallengeCountLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView* heroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");
	heroimg->addTouchEventListener(CC_CALLBACK_2(JhChallengeCountLayer::onHeroimg, this));
	std::string heroidstr = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);
	heroimg->setContentSize(Sprite::createWithSpriteFrameName(heroidstr)->getContentSize());

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhChallengeCountLayer::onCancel, this));

	cocos2d::ui::Button* continuebtn = (cocos2d::ui::Button*)csbnode->getChildByName("continuebtn");
	continuebtn->addTouchEventListener(CC_CALLBACK_2(JhChallengeCountLayer::onContinue, this));

	totalwincountlbl = (cocos2d::ui::Text*)csbnode->getChildByName("wincountlbl");

	std::string str = StringUtils::format("%d", *m_wincount);
	totalwincountlbl->setString(str);

	for (int i = 0; i < 3; i++)
	{
		str = StringUtils::format("btn%d", i);
		selectbtn[i] = (cocos2d::ui::Button*)csbnode->getChildByName(str);
		selectbtn[i]->setTag(i);
		selectbtn[i]->addTouchEventListener(CC_CALLBACK_2(JhChallengeCountLayer::onSelect, this));
		valuelbl[i] = (cocos2d::ui::Text*)selectbtn[i]->getChildByName("valuelbl");
		str = StringUtils::format("%d%%", winnpccount / 3);
		valuelbl[i]->setString(str);

		str = StringUtils::format("usesprite%d",i);

		cocos2d::ui::Widget* usespite = (cocos2d::ui::Widget*)csbnode->getChildByName(str);

		cocos2d::ui::Text* uselbl = (cocos2d::ui::Text*)usespite->getChildByName("neednum");
		str = StringUtils::format("胜点%d", usecount[winnpccount / 3 - 1]);
		uselbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		if (*m_wincount < usecount[winnpccount / 3 - 1])
			uselbl->setTextColor(Color4B(204,4,4,255));
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

JhChallengeCountLayer* JhChallengeCountLayer::create(int* wincount, int winnpccount, bool isRevive)
{
	JhChallengeCountLayer *pRet = new JhChallengeCountLayer();
	if (pRet && pRet->init(wincount, winnpccount, isRevive))
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

void JhChallengeCountLayer::onHeroimg(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_gameLayer->addChild(JhHeroStateUILayer::create(), 4, "JhHeroStateUILayer");
	}
}

void JhChallengeCountLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		g_hero->setIsWDChallenge(false);
		this->removeFromParentAndCleanup(true);

	}
}

void JhChallengeCountLayer::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (lastclickindex == 0)
		{
			float val = g_hero->getLifeValue();
			float addval = g_hero->getMaxLifeValue() * m_winnpcount / 300;
			if (val + addval > g_hero->getMaxLifeValue())
				g_hero->setLifeValue(g_hero->getMaxLifeValue());
			else
				g_hero->setLifeValue(val+addval);
		}
		else if (lastclickindex == 1)
		{
			float lastbonus = g_hero->getTotalAtkBonusPercent();
			float bonus = lastbonus + m_winnpcount * 1.0f / 300.0f;
			g_hero->setTotalAtkBonusPercent(bonus);

		}
		else if (lastclickindex == 2)
		{
			float lastbonus = g_hero->getTotalDfBonusPercent();
			float bonus = lastbonus + m_winnpcount *1.0f / 300.0f;
			g_hero->setTotalDfBonusPercent(bonus);
		}
		JhFightLayer* figherlayer = (JhFightLayer*)g_gameLayer->getChildByName("fightlayer");
		if (!m_isRevive)
			figherlayer->updateFightNextNpc();
		else
			figherlayer->reviveContinueChallege();
		this->removeFromParentAndCleanup(true);
	}
}

void JhChallengeCountLayer::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnnode = (cocos2d::ui::Button*)pSender;
		int tag = btnnode->getTag();
		if (tag == lastclickindex)
		{
			btnnode->setBright(true);
			lastclickindex = -1;
			*m_wincount += usecount[m_winnpcount / 3 - 1];
		}
		else
		{

			if (*m_wincount < usecount[m_winnpcount / 3 - 1])
			{
				JhHintBox* hintBox = JhHintBox::create(JhCommonFuncs::gbk2utf("胜点不足！!"));
				this->addChild(hintBox);
				return;
			}
			if (lastclickindex >= 0)
			{
				selectbtn[lastclickindex]->setBright(true);
			}
			else
			{
				*m_wincount -= usecount[m_winnpcount / 3 - 1];
			}
			selectbtn[tag]->setBright(false);
			lastclickindex = tag;

		}
		std::string str = StringUtils::format("%d", *m_wincount);
		totalwincountlbl->setString(str);
	}
}