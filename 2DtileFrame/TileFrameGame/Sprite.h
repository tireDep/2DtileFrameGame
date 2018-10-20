// Sprite class
#pragma once
#include <string>	// wstring 이용
#include <d3dx9.h>	// DirectX에러 해결
#include <vector>	

class Frame;	// 전방 선언
// 클래스 내에 다른 클래스 포인터 멤버변수 추가 -> 기본적으로 선언된것이 아니므로 컴파일러 에러남
/*
< 해결책 >
1) 헤더파일 선언 - 직접 알려주는 방법(직관적), but include 지옥
2) 전방선언 - 뭔지는 모르지만, 일단 넘어감 + 어딘가에 존재하므로 사용할 때 검색해봄

+) 이미 선언되어있는 헤더, 상속관련들 정도만 include 사용
*/

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init(std::wstring fileName, std::wstring scriptName, LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX);
	void Init(std::wstring fileName, int x, int y, int width, int height, float frameInterval, LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX);
	void Update(float deltaTime);	// 일반적으로 Render 위에 Update 함수 생성됨
	void Render();
	void Release(); 
	void Reset();

	void SetPosition(float x, float y);	// 타일맵 함수, 위치

private:
	IDirect3DTexture9* _textureDX;
	LPDIRECT3DDEVICE9 _dxDevice;
	ID3DXSprite* _spriteDX;

	std::wstring _fileName;
	D3DXIMAGE_INFO _texInfo;	// 파일로부터 이미지의 너비와 높이를 얻음
	// 멤버변수
	// 클래스가 필요한 속성

	std::vector<Frame*>_frameList;
	int _frameIndex;
	float _frameDuration;
	// Frame* _frame;	// 프레임 변수

	float _x;
	float _y;	// 위치 변수
};