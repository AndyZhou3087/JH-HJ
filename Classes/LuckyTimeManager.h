#ifndef _luckytime_manager_h_
#define _luckytime_manager_h_

class LuckyTimeManager{
public:
	~LuckyTimeManager();
	static LuckyTimeManager* getInstance();

	void init();
	void on1sTimer();

	void loadTime();
	void saveTime();

	inline bool isLotteryed() { return m_bLotteryFlag; }
	inline bool isUnLotteryed() { return !m_bLotteryFlag; }
	inline long getCountDown() { return m_countdown; }

private:
	LuckyTimeManager();

private:
	static LuckyTimeManager* m_instance;
	long long m_lastLuckyTime;
	long m_countdown;
	bool m_bLotteryFlag;
};

#endif