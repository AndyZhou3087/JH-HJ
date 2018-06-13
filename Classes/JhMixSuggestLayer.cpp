#include "JhMixSuggestLayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhSoundManager.h"
#include "JhWaitingProgress.h"
#include "JhGameDataSave.h"
#include "JhHintBox.h"
#include "MD5.h"

JhMixSuggestLayer::JhMixSuggestLayer()
{
	startime0 = 0;
	startime1 = 1;
	startime2 = 0;
}


JhMixSuggestLayer::~JhMixSuggestLayer()
{
	JhGlobalData::g_gameStatus = GAMESTART;
}

bool JhMixSuggestLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	csbroot = CSLoader::createNode("jhmixSuggestLayer.csb");
	this->addChild(csbroot);


	desc0 = (cocos2d::ui::Text*)csbroot->getChildByName("desc0");
	desc1 = (cocos2d::ui::Text*)csbroot->getChildByName("desc1");
	desc = (cocos2d::ui::Text*)csbroot->getChildByName("desc");

	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("mixbox_%d", i);

		imgbox[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str);
		img[i] = (cocos2d::ui::ImageView*)csbroot->getChildByName(str)->getChildByName("img");

		str = StringUtils::format("name%d", i);
		name[i] = (cocos2d::ui::Text*)csbroot->getChildByName(str);
	}

	freebtn = (cocos2d::ui::Button*)csbroot->getChildByName("freebtn");
	freebtn->addTouchEventListener(CC_CALLBACK_2(JhMixSuggestLayer::onFree, this));
	freebtn->setEnabled(false);

	silverbtn = (cocos2d::ui::Button*)csbroot->getChildByName("silverbtn");
	silverbtn->addTouchEventListener(CC_CALLBACK_2(JhMixSuggestLayer::onSilver, this));

	goldbtn = (cocos2d::ui::Button*)csbroot->getChildByName("goldbtn");
	goldbtn->addTouchEventListener(CC_CALLBACK_2(JhMixSuggestLayer::onGold, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbroot->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhMixSuggestLayer::onBack, this));

	freetimelbl0 = (cocos2d::ui::Text*)csbroot->getChildByName("freetime0");
	freetimelbl0->setVisible(false);

	freetimelbl1 = (cocos2d::ui::Text*)csbroot->getChildByName("freetime1");
	freetimelbl1->setVisible(false);

	freetimelbl2 = (cocos2d::ui::Text*)csbroot->getChildByName("freetime2");
	freetimelbl2->setVisible(false);


	mixname = (cocos2d::ui::Text*)csbroot->getChildByName("mixtitle");

	std::string suggestgfstr = JhGameDataSave::getInstance()->getSuggestMixGf();

	if (suggestgfstr.length() > 0)
		loadMixGfUi(JhGlobalData::map_MixGfData[suggestgfstr]);

	startime0 = JhGameDataSave::getInstance()->getFreeMixTime(0);
	startime1 = JhGameDataSave::getInstance()->getFreeMixTime(1);
	startime2 = JhGameDataSave::getInstance()->getFreeMixTime(2);

	getServerTime();

	JhGlobalData::g_gameStatus = GAMEPAUSE;
	auto m_listener = EventListenerTouchOneByOne::create();
	m_listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	m_listener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	return true;
}

void JhMixSuggestLayer::getServerTime()
{
	JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
	JhServerDataSwap::init(this)->getServerTime();
}

void JhMixSuggestLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhMixSuggestLayer::onExit()
{
	Layer::onExit();
}

void JhMixSuggestLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhMixSuggestLayer::onFree(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		freebtn->setEnabled(false);
		freetimelbl0->setVisible(true);
		startime0 = severtime;
		JhGameDataSave::getInstance()->setFreeMixTime(0, startime0);
		randMixGf(0);
	}
}

void JhMixSuggestLayer::onSilver(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)silverbtn->getChildByName("text");
		if (severtime - startime1 >= 86400)
		{
			silverbtn->getChildByName("silver")->setVisible(false);
			silverbtn->getChildByName("silvercount")->setVisible(false);
			freetimelbl1->setVisible(false);
			text->setPositionY(25);
			text->setFontSize(50);
			text->setString(JhCommonFuncs::gbk2utf("免费"));
			startime1 = severtime;
			JhGameDataSave::getInstance()->setFreeMixTime(1, startime1);
			randMixGf(1);
		}
		else
		{

			silverbtn->getChildByName("silver")->setVisible(true);
			silverbtn->getChildByName("silvercount")->setVisible(true);
			freetimelbl1->setVisible(true);
			text->setPositionY(32);
			text->setFontSize(42);
			text->setString(JhCommonFuncs::gbk2utf("刷新"));

			if (JhStorageRoom::getCountById("80") < 50)
			{
				JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("银两不足！"));
				this->addChild(hbox);
			}
			else
			{
				JhStorageRoom::use("80", 50);
				randMixGf(1);
			}
		}
	}
}

