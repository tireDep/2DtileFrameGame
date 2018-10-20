#include"Sprite.h"
#include "Map.h"	
// 관련 인클루드는 맨 아래에 두는 것이 인클루드 관리에 편함

Map::Map()
{
}

Map::~Map()
{
}

void Map::Init(LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	int tileMapIndex[16][16];	// 타일맵 변수
	int idx = 0;	// 순서 체크 변수

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			// tileMapIndex[y][x] = rand() % 4;	// 랜덤으로 0 ~ 3
			tileMapIndex[y][x] = (idx++) % 4;	// 순서대로 0 ~ 3 저장
		}
	}	// 타일맵 정보 저장

	// Sprite 객체 생성
	// 현재 : 1개 스프라이트 생성
	LPCWSTR fileName = L"../Resources/Images/PathAndObjects.png";
	LPCWSTR scriptName;	// 스크립트 생성

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			_tileSprite[y][x] = new Sprite();

			int index = tileMapIndex[y][x];	// 타일맵! 이게 중요함!! -> 이거로 뭐가 찍히는지가 변함
			switch (index)
			{
				case 0:
				scriptName = L"playerLeft.json";
				break;
				case 1:
				scriptName = L"playerRight.json";
				break;
				case 2:
				scriptName = L"playerFront.json";
				break;
				case 3:
				scriptName = L"playerBack.json";
				break;
				default:
				break;
			}

			_tileSprite[y][x]->Init(fileName, scriptName, dxDevice, spriteDX);
		}
	}
}

void Map::Update(float deltaTime)
{
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
			_tileSprite[y][x]->Update(deltaTime);
	}
}

void Map::Render()
{
	// 타일 적용
	float startX = 0.0f;
	float startY = 0.0f;
	float posX = startX;
	float posY = startY;
	int tileSize = 32;

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			_tileSprite[y][x]->SetPosition(posX, posY);	// 위치 설정
			_tileSprite[y][x]->Render();	// 출력*

			posX += tileSize;	// 위치 이동
		}
		posX = startX;
		posY += tileSize;
	}
}

void Map::Realease()
{
	// 기존에 만들어져 있던 것 reset
	// 다른것은 이미 망가졌으므로 이거만 release, 다해도 상관없음
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
			_tileSprite[y][x] ->Release();
	}
}

void Map::Reset()
{
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
			_tileSprite[y][x]->Reset();
	}
}

void Map::Deinit()
{
	// 객체 파괴
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (NULL != _tileSprite[y][x])
				delete _tileSprite[y][x];
		}
	}
}