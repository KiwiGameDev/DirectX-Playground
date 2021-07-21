#pragma once
#include <chrono>
#include "Singleton.h"

class Time : public Singleton<Time>
{
	friend class Singleton<Time>;

public:
	void logStart();
	
	float deltaTime();
	float timeSinceApplicationStart();

	void logEnd();

private:
	Time();
	
	std::chrono::time_point<std::chrono::steady_clock> application_start;
	std::chrono::time_point<std::chrono::steady_clock> frame_start;
	std::chrono::time_point<std::chrono::steady_clock> frame_end;
	float delta_time = 0.0f;
};
