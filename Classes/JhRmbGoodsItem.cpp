#include "JhRmbGoodsItem.h"
#include "JhCommonFuncs.h"
#include "JhShopLayer.h"
#include "JhSoundManager.h"
JhRmbGoodsItem::JhRmbGoodsItem()
{
	isDraging = false;
}


JhRmbGoodsItem::~JhRmbGoodsItem()
{
}

bool JhRmbGoodsItem::init(GoodsData* gdata)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	Node* csbnode = CSLoader::createNode("jhrmbGoodsNode.csb");//物品节点
	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	Node* bgnode = csbnode->getChildByName("itembg");

	icon = (cocos2d::ui::ImageView*)bgnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)bgnode->getChildByName("name");

	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)bgnode->getChildByName("desc");
	desc->setString(gdata->desc);

	//图标
	std::string imagepath = StringUtils::format("ui/%s.png", gdata->img.c_str());
	icon->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	icon->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(gdata->name);
	std::string pricestr = StringUtils::format("%d元", gdata->price);

	cocos2d::ui::Button* bgbtn = (cocos2d::ui::Button*)csbnode->getChildByName("itembg");//整块节点击
	bgbtn->addTouchEventListener(CC_CALLBACK_2(JhRmbGoodsItem::onItem, this));
	bgbtn->setSwallowTouches(false);

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)bgnode->getChildByName("buybtn");//整块节点击
	buybtn->addTouchEventListener(CC_CALLBACK_2(JhRmbGoodsItem::onBuyBtn, this));
	buybtn->setSwallowTouches(false);
	buybtn->setTitleText(JhCommonFuncs::gbk2utf(pricestr.c_str()));
	return true;
}

JhRmbGoodsItem* JhRmbGoodsItem::create(GoodsData* gdata)
{
	JhRmbGoodsItem *pRet = new JhRmbGoodsItem();
	if (pRet && pRet->init(gdata))
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
void JhRmbGoodsItem::onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	onItem(pSender, type);
}

void JhRmbGoodsItem::onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		startPos = node->convertToWorldSpace(this->getParent()->getPosition());
	}
	else if (type == ui::Widget::TouchEventType::MOVED)
	{
		Vec2 pos = node->convertToWorldSpace(this->getParent()->getPosition());
		if (fabsf(pos.y - startPos.y) > 20)
			isDraging = true;
	}
	else if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!isDraging)
		{
			JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
			//购买
			JhShopLayer::beginPay(this->getTag());
		}
		isDraging = false;
	}
}