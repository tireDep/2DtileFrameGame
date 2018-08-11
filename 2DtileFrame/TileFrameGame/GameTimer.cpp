// GameTimer function
#include <Windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
{
	// 프레임 계산과 같음
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);	// (1초에 몇번 튀기는가)실제 하드웨어 cpu에서 계산된 값(cpu 성능 근거) -> 실제 시간 x
	_secondsPerCount = 1.0f / (double)countsPerSec;	// 시간 계산
}

GameTimer::~GameTimer()
{
}

void GameTimer::Init() // 초기화, 생성자랑 분리해서 -> 언제 초기화 할 지 모름 => 모듈화
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

	_deltaTime = (currentCount - _prevCount)*_secondsPerCount;	// 실제 시간
	_prevCount = currentCount;
}

float GameTimer::GetDeltaTimer()
{
	return _deltaTime;
}