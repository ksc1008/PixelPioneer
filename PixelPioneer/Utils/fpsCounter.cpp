#include "fpsCounter.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void FpsCounter::setDelay(double delay_ms)
{
	delay_mean -= delays[latestCounter] / COUNTER_SIZE;
	delays[latestCounter] = delay_ms;
	delay_mean += delays[latestCounter] / COUNTER_SIZE;
	latestCounter = (latestCounter + 1) % COUNTER_SIZE;
}



double FpsCounter::checkDelayMean()
{
	return delay_mean;
}

int FpsCounter::getFPS()
{
	if (delay_mean == 0)
		return 0;
	return 1000 / delay_mean;
}

void FpsCounter::startStopwatch()
{
	m_startTime = glfwGetTime();
}

double FpsCounter::endStopwatch()
{
	auto result = (glfwGetTime() - m_startTime) * 1000;
	setDelay(result);
	return result;
}
