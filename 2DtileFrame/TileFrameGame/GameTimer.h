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
	float _secondsPerCount;	// window �ð� ������
	float _deltaTime;	// deltaTime
	float _prevCount;	// deltaTime ���� 
};