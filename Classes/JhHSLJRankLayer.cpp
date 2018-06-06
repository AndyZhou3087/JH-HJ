#include "JhHSLJRankLayer.h"
#include "JhCommonFuncs.h"
#include "JhHintBox.h"
#include "WaitingProgress.h"
#include "JhConst.h"
#include "SoundManager.h"
#include "JhHSLJMainLayer.h"
#include "JhGameScene.h"

JhHSLJRankLayer::JhHSLJRankLayer()
{

}


JhHSLJRankLayer::~JhHSLJRankLayer()
{
}


JhHSLJRankLayer* JhHSLJRankLayer::create()
{
	JhHSLJRankLayer *pRet = new JhHSLJRankLayer();
	if (pRet && pRet->init())
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

bool JhHSLJRankLayer::init()
{
	m_csbnode = CSLoader::createNode("jhHSLJRankLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhHSLJRankLayer::onBack, this));


	srollView = (cocos2d::ui::ScrollView*)m_csbnode->getChildByName("scrollview");
	srollView->setScrollBarEnabled(false);
	srollView->setBounceEnabled(true);

	getRankData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhHSLJRankLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhHSLJRankLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhHSLJRankLayer::getRankData()
{
	WaitingProgress* waitbox = WaitingProgress::create("排名中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	ServerDataSwap::init(this)->getHSLJRankData();
}

void JhHSLJRankLayer::delayShowData(float dt)
{

	srollView->removeAllChildrenWithCleanup(true);
	int size = JhGlobalData::vec_hsljRankData.size();

	int itemheight = 95;
	int innerheight = itemheight * size;

	int contentheight = srollView->getContentSize().height;
	srollView->setContentSize(Size(srollView->getContentSize().width, contentheight));
	if (innerheight < contentheight)
		innerheight = contentheight;
	srollView->setInnerContainerSize(Size(srollView->getContentSize().width, innerheight));

	myrankdata.rank = 0;
	myrankdata.nickname = JhGlobalData::getMyNickName();
	myrankdata.wincount = JhGlobalData::myMatchInfo.mywincount;
	myrankdata.totalcount = JhGlobalData::myMatchInfo.mywincount + JhGlobalData::myMatchInfo.myfailcount;
	myrankdata.exp = JhGlobalData::myMatchInfo.myexp;

	for (unsigned int i = 0; i < JhGlobalData::vec_hsljRankData.size(); i++)
	{
		HSLJRankItem* node = HSLJRankItem::create(&JhGlobalData::vec_hsljRankData[i]);
		node->setPosition(Vec2(srollView->getContentSize().width/2, innerheight - itemheight / 2 - i * itemheight));
		srollView->addChild(node);
	}

	this->removeChildByName("mynode");


	Node* mynode = HSLJRankItem::create(&myrankdata);
	mynode->setPosition(Vec2(360, 130));
	this->addChild(mynode, 0, "mynode");

	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
}

void JhHSLJRankLayer::onSuccess()
{
	this->scheduleOnce(schedule_selector(JhHSLJRankLayer::delayShowData), 0.1f);

}

void JhHSLJRankLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}

HSLJRankItem::HSLJRankItem()
{

}
HSLJRankItem::~HSLJRankItem()
{

}

bool HSLJRankItem::init(HSLJRankData *data)
{
	if (!Sprite::initWithSpriteFrameName("ui/blank.png"))
		return false;

	m_data = data;

	Node* csbnode = CSLoader::createNode("jhhsljRankNode.csb");//物品节点

	csbnode->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(csbnode);

	int rank = data->rank;

	std::string itemstr;


	if (data->nickname.compare(JhGlobalData::getMyNickName()) == 0)
	{
		JhHSLJRankLayer* rlayer = (JhHSLJRankLayer*)g_gameLayer->getChildByName("shljranklayer");
		rlayer->myrankdata.rank = rank;
		itemstr = "ui/hsljmyitem.png";

	}
	else if (rank % 2 == 0)
	{
		itemstr = "ui/hsljitem0.png";
	}

	if (itemstr.length() > 0)
	{
		cocos2d::ui::ImageView* item = (cocos2d::ui::ImageView*)csbnode->getChildByName("item");
		item->loadTexture(itemstr, cocos2d::ui::TextureResType::PLIST);
	}

	cocos2d::ui::TextBMFont* ranknumlbl = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("ranknum");
	std::string str = StringUtils::format("%d", rank);
	ranknumlbl->setString(str);

	if (rank <= 3 && rank > 0)
	{
		ranknumlbl->setVisible(false);
		std::string rankspritename = StringUtils::format("ui/ranknum%d.png", rank);
		Sprite* ranknum = Sprite::createWithSpriteFrameName(rankspritename);
		ranknum->setPosition(ranknumlbl->getPosition());
		csbnode->addChild(ranknum);
	}
	else if (rank == 0)
	{
		ranknumlbl->setString(JhCommonFuncs::gbk2utf("未上榜"));
		ranknumlbl->setScale(0.7f);
	}

	cocos2d::ui::Text* nicknamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("nickname");
	nicknamelbl->setString(data->nickname);

	if (data->totalcount > 0)
		str = StringUtils::format("%d%%", data->wincount * 100 / data->totalcount);
	else
		str = "0%";
	cocos2d::ui::Text* winpercentlbl = (cocos2d::ui::Text*)csbnode->getChildByName("winpercent");
	winpercentlbl->setString(str);

	int exp = data->exp;
	cocos2d::ui::Text* explbl = (cocos2d::ui::Text*)csbnode->getChildByName("exp");
	str = StringUtils::format("%d", exp);
	explbl->setString(str);

	str = JhHSLJMainLayer::getDwStr(exp);

	cocos2d::ui::Text* expnamelbl = (cocos2d::ui::Text*)csbnode->getChildByName("expname");
	expnamelbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));

	return true;
}

HSLJRankItem* HSLJRankItem::create(HSLJRankData *data)
{
	HSLJRankItem *pRet = new HSLJRankItem();
	if (pRet && pRet->init(data))
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
