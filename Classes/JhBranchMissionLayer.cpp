#include "JhBranchMissionLayer.h"
#include "JhCommonFuncs.h"
#include "JhGlobalData.h"
#include "JhGameDataSave.h"

JhBranchMissionLayer::JhBranchMissionLayer()
{
}


JhBranchMissionLayer::~JhBranchMissionLayer()
{

}

bool JhBranchMissionLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhbranchMissionLayer.csb");
	this->addChild(csbnode);
	
	cocos2d::ui::Text* mapTxt = (cocos2d::ui::Text*)csbnode->getChildByName("maptxt");
	cocos2d::ui::Text* npcTxt = (cocos2d::ui::Text*)csbnode->getChildByName("npctxt");

	cocos2d::ui::Widget* onDropbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("dropbtn");
	onDropbtn->addTouchEventListener(CC_CALLBACK_2(JhBranchMissionLayer::onDropMisson, this));

	cocos2d::ui::Widget* onClosebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("closebtn");
	onClosebtn->addTouchEventListener(CC_CALLBACK_2(JhBranchMissionLayer::onClose, this));


	std::string mid = JhGlobalData::getCurBranchPlotMissison();

	int subindex = JhGlobalData::map_BranchPlotMissionItem[mid].subindex;
	PlotMissionData pd = JhGlobalData::map_BranchPlotMissionData[mid][subindex];
	npcTxt->setString(JhGlobalData::map_npcs[pd.snpc].name);

	std::map<std::string, MapData>::iterator it;

	for (it = JhGlobalData::map_maps.begin(); it != JhGlobalData::map_maps.end(); ++it)
	{
		for (unsigned int i = 0; i < JhGlobalData::map_maps[it->first].npcs.size(); i++)
		{
			if (JhGlobalData::map_maps[it->first].npcs[i].compare(pd.snpc) == 0)
			{
				mapTxt->setString(JhGlobalData::map_maps[it->first].cname);
				break;
			}
		}
		
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

JhBranchMissionLayer* JhBranchMissionLayer::create()
{
	JhBranchMissionLayer *pRet = new JhBranchMissionLayer();
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

void JhBranchMissionLayer::onDropMisson(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string curmid = JhGlobalData::getCurBranchPlotMissison();
		if (curmid.length() > 0)
		{
			if (JhGlobalData::map_BranchPlotMissionItem[curmid].count > 0)
				JhGlobalData::map_BranchPlotMissionItem[curmid].count--;
			JhGlobalData::map_BranchPlotMissionItem[curmid].time = JhGlobalData::map_BranchPlotMissionItem[curmid].maxtime;

			int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
			JhGlobalData::map_BranchPlotMissionData[curmid][subindex].status = M_NONE;

			JhGlobalData::saveBranchPlotMissionStatus("", 0);
			JhGameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
		}
		this->removeFromParentAndCleanup(true);
	}
}

void JhBranchMissionLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}