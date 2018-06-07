#include "JhFactionCreateLayer.h"
#include "JhCommonFuncs.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhConst.h"
#include "JhFactionMainLayer.h"
#include "MD5.h"
#include "JhGameScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#endif

JhFactionCreateLayer::JhFactionCreateLayer()
{
	m_action = 0;
}


JhFactionCreateLayer::~JhFactionCreateLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}


JhFactionCreateLayer* JhFactionCreateLayer::create(int action, FactionListData* modifyfdata)
{
	JhFactionCreateLayer *pRet = new JhFactionCreateLayer();
	if (pRet && pRet->init(action, modifyfdata))
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

bool JhFactionCreateLayer::init(int action, FactionListData* modifyfdata)
{
	m_action = action;
	m_modifyfdata = modifyfdata;
	Node* csbnode = CSLoader::createNode("jhfactionCreateLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::ImageView *title = (cocos2d::ui::ImageView*)csbnode->getChildByName("title");

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhFactionCreateLayer::onBack, this));

	m_createbtn = (cocos2d::ui::Button*)csbnode->getChildByName("createbtn");
	m_createbtn->addTouchEventListener(CC_CALLBACK_2(JhFactionCreateLayer::onCreateFaction, this));

	nameInput = (cocos2d::ui::TextField*)csbnode->getChildByName("name");
	nameInput->setString("");
	nameInput->setTag(0);
	nameInput->addEventListener(CC_CALLBACK_2(JhFactionCreateLayer::textFieldEvent, this));

	descinput = (cocos2d::ui::TextField*)csbnode->getChildByName("descinput");
	descinput->setString("");
	descinput->setTag(1);
	descinput->addEventListener(CC_CALLBACK_2(JhFactionCreateLayer::textFieldEvent, this));
	
	m_factionNameEdit = cocos2d::ui::EditBox::create(Size(380, 40), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui/blank.png"));
	m_factionNameEdit->setPosition(Point(165, 1110));
	m_factionNameEdit->setAnchorPoint(Vec2(0, 0.5));
	m_factionNameEdit->setFontColor(Color3B::BLACK);
	m_factionNameEdit->setPlaceHolder(JhCommonFuncs::gbk2utf("请输入帮派名称").c_str());
	m_factionNameEdit->setPlaceholderFontSize(30);
	m_factionNameEdit->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	m_factionNameEdit->setPlaceholderFontColor(Color3B(112,116,109));
	m_factionNameEdit->setMaxLength(8);
	m_factionNameEdit->setText("");
	m_factionNameEdit->setDelegate(this);
	csbnode->addChild(m_factionNameEdit);
	m_factionNameEdit->setVisible(false);

	for (int i = 0; i < 4; i++)
	{
		std::string nodestr = StringUtils::format("btn%d", i);
		lvlimitbtn[i] = (cocos2d::ui::Button*)csbnode->getChildByName("lvbtnnode")->getChildByName(nodestr);
		lvlimitbtn[i]->addTouchEventListener(CC_CALLBACK_2(JhFactionCreateLayer::onSelectLv, this));
		lvlimitbtn[i]->setTag(i);
	}
	lvlimitbtn[3]->setBright(false);
	for (int i = 0; i < 5; i++)
	{
		std::string nodestr = StringUtils::format("btn%d", i);
		sexlimitbtn[i] = (cocos2d::ui::Button*)csbnode->getChildByName("sexbtnnode")->getChildByName(nodestr);
		sexlimitbtn[i]->addTouchEventListener(CC_CALLBACK_2(JhFactionCreateLayer::onSelectSex, this));
		sexlimitbtn[i]->setTag(i);
	}
	sexlimitbtn[4]->setBright(false);

	selectlv = 0;
	selectsex = 4;

	JhGlobalData::g_gameStatus = GAMEPAUSE;

	if (m_action == 1)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		m_factionNameEdit->setEnabled(false);
		m_factionNameEdit->setText(modifyfdata->factionname.c_str());
#else
		nameInput->setEnabled(false);
		nameInput->setString(m_modifyfdata->factionname);
#endif
		title->loadTexture("ui/modifyftitle.png", cocos2d::ui::Widget::TextureResType::PLIST);
		m_createbtn->setTitleText(JhCommonFuncs::gbk2utf("修改"));
		m_createbtn->setEnabled(false);
		csbnode->getChildByName("ruletext")->setVisible(false);
		csbnode->getChildByName("rule")->setVisible(false);

		int lv = modifyfdata->lvlimit;
		int sex = modifyfdata->sexlimit;

		int lvl[] = { 30, 50, 80, 0 };
		int index = 0;
		for (int i = 0; i < 4; i++)
		{
			if (lv == lvl[i])
			{
				lvlimitbtn[i]->setBright(false);
				index = i;
			}
			else
				lvlimitbtn[i]->setBright(true);
		}
		selectlv = lvl[index];

		int sexl[] = { 1, 2, 3, 0, 4 };
		for (int i = 0; i < 5; i++)
		{
			if (sex == sexl[i])
			{
				sexlimitbtn[i]->setBright(false);
				index = i;
			}
			else
				sexlimitbtn[i]->setBright(true);
		}
		selectsex = sexl[index];
		descinput->setString(modifyfdata->desc);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	nameInput->setVisible(false);
	m_factionNameEdit->setVisible(true);
#endif


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhFactionCreateLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhFactionCreateLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhFactionCreateLayer::onCreateFaction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string factionname;

		std::string utf8name;
		std::string utf8desc = descinput->getString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		factionname = m_factionNameEdit->getText();
		utf8name = gbkToUTF8(factionname.c_str());
		utf8desc = gbkToUTF8(descinput->getString().c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		factionname = nameInput->getString();
		const char* retstr = "";
		JniMethodInfo methodInfo;
		char p_str[256] = { 0 };
		sprintf(p_str, "%s", factionname.c_str());
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

			retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
		}
		utf8name = retstr;

		sprintf(p_str, "%s", utf8desc.c_str());
		if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
		{
			jstring para1 = methodInfo.env->NewStringUTF(p_str);
			jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

			retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
		utf8desc = retstr;
#else
		factionname = nameInput->getString();
		utf8name = factionname;
#endif
		if (factionname.length() <= 0)
		{
			JhHintBox* hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("请输入帮派名称！！"));
			this->addChild(hintbox);
			return;
	}

		if (m_action == 0)
		{
			int mygold = JhGlobalData::getMyGoldCount();

			if (mygold >= 50)
			{
				if (JhGlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					JhGlobalData::dataIsModified = true;
					JhGlobalData::setMyGoldCount(0);
					JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}
				JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
				Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

				JhServerDataSwap::init(this)->createFaciton(utf8name, selectlv, selectsex, utf8desc);
			}
			else
			{
				JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("金元宝不足！！"));
				this->addChild(hint);
			}
		}
		else
		{
			JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			JhServerDataSwap::init(this)->modifyFaciton(m_modifyfdata->id, utf8name, selectlv, selectsex, utf8desc);

		}
	}
}

