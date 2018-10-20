#pragma once
#include <d3dx9.h>
#include <string.h>

class Sprite;

class Map
{
public:
	Map();
	~Map();

	void Init(LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX);
	void Update(float deltaTime);
	void Render();
	void Realease();
	void Reset();
	void Deinit();

private:
	Sprite *_tileSprite[16][16];	// 여러 개 동시 저장 위해 배열 선언
	std::vector<Sprite*>_spriteList;	
	// 배열 상위호환!, Sprite *_spriteList[]; : 어느 정도인지 모르기 때문에 포인터형 or 한정된 크기
	// 배열은 아닌데 배열처럼 사용해도 에러 x
};