void JhMixSuggestLayer::onGold(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)goldbtn->getChildByName("text");
		if (severtime - startime2 >= 86400)
		{
			goldbtn->getChildByName("gold")->setVisible(false);
			goldbtn->getChildByName("goldcount")->setVisible(false);
			freetimelbl2->setVisible(false);
			text->setPositionY(25);
			text->setFontSize(50);
			text->setString(JhCommonFuncs::gbk2utf("免费"));
			startime2 = severtime;

			JhGameDataSave::getInstance()->setFreeMixTime(2, startime2);
			randMixGf(2);
		}
		else
		{
			goldbtn->getChildByName("gold")->setVisible(true);
			goldbtn->getChildByName("goldcount")->setVisible(true);
			freetimelbl2->setVisible(true);
			text->setPositionY(32);
			text->setFontSize(42);
			text->setString(JhCommonFuncs::gbk2utf("刷新"));

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

				JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - 50);
				randMixGf(2);
			}
			else
			{
				JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("金元宝不足！"));
				this->addChild(hbox);
			}
		}
	}
}

void JhMixSuggestLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

	severtime = JhGlobalData::servertime;

	this->unschedule(schedule_selector(JhMixSuggestLayer::updateServerTime));
	this->schedule(schedule_selector(JhMixSuggestLayer::updateServerTime), 1.0f);

	if (severtime - startime0 >= 86400)
	{
		freebtn->setEnabled(true);
		freetimelbl0->setVisible(false);
	}

	if (severtime - startime1 >= 86400)
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)silverbtn->getChildByName("text");
		silverbtn->getChildByName("silver")->setVisible(false);
		silverbtn->getChildByName("silvercount")->setVisible(false);
		freetimelbl1->setVisible(false);
		text->setPositionY(25);
		text->setFontSize(50);
		text->setString(JhCommonFuncs::gbk2utf("免费"));
	}
	if (severtime - startime2 >= 86400)
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)goldbtn->getChildByName("text");
		goldbtn->getChildByName("gold")->setVisible(false);
		goldbtn->getChildByName("goldcount")->setVisible(false);
		freetimelbl2->setVisible(false);
		text->setPositionY(25);
		text->setFontSize(50);
		text->setString(JhCommonFuncs::gbk2utf("免费"));
	}
}

void JhMixSuggestLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");

}

void JhMixSuggestLayer::updateServerTime(float dt)
{
	severtime++;

	if (severtime - startime0 >= 86400)
	{
		freebtn->setEnabled(true);
		freetimelbl0->setVisible(false);
	}
	else
	{
		freebtn->setEnabled(false);
		freetimelbl0->setVisible(true);

		int lefttime = 86400 - (severtime - startime0);
		std::string str = StringUtils::format("%02d:%02d:%02d", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl0->setString(str);
	}

	if (severtime - startime1 >= 86400)
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)silverbtn->getChildByName("text");
		silverbtn->getChildByName("silver")->setVisible(false);
		silverbtn->getChildByName("silvercount")->setVisible(false);
		freetimelbl1->setVisible(false);
		text->setPositionY(25);
		text->setFontSize(50);
		text->setString(JhCommonFuncs::gbk2utf("免费"));
	}
	else
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)silverbtn->getChildByName("text");
		silverbtn->getChildByName("silver")->setVisible(true);
		silverbtn->getChildByName("silvercount")->setVisible(true);
		freetimelbl1->setVisible(true);
		text->setPositionY(32);
		text->setFontSize(42);
		text->setString(JhCommonFuncs::gbk2utf("刷新"));

		int lefttime = 86400 - (severtime - startime1);
		std::string str = StringUtils::format("%02d:%02d:%02d后免费", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl1->setString(JhCommonFuncs::gbk2utf(str.c_str()));
	}

	if (severtime - startime2 >= 86400)
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)goldbtn->getChildByName("text");
		goldbtn->getChildByName("gold")->setVisible(false);
		goldbtn->getChildByName("goldcount")->setVisible(false);
		freetimelbl2->setVisible(false);
		text->setPositionY(25);
		text->setFontSize(50);
		text->setString(JhCommonFuncs::gbk2utf("免费"));
	}
	else
	{
		cocos2d::ui::Text* text = (cocos2d::ui::Text*)goldbtn->getChildByName("text");
		goldbtn->getChildByName("gold")->setVisible(true);
		goldbtn->getChildByName("goldcount")->setVisible(true);
		freetimelbl2->setVisible(true);
		text->setPositionY(32);
		text->setFontSize(42);

		int lefttime = 86400 - (severtime - startime2);
		std::string str = StringUtils::format("%02d:%02d:%02d后免费", lefttime / 3600, lefttime % 3600 / 60, lefttime % 3600 % 60);
		freetimelbl2->setString(JhCommonFuncs::gbk2utf(str.c_str()));
		text->setString(JhCommonFuncs::gbk2utf("刷新"));
	}
}

