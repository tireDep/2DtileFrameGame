// GameTimer class
#pragma once

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	void Init();
	void Update();
	float GetDeltaTimer();

private:
	float _secondsPerCount;	// window 시간 가져옴
	float _deltaTime;	// deltaTime
	float _prevCount;	// deltaTime 계산용 
};