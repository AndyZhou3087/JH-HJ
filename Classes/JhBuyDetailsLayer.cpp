#include "JhBuyDetailsLayer.h"
#include "JhCommonFuncs.h"
#include "ShopLayer.h"
#include "SoundManager.h"
#include "JhGlobalData.h"
#include "JhConst.h"
JhBuyDetailsLayer::JhBuyDetailsLayer()
{

}


JhBuyDetailsLayer::~JhBuyDetailsLayer()
{
}

bool JhBuyDetailsLayer::init(int heroid)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhbuyDetailsLayer.csb");
	this->addChild(csbnode);
	image = (cocos2d::ui::ImageView*)csbnode->getChildByName("icon");
	nameTxt = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	descTxt = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	priceTxt = (cocos2d::ui::Text*) csbnode->getChildByName("price");

	goodstext = (cocos2d::ui::Text*) csbnode->getChildByName("goodsdesc");

	cocos2d::ui::ScrollView* m_scrollview = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");

	std::string goodsstr;
	goodsstr.append(JhCommonFuncs::gbk2utf("初始物品："));
	std::string strval = JhGlobalData::getOriginLocalStorage(heroid);

	std::vector<std::string> tmp;
	JhCommonFuncs::split(strval, tmp, ";");
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		std::vector<std::string> tmp2;
		JhCommonFuncs::split(tmp[i], tmp2, "-");
		std::string countstr = StringUtils::format("*%s", tmp2[2].c_str());
		std::string idstr = tmp2[0];
		std::string strr = StringUtils::format("ui/%s.png", tmp2[0].c_str());
		/*goodsstr.append(JhGlobalData::map_allResource[idstr].cname.c_str());
		goodsstr.append(countstr);
		if (i < tmp.size() - 1)
			goodsstr.append(JhCommonFuncs::gbk2utf("，"));*/

		Sprite* buildsmall = Sprite::createWithSpriteFrameName("ui/buildsmall.png");
		buildsmall->setScale(0.6f);
		buildsmall->setPosition(Vec2(43 + i % 5 * 83, 128 - i / 5 * 80));
		m_scrollview->addChild(buildsmall);

		Sprite* res = Sprite::createWithSpriteFrameName(strr);
		res->setScale(0.6f);
		res->setPosition(buildsmall->getPosition());
		m_scrollview->addChild(res);

		Label * reslbl = Label::createWithBMFont("fonts/herofont.fnt", countstr); //Label::createWithTTF(countstr, "fonts/SIMHEI.TTF", 20);
		reslbl->setPosition(Vec2(buildsmall->getPositionX() + 25, buildsmall->getPositionY() - buildsmall->getContentSize().height / 2 + 20));
		reslbl->setColor(Color3B(255, 255, 255));
		m_scrollview->addChild(reslbl);

	}
	//goodstext->setString(goodsstr);
	std::string imagepath = StringUtils::format("ui/tophero%d.png", heroid);
	image->loadTexture(imagepath, cocos2d::ui::TextureResType::PLIST);
	image->setContentSize(Sprite::createWithSpriteFrameName(imagepath)->getContentSize());

	nameTxt->setString(JhCommonFuncs::gbk2utf(heroname[heroid - 1].c_str()));
	descTxt->setString(JhCommonFuncs::gbk2utf(herodesc[heroid - 1].c_str()));
	std::string pricestr = StringUtils::format("%d元", heroprice[heroid - 1]);
	priceTxt->setString(JhCommonFuncs::gbk2utf(pricestr.c_str()));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhBuyDetailsLayer::onBack, this));

	cocos2d::ui::Button* buybtn = (cocos2d::ui::Button*)csbnode->getChildByName("buybtn");
	buybtn->addTouchEventListener(CC_CALLBACK_2(JhBuyDetailsLayer::onBuy, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

JhBuyDetailsLayer* JhBuyDetailsLayer::create(int heroid)
{
	JhBuyDetailsLayer *pRet = new JhBuyDetailsLayer();
	if (pRet && pRet->init(heroid))
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

void JhBuyDetailsLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void JhBuyDetailsLayer::onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ShopLayer::beginPay(this->getTag());
	}
}