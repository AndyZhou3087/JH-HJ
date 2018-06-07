#include "JhSoundManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

static JhSoundManager* g_sharedManager = NULL;

JhSoundManager* JhSoundManager::getInstance()
{
	//获取instance;
    if(!g_sharedManager)
    {
        g_sharedManager = new JhSoundManager();
        g_sharedManager->init();

    }
    
    return g_sharedManager;
}

Ref* JhSoundManager::init()
{
	_isSoundOn = UserDefault::getInstance()->getBoolForKey("issoundon", true);
	_isMusicOn = UserDefault::getInstance()->getBoolForKey("ismusicon", true);
	_isMusicPlayed = false;
    return this;
}

void JhSoundManager::loadSounds()
{
    // TODO :插入soundid列表
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_BUTTON, "sounds/button.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_CAIJI, "sounds/caiji.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_KANSHU, "sounds/kanshu.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_WAJUE, "sounds/wajue.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_ATTACK, "sounds/attack.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_HURT0, "sounds/hurt0.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_HURT1, "sounds/hurt1.mp3"));
	_soundIds.insert(SoundIdMapType::value_type((int)SOUND_ID_BUYOK, "sounds/buyok.mp3"));
	//插入背景音乐列表
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_START, "sounds/start.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_HOME, "sounds/home.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_MAP, "sounds/map.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_ENTER_MAPADDR, "sounds/mapaddr.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_FIGHT_0, "sounds/fight0.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_FIGHT_1, "sounds/fight1.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_FIGHT_2, "sounds/fight2.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_FIGHT_3, "sounds/fight3.mp3"));
	_musicIds.insert(SoundIdMapType::value_type((int)MUSIC_ID_DEATH, "sounds/death.mp3"));


	//预加载音效
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/button.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/caiji.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/kanshu.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/wajue.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/attack.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/hurt0.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/hurt1.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/buyok.mp3");
}

void JhSoundManager::unloadSounds()
{
	_soundIds.clear();
	_musicIds.clear();
}

int JhSoundManager::playSound(int soundId, bool isloop)
{

	if(!_isSoundOn)
	{
	    return -1;
	}
    
    SoundIdMapType::iterator it = _soundIds.find(soundId);
    if (it == _soundIds.end()) {
        return -1;
    }
    
    std::string soundName = it->second;
    SimpleAudioEngine* soundEngine = SimpleAudioEngine::getInstance();
	return soundEngine->playEffect(soundName.c_str(), isloop);
}

bool JhSoundManager::getSoundIsOn()
{
	return _isSoundOn;
}

bool JhSoundManager::getMusicPlayed()
{
	return _isMusicPlayed;
}

void JhSoundManager::playBackMusic(const char * filename)
{
	if (_isMusicOn)
    {
		_isMusicPlayed = true;
        SimpleAudioEngine::getInstance()->playBackgroundMusic(filename,true);
    }
}

void JhSoundManager::playBackMusic(int musicid)
{
	if (_isMusicOn)
	{
		MusicIdMapType::iterator it = _musicIds.find(musicid);
		if (it == _musicIds.end()) {
			return;
		}

		_isMusicPlayed = true;

		std::string musciName = it->second;
		SimpleAudioEngine::getInstance()->playBackgroundMusic(musciName.c_str(), true);
	}
}

void JhSoundManager::stopBackMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}


void JhSoundManager::pauseBackMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void JhSoundManager::resumeBackMusic()
{
	if (_isMusicOn)
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void JhSoundManager::setIsMusicOn(bool isMusicOn)
{
	_isMusicOn = isMusicOn;
	UserDefault::getInstance()->setBoolForKey("ismusicon", _isMusicOn);
}

void JhSoundManager::setIsSoundOn(bool isSoundOn)
{
    _isSoundOn = isSoundOn;
	UserDefault::getInstance()->setBoolForKey("issoundon", _isSoundOn);
}

void JhSoundManager::stopSound(int soundId)
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->stopEffect(soundId);

}
void JhSoundManager::stopAllEffectSound()
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->stopAllEffects();
}
void JhSoundManager::pauseAllEffectSound()
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->pauseAllEffects();
}
void JhSoundManager::resumeAllEffectSound()
{
	if (_isSoundOn)
		SimpleAudioEngine::getInstance()->resumeAllEffects();
}

float JhSoundManager::getVolume()
{
    return SimpleAudioEngine::getInstance()->getEffectsVolume();
}

void JhSoundManager::setVolume(float volume)
{
    SimpleAudioEngine* soundEngine = SimpleAudioEngine::getInstance();
    soundEngine->setEffectsVolume(volume);
    soundEngine->setBackgroundMusicVolume(volume);
}

void JhSoundManager::saveVolume()
{
    UserDefault::getInstance()->setFloatForKey("volume", getVolume());
}

int JhSoundManager::getSaveVolume()
{
    return UserDefault::getInstance()->getFloatForKey("volume", getVolume());
}
