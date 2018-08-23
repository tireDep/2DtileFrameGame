// GameTimer function
#include <Windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	_secondsPerCount = 1.0f / (double)countsPerSec;	
}

GameTimer::~GameTimer()
{
}

void GameTimer::Init()
{
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
	
	_prevCount = currentCount;
	_deltaTime = 0.0f;
}

void GameTimer::Update()
{
	__int64 currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);

	_deltaTime = (currentCount - _prevCount)*_secondsPerCount;
	_prevCount = currentCount;
}

float GameTimer::GetDeltaTimer()
{
	return _deltaTime;
}