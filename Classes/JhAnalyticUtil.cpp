#include "JhAnalyticUtil.h"
#include "JhConst.h"
#ifdef ANALYTICS
#include "MobClickCpp.h"

void JhAnalyticUtil::startLevel(const char * level) {
        umeng::MobClickCpp::startLevel(level);
    }
    
    void JhAnalyticUtil::failLevel(const char * level) {
        umeng::MobClickCpp::failLevel(level);
    }
    
    void JhAnalyticUtil::finishLevel(const char * level) {
        umeng::MobClickCpp::finishLevel(level);
    }
    
    void JhAnalyticUtil::pay(const char * item ,int money, int number)  {
        umeng::MobClickCpp::pay((double)money, 1, item, number, (double)0);
    }
    
    void JhAnalyticUtil::buy(const char * item, int number, float gem)
    {
        umeng::MobClickCpp::buy(item, number, (double)gem);
    }
    
    void JhAnalyticUtil::use(const char * item, int number) {
        umeng::MobClickCpp::use(item, number, (double)0);
    }
    
    void JhAnalyticUtil::bonus(const char * item, int num, int type)
    {
        umeng::MobClickCpp::bonus(item, num, (double)0, type);
    }
    
    void JhAnalyticUtil::onEvent(const char * eventId, const char * label) {
        umeng::MobClickCpp::event(eventId, label);
    }
    
    void JhAnalyticUtil::onEvent(const char * eventId, std::map<string, string> data) {
        umeng::eventDict dict;
        map<string, string>::iterator iter;
        for (iter = data.begin(); iter!=data.end(); iter++)
        {
            dict[iter->first] = dict[iter->second];
        }
        umeng::MobClickCpp::event(eventId, &dict);
    }
    
    void JhAnalyticUtil::setPlayerLevel(int level)
    {
        umeng::MobClickCpp::setUserLevel(level);
    }
#endif
    
