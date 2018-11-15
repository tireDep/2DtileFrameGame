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
	Sprite *_tileSprite[16][16];	// ���� �� ���� ���� ���� �迭 ����
	std::vector<Sprite*>_spriteList;	
	// �迭 ����ȣȯ!, Sprite *_spriteList[]; : ��� �������� �𸣱� ������ �������� or ������ ũ��
	// �迭�� �ƴѵ� �迭ó�� ����ص� ���� x
};