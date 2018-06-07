#ifndef _energy_manager_h_
#define _energy_manager_h_

class EnergyManager{
public:
	~EnergyManager();
	static EnergyManager* getInstance();
	
	void init();
	void on1sTimer();

	void loadTime();
	void saveTime();

	inline long getCountDown() { return m_countdown; }

private:
	EnergyManager();

private:
	static EnergyManager* m_instance;
	long m_lastSaveTime;
	long m_countdown;
};

#endif