#include "JhMyParticle.h"


JhMyParticle::JhMyParticle()
{

}

JhMyParticle::~JhMyParticle()
{

}

JhMyParticle * JhMyParticle::create(const std::string& plistFile, bool isbleand, float duration)
{
	JhMyParticle *ret = new (std::nothrow) JhMyParticle();
	if (ret && ret->initwithdata(plistFile, isbleand, duration))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}

bool JhMyParticle::initwithdata(const std::string& plistFile, bool isbleand, float duration)
{
	if (!initWithFile(plistFile))
		return false;
	setBlendAdditive(isbleand);
	setDuration(duration);
	setAutoRemoveOnFinish(true);
	return true;
}

JhMyParticle* JhMyParticle::Show(Node* parent, const std::string& plistFile, int x, int y, bool isbleand, float duration)
{
	JhMyParticle* particle = create(plistFile, isbleand, duration);
	particle->setPosition(Vec2(x, y));
	parent->addChild(particle, 1000);
	return particle;
}