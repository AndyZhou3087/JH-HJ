#ifndef _MYPARTICLE_H_
#define _MYPARTICLE_H_
#include "cocos2d.h"
#include "JhConst.h"
USING_NS_CC;

class JhMyParticle : public ParticleSystemQuad
{
public:

	static JhMyParticle* Show(Node* parent, const std::string& plistFile, int x, int y, bool isbleand, float duration);
	static JhMyParticle* create(const std::string& plistFile, bool isbleand, float duration);
	bool initwithdata(const std::string& plistFile, bool isbleand, float duration);

	JhMyParticle();
	
	~JhMyParticle();

private:

};

#endif