void JhMixSuggestLayer::randMixGf(int type)
{
	int count = JhGameDataSave::getInstance()->getMixGfCountByType(type);

	std::map<int, std::vector<MixGfData>> map_bylvMixGf;
	std::map<std::string, MixGfData>::iterator it;

	for (it = JhGlobalData::map_MixGfData.begin(); it != JhGlobalData::map_MixGfData.end(); ++it)
	{
		int lv = JhGlobalData::map_MixGfData[it->first].lv;
		map_bylvMixGf[lv].push_back(JhGlobalData::map_MixGfData[it->first]);
	}
	int r[][4] = { { 60, 100, 100, 100 }, { 40, 70, 98, 100 }, { 0, 50, 95, 100 } };

	int lvrand = JhGlobalData::createRandomNum(100);

	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		if (lvrand < r[type][i])
		{
			index = i;
			break;
		}
	}
	if (count > 0 && count % 10 == 0)
	{
		if (type == 0)
			index = 1;
		else if (type == 1)
			index = 2;
		else if (type == 2)
			index = 3;
	}

	int size = map_bylvMixGf[index + 1].size();
	int gfrand = JhGlobalData::createRandomNum(size);

	MixGfData mixdata = map_bylvMixGf[index + 1][gfrand];

	JhGameDataSave::getInstance()->setMixGfCountByType(type, count + 1);
	JhGameDataSave::getInstance()->setSuggestMixGf(mixdata.id);

	loadMixGfUi(mixdata);
}

void JhMixSuggestLayer::loadMixGfUi(MixGfData mixdata)
{
	std::string suggestgfstr = JhGameDataSave::getInstance()->getSuggestMixGf();
	mixname->setString(JhGlobalData::map_MixGfData[suggestgfstr].name);
	std::string str = StringUtils::format("ui/%s.png", mixdata.mastergf.c_str());
	img[0]->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
	name[0]->setString(JhGlobalData::map_allResource[mixdata.mastergf].cname);

	std::string sstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[mixdata.mastergf].qu);
	imgbox[0]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);

	int secsize = mixdata.vec_secgf.size();
	for (int i = 0; i < 3; i++)
	{
		if (i < secsize)
		{
			str = StringUtils::format("ui/%s.png", mixdata.vec_secgf[i].c_str());
			sstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[mixdata.vec_secgf[i]].qu);
			name[i + 1]->setString(JhGlobalData::map_allResource[mixdata.vec_secgf[i]].cname);
		}
		else
		{
			str = "ui/mixsectext.png";
			sstr = "ui/buildsmall.png";
			name[i + 1]->setString("");
		}
		img[i + 1]->loadTexture(str, cocos2d::ui::Widget::TextureResType::PLIST);
		imgbox[i+1]->loadTexture(sstr, cocos2d::ui::TextureResType::PLIST);
	}
	updateDesc();
}

void JhMixSuggestLayer::updateDesc()
{
	std::string mixid = JhGameDataSave::getInstance()->getSuggestMixGf();
	if (mixid.length() > 0)
	{
		MixGfData mixdata = JhGlobalData::map_MixGfData[mixid];
		desc0->setVisible(false);
		desc1->setVisible(false);
		std::string atkstr;
		std::string dfstr;
		std::string critstr;
		std::string dodgestr;
		std::string maxhpstr;
		float atk = mixdata.atkpercent;
		float df = mixdata.dfpercent;
		float crit = mixdata.critpercent;
		float dodge = mixdata.dodgepercent;
		float maxhp = mixdata.hppercent;

		std::string desstr;
		if (atk > 0.0f)
		{
			atkstr = StringUtils::format("+%.2f%%", atk);
			desstr.append(JhCommonFuncs::gbk2utf("大幅增加攻击"));
		}
		else if (atk < 0.0f)
		{
			atkstr = StringUtils::format("%.2f%%", atk);
			desstr.append(JhCommonFuncs::gbk2utf("大幅降低攻击"));
		}

		if (df > 0.0f)
		{
			dfstr = StringUtils::format("+%.2f%%", df);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅增加防御"));
		}
		else if (df < 0.0f)
		{
			dfstr = StringUtils::format("%.2f%%", df);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅降低防御"));
		}

		if (crit > 0.0f)
		{
			critstr = StringUtils::format("+%.2f%%", crit);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅增加暴击率"));
		}
		else if (crit < 0.0f)
		{
			critstr = StringUtils::format("%.2f%%", crit);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅降低暴击率"));
		}

		if (dodge > 0.0f)
		{
			dodgestr = StringUtils::format("+%.2f%%", dodge);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅增加闪避率"));
		}
		else if (dodge < 0.0f)
		{
			dodgestr = StringUtils::format("%.2f%%", dodge);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅降低闪避率"));
		}

		if (maxhp > 0.0f)
		{
			maxhpstr = StringUtils::format("+%.2f%%", maxhp);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅增加最大气血。"));
		}
		else if (maxhp < 0.0f)
		{
			maxhpstr = StringUtils::format("%.2f%%", maxhp);

			if (desstr.length() > 0)
				desstr.append(JhCommonFuncs::gbk2utf("，"));
			desstr.append(JhCommonFuncs::gbk2utf("大幅降低最大气血。"));
		}
		desc->setString(desstr);
		desc->setVisible(true);
	}
	else
	{
		desc0->setVisible(true);
		desc1->setVisible(true);
		desc->setVisible(false);
	}
}