void JhFactionCreateLayer::onSuccess()
{
	if (m_action == 0)
	{
		JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - 50);
		int usegold = JhGlobalData::getUseGold() + 50;
		JhGlobalData::setUseGold(usegold);
	}
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhFactionMainLayer* fmlayer = (JhFactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmlayer != NULL)
	{
		fmlayer->getFactionListData();
	}
	this->removeFromParentAndCleanup(true);
}

void JhFactionCreateLayer::onErr(int errcode)
{
	if (m_action == 0)
	{
		if (errcode == -1 || errcode == -2)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("创建帮派失败，请检查网络设置或者联系客服！！"));
			this->addChild(box);
		}
		else if (errcode == -3)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("输入中包含敏感词！！"));
			this->addChild(box);
		}
		else if (errcode == -4)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("已创建过帮派！！"));
			this->addChild(box);
		}
		else if (errcode == -5)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("帮派名称已存在！！"));
			this->addChild(box);
		}
	}
	else
	{
		if (errcode == -3)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("输入中包含敏感词！！"));
			this->addChild(box);
		}
		else if (errcode == -5)
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("帮派名称已存在！！"));
			this->addChild(box);
		}
		else
		{
			Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
			JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("修改失败，请检查网络设置或者联系客服！！"));
			this->addChild(box);
		}
	}
}

void JhFactionCreateLayer::onSelectLv(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnode = (cocos2d::ui::Button*)pSender;
		int tag = btnode->getTag();
		int lvl[] = {30,50,80,0};
		for (int i = 0; i < 4; i++)
		{
			if (i == tag)
				lvlimitbtn[i]->setBright(false);
			else
				lvlimitbtn[i]->setBright(true);
		}
		selectlv = lvl[tag];
		m_createbtn->setEnabled(true);
	}

}

void JhFactionCreateLayer::onSelectSex(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btnode = (cocos2d::ui::Button*)pSender;
		int tag = btnode->getTag();
		int sexl[] = {1,2,3,0,4};
		for (int i = 0; i < 5; i++)
		{
			if (i == tag)
				sexlimitbtn[i]->setBright(false);
			else
				sexlimitbtn[i]->setBright(true);
		}
		selectsex = sexl[tag];
		m_createbtn->setEnabled(true);
	}
}

void JhFactionCreateLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{

}

void JhFactionCreateLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{

}

void JhFactionCreateLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
	m_createbtn->setEnabled(true);
}

void JhFactionCreateLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{

}

void JhFactionCreateLayer::textFieldEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	Node* node = (Node*)pSender;
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
	{
		cocos2d::ui::TextField * textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		std::string str = textField->getString();
		m_createbtn->setEnabled(true);
	}
	break;
	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		break;
	case cocos2d::ui::TextField::EventType::INSERT_TEXT:
		break;
	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
		break;
	}
}