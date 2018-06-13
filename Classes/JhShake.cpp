//
//  JhShake.cpp
//

#include "JhShake.h"

// not really useful, but I like clean default constructors
JhShake::JhShake() : _strength_x(0), _strength_y(0), _initial_x(0), _initial_y(0)
{
}

JhShake* JhShake::create(float d, float strength )
{
    // call other construction method with twice the same strength
    return create( d, strength, strength );
}

JhShake* JhShake::create(float duration, float strength_x, float strength_y)
{
    JhShake *p_action = new JhShake();
    p_action->initWithDuration(duration, strength_x, strength_y);
    p_action->autorelease();
    
    return p_action;
}

bool JhShake::initWithDuration(float duration, float strength_x, float strength_y)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        _strength_x = strength_x;
        _strength_y = strength_y;
        
        return true;
    }
    
    return false;
}

float fgRangeRand( float min, float max )
{
     float rnd = ((float)rand()/(float)RAND_MAX);
     return rnd*(max-min)+min;
}

void JhShake::update(float time)
{
     float randx = fgRangeRand( -_strength_x, _strength_x );
     float randy = fgRangeRand( -_strength_y, _strength_y );
    
     // move the target to a shaked position
     _target->setPosition(Vec2(_initial_x + randx,
                               _initial_y + randy));
}

JhShake* JhShake::clone(void) const
{
    auto a = new JhShake();
    a->initWithDuration(_duration, _strength_x, _strength_y);
    a->autorelease();
    return a;
}

JhShake* JhShake::reverse() const
{
    return JhShake::create(_duration, -_strength_x, -_strength_y);
}

void JhShake::startWithTarget(Node *target)
{
    CCActionInterval::startWithTarget(target);
    
    // save the initial position
    _initial_x = target->getPosition().x;
    _initial_y = target->getPosition().y;
}

void JhShake::stop(void)
{
    // Action is done, reset clip position
    _target->setPosition(Vec2( _initial_x, _initial_y ) );
    
    CCActionInterval::stop();
}