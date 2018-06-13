#include "JhMyMenu.h"

bool JhMyMenu::init()
{
	return initWithArray(Vector<MenuItem*>());
}
 
JhMyMenu::JhMyMenu()
{
	m_szTouchLimitNode = NULL;
	m_bTouchLimit = false;
	m_isdraging = false;
}

JhMyMenu::~JhMyMenu()
{
	m_szTouchLimitNode = NULL;
}

bool JhMyMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    if (Layer::init())
    {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
 
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
 
        setPosition(Vec2(s.width/2, s.height/2));
 
        int z=0;
 
        for (auto& item : arrayOfItems)
        {
            this->addChild(item, z);
            z++;
        }
 
        _selectedItem = nullptr;
        _state = Menu::State::WAITING;
 
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
 
 
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(false);
 
		touchListener->onTouchBegan = CC_CALLBACK_2(JhMyMenu::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(JhMyMenu::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(JhMyMenu::onTouchEnded, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(JhMyMenu::onTouchCancelled, this);
 
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
 
        return true;
    }
    return false;
}
 
JhMyMenu* JhMyMenu::createWithArray( const Vector<MenuItem*>& arrayOfItems )
{
	auto ret = new JhMyMenu();
    if (ret && ret->initWithArray(arrayOfItems))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
 
    return ret;
}
 
JhMyMenu* JhMyMenu::createWithItem( MenuItem* item )
{
	return JhMyMenu::create(item, nullptr);
}
 
JhMyMenu* JhMyMenu::createWithItems( MenuItem *item, va_list args )
{
	Vector<MenuItem*> items;
    if( item )
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
 
	return JhMyMenu::createWithArray(items);
}
 
JhMyMenu* JhMyMenu::create( MenuItem* item, ... )
{
    va_list args;
    va_start(args,item);
 
	JhMyMenu *ret = JhMyMenu::createWithItems(item, args);
 
    va_end(args);
 
    return ret;
}

JhMyMenu* JhMyMenu::create()
{
	return JhMyMenu::create(nullptr, nullptr);
}

bool JhMyMenu::onTouchBegan(Touch* touch, Event* event)
{
	startY = touch->getLocation().y;
	auto camera = Camera::getVisitingCamera();
	if (_state != Menu::State::WAITING || !_visible || !_enabled || !camera)
	{
		return false;
	}

	for (Node *c = this->_parent; c != nullptr; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	if (isInTouchLimit(touch))
	{
		return false;
	}
	
	_selectedItem = this->getItemForTouch(touch, camera);
	if (_selectedItem)
	{
		_state = Menu::State::TRACKING_TOUCH;
		_selectedWithCamera = camera;
		_selectedItem->selected();

		return true;
	}
	return false;
}


void JhMyMenu::onTouchEnded(Touch* touch, Event* event)
{
	//CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
	if (_state != Menu::State::TRACKING_TOUCH)
		return;
	if (!m_isdraging)
	{
		this->retain();
		if (_selectedItem)
		{
			_selectedItem->unselected();
			_selectedItem->activate();
		}
		_state = Menu::State::WAITING;
		_selectedWithCamera = nullptr;
		this->release();
	}
	else
	{
		onTouchCancelled(touch, event);
		m_isdraging = false;
	}
}

void JhMyMenu::onTouchCancelled(Touch* touch, Event* event)
{
	//CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
	if (_state != Menu::State::TRACKING_TOUCH)
		return;
	this->retain();
	if (_selectedItem)
	{
		_selectedItem->unselected();
	}
	_state = Menu::State::WAITING;
	this->release();
}

void JhMyMenu::onTouchMoved(Touch* touch, Event* event)
{
	//CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
	if (_state != Menu::State::TRACKING_TOUCH)
		return;

	if (fabsf(startY - touch->getLocation().y) > 20)
		m_isdraging = true;


	MenuItem *currentItem = this->getItemForTouch(touch, _selectedWithCamera);
	if (currentItem != _selectedItem)
	{
		if (_selectedItem)
		{
			_selectedItem->unselected();
		}
		_selectedItem = currentItem;
		if (_selectedItem)
		{
			_selectedItem->selected();
		}
	}
}
void JhMyMenu::setTouchlimit(cocos2d::Node *node)
{
	m_szTouchLimitNode = node;
	m_bTouchLimit = true;
}

bool JhMyMenu::isInTouchLimit(Touch* touch)
{
	if (m_bTouchLimit)
	{
		Vec2 touchLocation = touch->getLocation();

		Vec2 local = m_szTouchLimitNode->convertToNodeSpace(touchLocation);
		Rect r = m_szTouchLimitNode->getBoundingBox();
		r.origin = Vec2::ZERO;

		if (!r.containsPoint(local))
		{
			return true;
		}
	}
	return false;
}