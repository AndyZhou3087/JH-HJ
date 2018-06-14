#include "JhPromotionLayer.h"
#include "JhCommonFuncs.h"
#include "JhSoundManager.h"
#include "JhWaitingProgress.h"
#include "JhHintBox.h"
#include "JhFactionMemberLayer.h"
#include "JhConst.h"

JhPromotionLayer::JhPromotionLayer()
{
	m_select = -1;
}


JhPromotionLayer::~JhPromotionLayer()
{

}

JhPromotionLayer* JhPromotionLayer::create(FactionMemberData *data, Node* target)
{
	JhPromotionLayer *pRet = new JhPromotionLayer();
	if (pRet && pRet->init(data, target))
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

bool JhPromotionLayer::init(FactionMemberData *data, Node* target)
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("jhpromotionLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("actionbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhPromotionLayer::onOk, this));

	/*cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhPromotionLayer::onBack, this));*/

	//checkbox

	for (int i = 0; i < 3; i++)
	{
		std::string str = StringUtils::format("check%d", i + 1);
		m_checkBox[i] = (cocos2d::ui::CheckBox*)csbnode->getChildByName(str);
		m_checkBox[i]->addEventListener(CC_CALLBACK_2(JhPromotionLayer::checkBoxCallback, this));
		m_checkBox[i]->setTag(i + 2);
	}
	m_target = target;
	m_memberdata = data;
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



void JhPromotionLayer::checkBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type)
{
	Node* checkbtn = (Node*)pSender;
	int tag = checkbtn->getTag();
	switch(type)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		case cocos2d::ui::CheckBox::EventType::SELECTED://选中
			for (int i = 0; i < 3; i++)
			{
				if (i != tag - 2)
				{
					m_checkBox[i]->setSelected(false);
				}
				else
				{
					m_select = tag;
				}
			}
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED://不选中
			m_select = -1;
			break;
		default:
			break;
	}
}

void JhPromotionLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if (m_select == m_memberdata->position || m_select < 0)
		{
			this->removeFromParentAndCleanup(true);
			return;
		}
		JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		JhServerDataSwap::init(this)->promotionFaction(m_memberdata->factionid, m_memberdata->userid, m_memberdata->herotype, m_select);
	}
}

void JhPromotionLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhPromotionLayer::onSuccess()
{
	FactionMemberItem* mbitem = (FactionMemberItem*)m_target;
	mbitem->updatePosition(m_select);
	this->removeFromParentAndCleanup(true);
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void JhPromotionLayer::onErr(int errcode)
{
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox* hintbox;
	if (errcode == -4)
	{
		if (m_select == 2)
			hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("任命失败！ 副帮主人数已超限，可提升帮派等级增加副帮主人数！"));
		else
			hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("任命失败！ 长老人数已超限，可提升帮派等级增加长老人数！"));
	}
	else
	{
		hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("任命失败！请检查网络连接或者稍后重试！"));
	}
	this->addChild(hintbox);
}
