#include "JhSelectSaveLayer.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhGameDataSave.h"
#include "Utility.h"
#include "JhConst.h"
#include "JhSelectSaveComfirmLayer.h"
#include "JhCommonFuncs.h"
bool JhSelectSaveLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("jhselectSavedLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhSelectSaveLayer::onBack, this));

	std::vector<std::string> saveids = JhGlobalData::getSaveListId();
	for (int i = 0; i < 4; i++)
	{
		std::string nodestr = StringUtils::format("save%d", i);
		m_saveNode[i] = csbnode->getChildByName(nodestr);

		cocos2d::ui::Button* selectbtn = (cocos2d::ui::Button*)m_saveNode[i]->getChildByName("btn");

		cocos2d::ui::Widget* itembg = (cocos2d::ui::Widget*)selectbtn->getChildByName("saveitembg");

		cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)itembg->getChildByName("head");
		std::string headstr = StringUtils::format("ui/tophero%d.png", i+1);
		headimg->loadTexture(headstr, cocos2d::ui::TextureResType::PLIST);

		cocos2d::ui::Widget* textnode = (cocos2d::ui::Widget*)selectbtn->getChildByName("textnode");

		if (saveids[i].size() <= 0)
		{
			itembg->setOpacity(150);
			textnode->setVisible(false);
			m_saveNode[i]->getChildByName("nodatatext")->setVisible(true);
			
		}
		else
		{
			selectbtn->addTouchEventListener(CC_CALLBACK_2(JhSelectSaveLayer::onSelect, this));
			selectbtn->setTag(i);
			textnode->setVisible(true);
			cocos2d::ui::Text* nametxt = (cocos2d::ui::Text*)textnode->getChildByName("name");
			nametxt->setString(JhCommonFuncs::gbk2utf(heroname[i].c_str()));

			std::string str = StringUtils::format("%sherolv", saveids[i].c_str());
			int lv = loadIntDataByKey(str, 0);
			str = StringUtils::format("%d", lv + 1);
			cocos2d::ui::Text* lvtxt = (cocos2d::ui::Text*)textnode->getChildByName("lv");
			lvtxt->setString(str);

			str = StringUtils::format("%slivedays", saveids[i].c_str());
			int daycount = loadIntDataByKey(str, 0);
			str = StringUtils::format("%d", daycount);
			cocos2d::ui::Text* daytxt = (cocos2d::ui::Text*)textnode->getChildByName("day");
			daytxt->setString(str);
		}
	}

	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void JhSelectSaveLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void JhSelectSaveLayer::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		JhSelectSaveComfirmLayer* layer = JhSelectSaveComfirmLayer::create(node->getTag());
		this->addChild(layer);
	}
}

void JhSelectSaveLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}
