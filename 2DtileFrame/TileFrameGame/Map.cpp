#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include "Sprite.h"
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
	// 맵용 스프라이트 리스트 작업
	// 타일 개수, 개별 타일 크기는 합의가 끝난 상태(팀 작업일 경우) => 32x32, 256개

	// 맵 생성
	int tileMapIndex[16][16];	// 타일맵 변수
	/*int idx = 0;	// 순서 체크 변수

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			// tileMapIndex[y][x] = rand() % 4;	// 랜덤으로 0 ~ 3
			tileMapIndex[y][x] = idx++; //0 ~ 255
		}
	}	// 타일맵 정보 저장
	*/
	// 스크립트 대체
	{
		std::string fileName = "map.csv";	// 파일 이름
		std::vector<std::string>recordList;	// 토큰 집합
		
		// file open -> file read(record) -> parshing(token) -> save
		std::ifstream infile(fileName);	// 파일 읽어옴!!
		if (true == infile.is_open())	// 파일이 정상적으로 열릴 경우
		{
			char recordString[1000];
			while (false == infile.eof())	// 파일 마지막까지 루프
			{
				infile.getline(recordString, 1000);	// 한 줄씩 읽어옴
				recordList.push_back(recordString);	// 레코드에 저장
			}
		}

		// 레코드 -> 토큰, 게임에 적용
		char record[1000];
		for(int y = 0; y < 16; y++)	// 16개 레코드가 16줄 존재
		{
			strcpy(record, recordList[y].c_str());	// 통째로 한 줄 복사
			char *token = strtok(record, ",");	// 분리가 된 첫 번째 토큰이 record에 저장됨(문자로)

			for (int x = 0; x < 16; x++)
			{
				int tileIndex = atoi(token);	// 문자 -> 숫자 저장
				tileMapIndex[y][x] = tileIndex;
				token = strtok(NULL, ",");	// 다음거 저장 후 parse
			}
		}

	}
	// LPCWSTR scriptName;	// 스크립트 생성

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			int spriteIndex = tileMapIndex[y][x];
			_tileSprite[y][x] = _spriteList[spriteIndex]; // 어딘가에 미리 다 생성해 둔 후 불러옴
			/*_tileSprite[y][x] = new Sprite();

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
			_tileSprite[y][x]->SetPosition(posX+30, posY+30);	// 위치 설정
			_tileSprite[y][x]->Render();	// 출력*

			posX += tileSize;	// +2 : 오프셋 확인용 => 실제 사이즈 확인
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