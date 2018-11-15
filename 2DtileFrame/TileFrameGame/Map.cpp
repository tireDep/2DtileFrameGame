#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include "Sprite.h"
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
	LPCWSTR fileName = L"../Resources/Images/PathAndObjects.png";
	int srcX = 0;
	int srcY = 0;
	int tileSize = 32;

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Sprite *sprite = new Sprite();
			sprite->Init(fileName, srcX, srcY, tileSize, tileSize, 1.0, dxDevice, spriteDX);
			_spriteList.push_back(sprite);
			srcX += tileSize;
		}
		srcX = 0;
		srcY += tileSize;
	}
	// �ʿ� ��������Ʈ ����Ʈ �۾�
	// Ÿ�� ����, ���� Ÿ�� ũ��� ���ǰ� ���� ����(�� �۾��� ���) => 32x32, 256��

	// �� ����
	int tileMapIndex[16][16];	// Ÿ�ϸ� ����
	/*int idx = 0;	// ���� üũ ����

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			// tileMapIndex[y][x] = rand() % 4;	// �������� 0 ~ 3
			tileMapIndex[y][x] = idx++; //0 ~ 255
		}
	}	// Ÿ�ϸ� ���� ����
	*/
	// ��ũ��Ʈ ��ü
	{
		std::string fileName = "map.csv";	// ���� �̸�
		std::vector<std::string>recordList;	// ��ū ����
		
		// file open -> file read(record) -> parshing(token) -> save
		std::ifstream infile(fileName);	// ���� �о��!!
		if (true == infile.is_open())	// ������ ���������� ���� ���
		{
			char recordString[1000];
			while (false == infile.eof())	// ���� ���������� ����
			{
				infile.getline(recordString, 1000);	// �� �پ� �о��
				recordList.push_back(recordString);	// ���ڵ忡 ����
			}
		}

		// ���ڵ� -> ��ū, ���ӿ� ����
		char record[1000];
		for(int y = 0; y < 16; y++)	// 16�� ���ڵ尡 16�� ����
		{
			strcpy(record, recordList[y].c_str());	// ��°�� �� �� ����
			char *token = strtok(record, ",");	// �и��� �� ù ��° ��ū�� record�� �����(���ڷ�)

			for (int x = 0; x < 16; x++)
			{
				int tileIndex = atoi(token);	// ���� -> ���� ����
				tileMapIndex[y][x] = tileIndex;
				token = strtok(NULL, ",");	// ������ ���� �� parse
			}
		}

	}
	// LPCWSTR scriptName;	// ��ũ��Ʈ ����

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			int spriteIndex = tileMapIndex[y][x];
			_tileSprite[y][x] = _spriteList[spriteIndex]; // ��򰡿� �̸� �� ������ �� �� �ҷ���
			/*_tileSprite[y][x] = new Sprite();

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
			*/
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
			_tileSprite[y][x]->SetPosition(posX+30, posY+30);	// ��ġ ����
			_tileSprite[y][x]->Render();	// ���*

			posX += tileSize;	// +2 : ������ Ȯ�ο� => ���� ������ Ȯ��
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