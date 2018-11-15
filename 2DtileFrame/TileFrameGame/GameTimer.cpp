// GameTimer function
#include <Windows.h>
#include "GameTimer.h"

GameTimer::GameTimer()
{
	// ������ ���� ����
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);	// (1�ʿ� ��� Ƣ��°�)���� �ϵ���� cpu���� ���� ��(cpu ���� �ٰ�) -> ���� �ð� x
	_secondsPerCount = 1.0f / (double)countsPerSec;	// �ð� ���
}

GameTimer::~GameTimer()
{
}

void GameTimer::Init() // �ʱ�ȭ, �����ڶ� �и��ؼ� -> ���� �ʱ�ȭ �� �� �� => ���ȭ
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

	_deltaTime = (currentCount - _prevCount)*_secondsPerCount;	// ���� �ð�
	_prevCount = currentCount;
}

float GameTimer::GetDeltaTimer()
{
	return _deltaTime;
}