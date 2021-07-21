#include "Time.h"

Time* Singleton<Time>::instance = nullptr;

Time::Time()
{
	application_start = std::chrono::steady_clock::now();
}

void Time::logStart()
{
	frame_start = std::chrono::steady_clock::now();
}

float Time::deltaTime()
{
	return delta_time;
}

float Time::timeSinceApplicationStart()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(frame_end - application_start).count() / 1000000.0f;
}

void Time::logEnd()
{
	frame_end = std::chrono::steady_clock::now();
	delta_time = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start).count() / 1000000.0f;
}
