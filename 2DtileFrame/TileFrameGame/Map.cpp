#include"Sprite.h"
#include "Map.h"	
// ���� ��Ŭ���� �� �Ʒ��� �δ� ���� ��Ŭ��� ������ ����

Map::Map()
{
}

Map::~Map()
{
}

void Map::Init(LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	int tileMapIndex[16][16];	// Ÿ�ϸ� ����
	int idx = 0;	// ���� üũ ����

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			// tileMapIndex[y][x] = rand() % 4;	// �������� 0 ~ 3
			tileMapIndex[y][x] = (idx++) % 4;	// ������� 0 ~ 3 ����
		}
	}	// Ÿ�ϸ� ���� ����

	// Sprite ��ü ����
	// ���� : 1�� ��������Ʈ ����
	LPCWSTR fileName = L"../Resources/Images/PathAndObjects.png";
	LPCWSTR scriptName;	// ��ũ��Ʈ ����

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			_tileSprite[y][x] = new Sprite();

			int index = tileMapIndex[y][x];	// Ÿ�ϸ�! �̰� �߿���!! -> �̰ŷ� ���� ���������� ����
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
	// Ÿ�� ����
	float startX = 0.0f;
	float startY = 0.0f;
	float posX = startX;
	float posY = startY;
	int tileSize = 32;

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			_tileSprite[y][x]->SetPosition(posX, posY);	// ��ġ ����
			_tileSprite[y][x]->Render();	// ���*

			posX += tileSize;	// ��ġ �̵�
		}
		posX = startX;
		posY += tileSize;
	}
}

void Map::Realease()
{
	// ������ ������� �ִ� �� reset
	// �ٸ����� �̹� ���������Ƿ� �̰Ÿ� release, ���ص� �������
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
	// ��ü �ı�
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (NULL != _tileSprite[y][x])
				delete _tileSprite[y][x];
		}
	}
}