#pragma once
class FpsCounter
{
	double m_startTime = 0;

	static const int COUNTER_SIZE = 10;
	double delay_mean = 0;
	double delays[COUNTER_SIZE] = { 0 };
	int latestCounter = 0;

public:
	void setDelay(double delay);
	double checkDelayMean();
	int getFPS();

	void startStopwatch();
	double endStopwatch();
};

