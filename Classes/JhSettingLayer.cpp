#include "JhSettingLayer.h"
#include "JhCommonFuncs.h"
#include "JhSoundManager.h"
#include "JhGlobalData.h"
#include "JhGameDataSave.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif
#include "JhHintBox.h"
#include "JhWaitingProgress.h"

JhSettingLayer::JhSettingLayer()
{
}


JhSettingLayer::~JhSettingLayer()
{

}

bool JhSettingLayer::init()
{
	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	//加载csb文件
	Node* csbnode = CSLoader::createNode("jhsettingLayer.csb");
	this->addChild(csbnode);

	//关闭按钮
	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhSettingLayer::onBack, this));
	backbtn->setVisible(true);

	//checkbox
	m_soundCheckBox = (cocos2d::ui::CheckBox*)csbnode->getChildByName("soundcheck");
	m_soundCheckBox->addEventListener(CC_CALLBACK_2(JhSettingLayer::soundCheckBoxCallback, this));

	m_soundOnOffText = (cocos2d::ui::Text*)csbnode->getChildByName("settingdesc");

	updateSoundStatus();

	cocos2d::ui::Text* idlbl = (cocos2d::ui::Text*)csbnode->getChildByName("id");
	idlbl->setString(JhGlobalData::getMyID());

    mynamestr = JhGlobalData::getMyNickName();
    
	m_nameTextField = (cocos2d::ui::TextField*)csbnode->getChildByName("name");
	m_nameTextField->setString(mynamestr);
	m_nameTextField->addEventListener(CC_CALLBACK_2(JhSettingLayer::textFieldEvent, this));

	m_editName = cocos2d::ui::EditBox::create(Size(380, 44), cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("ui/blank.png"));
	m_editName->setPosition(Point(223, 818));
	m_editName->setAnchorPoint(Vec2(0, 0.5));
	m_editName->setFontColor(Color3B::BLACK);
	m_editName->setPlaceHolder(JhCommonFuncs::gbk2utf("请输入昵称:").c_str());
	m_editName->setPlaceholderFontSize(30);
	m_editName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
	m_editName->setPlaceholderFontColor(Color3B(112, 116, 109));
	m_editName->setMaxLength(12);
	m_editName->setText(mynamestr.c_str());
	//editName->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_editName->setDelegate(this);
	m_editName->setVisible(false);
	csbnode->addChild(m_editName);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//ios 恢复购买按钮
	cocos2d::ui::Button* resumebuybtn = (cocos2d::ui::Button*)csbnode->getChildByName("resumebuybtn");
	resumebuybtn->addTouchEventListener(CC_CALLBACK_2(JhSettingLayer::onResumeBuy, this));
	resumebuybtn->setVisible(true);
	m_nameTextField->setVisible(false);
	m_editName->setVisible(true);
#endif

	if (!JhGlobalData::isOnline)
	{
		m_nameTextField->setVisible(false);
		m_editName->setVisible(false);
	}


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

void JhSettingLayer::updateSoundStatus()
{
	if (JhSoundManager::getInstance()->getSoundIsOn())
	{
		m_soundCheckBox->setSelected(true);
		m_soundOnOffText->setString(JhCommonFuncs::gbk2utf("开"));
	}
	else
	{
		m_soundCheckBox->setSelected(false);
		m_soundOnOffText->setString(JhCommonFuncs::gbk2utf("关"));
	}
}

void JhSettingLayer::soundCheckBoxCallback(cocos2d::Ref* pSender, cocos2d::ui::CheckBox::EventType type)
{
	switch(type)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		case cocos2d::ui::CheckBox::EventType::SELECTED://选中
			JhSoundManager::getInstance()->setIsSoundOn(true);
			JhSoundManager::getInstance()->setIsMusicOn(true);
			JhSoundManager::getInstance()->playBackMusic(JhSoundManager::MUSIC_ID_START);
			//updateSoundStatus();
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED://不选中
			JhSoundManager::getInstance()->stopAllEffectSound();
			JhSoundManager::getInstance()->stopBackMusic();
			JhSoundManager::getInstance()->setIsSoundOn(false);
			JhSoundManager::getInstance()->setIsMusicOn(false);
			//updateSoundStatus();
			break;
		default:
			break;
	}
}

void JhSettingLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void JhSettingLayer::onResumeBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		restoreBuy();
	}
}
#endif

void JhSettingLayer::textFieldEvent(Ref * pSender, cocos2d::ui::TextField::EventType type)
{
	std::string str;
	cocos2d::ui::TextField * textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
	switch (type)
	{
		case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
			str = textField->getString();
			break;
		case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
		{
			str = textField->getString();
			editstr = str;
			std::string utf8str = str;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			const char* retstr = "";
			JniMethodInfo methodInfo;
			char p_str[64] = { 0 };
			sprintf(p_str, "%s", str.c_str());
			if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "gbkToUTF8", "(Ljava/lang/String;)Ljava/lang/String;"))
			{
				jstring para1 = methodInfo.env->NewStringUTF(p_str);
				jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, para1);

				retstr = methodInfo.env->GetStringUTFChars(jstr, 0);
			}
			utf8str = retstr;
#endif
			modifyName(utf8str);
		}
			break;
		case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			break;
		case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			break;
	}
}


void JhSettingLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{

}

void JhSettingLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	editstr = editBox->getText();
	std::string utf8str = editstr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    utf8str = gbkToUTF8(editstr.c_str());
#endif
	modifyName(utf8str);
}

void JhSettingLayer::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text)
{
}

void JhSettingLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{

}

void JhSettingLayer::modifyName(std::string utf8name)
{
	if (utf8name.length() > 0)
	{
		JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

		JhServerDataSwap::init(this)->modifyNickName(utf8name);
	}
}

void JhSettingLayer::onSuccess()
{

	JhGlobalData::setMyNickName(editstr);
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void JhSettingLayer::onErr(int errcode)
{
    Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox* hintbox;
	if (errcode == 2)
	{
		hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("抱歉！输入中包含敏感关键字，修改失败！"));
	}
	else if (errcode == 3)
	{
		hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("抱歉！该昵称已存在，大侠换一个吧！"));
	}
	else
	{
		hintbox = JhHintBox::create(JhCommonFuncs::gbk2utf("网络异常，修改失败！"));
	}
	this->addChild(hintbox);

	m_editName->setText(mynamestr.c_str());

}
