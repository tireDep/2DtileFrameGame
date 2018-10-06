#include "Sprite.h"
#include "Frame.h"
#include <d3dx9.h>

#include <string.h>
#include <fstream>	// 파일
#include <reader.h>	// json 헤더

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	/*if (NULL != _frame)
		delete _frame;
	*/

	for (int i = 0; i < _frameList.size(); i++)
	{
		delete _frameList[i];
	}
	_frameList.clear();

	if (_textureDX)
	{
		_textureDX->Release();	// this->_textureDX->Release();
		_textureDX = NULL;		
	}
}

void Sprite::Init(std::wstring fileName, LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	HRESULT hr;
	_dxDevice = dxDevice;
	_spriteDX = spriteDX;
	_fileName = fileName;

	{
		hr = D3DXGetImageInfoFromFile(fileName.c_str(), &_texInfo);
		if (FAILED(hr))
			return;

		// 이미지 데이터 로드
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName.c_str(), _texInfo.Width, _texInfo.Height,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo, NULL, &_textureDX);

		if (FAILED(hr))
			return;

		/*
		_textureRect.left = 104;
		_textureRect.right = _textureRect.left + 52;
		_textureRect.top = 0;
		_textureRect.bottom = _textureRect.top + 72;

		_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);	// 알파채널 작동, 원본 그대로 출력(흰색)
		*/
		//Frame* frame = new Frame();
		//frame->Init(_spriteDX, _textureDX, 0, 0, 52, 72);
		// 크기 : 52, 72
		// left, top, width, height
		
		/*{
			Frame* frame = new Frame();
			frame->Init(_spriteDX, _textureDX, 0, 0, 52, 72, 0.5f, D3DCOLOR_ARGB(100, 255, 255, 255),0.5f);
			// 왼, 위, 오, 아, 흐르는 시간!(스피드 조절), 색상설정(투명도,rgb), 이미지 크기
			_frameList.push_back(frame);
		}
		{
			Frame* frame = new Frame();
			frame->Init(_spriteDX, _textureDX, 52, 0, 52, 72, 0.5f, D3DCOLOR_ARGB(155, 255, 255, 255),1.0f);
			_frameList.push_back(frame);
		}
		{
			Frame* frame = new Frame();
			frame->Init(_spriteDX, _textureDX, 104, 0, 52, 72, 0.5f, D3DCOLOR_ARGB(255, 250, 255, 255),3.0f);
			_frameList.push_back(frame);
		}*/

		// 스크립트로 프레임 불러옴
		std::string fileName = "Test.json";
		std::ifstream infile(fileName);	// 파일 로딩

		if (infile.is_open())	// 파일이 잘 열릴 경우
		{
			char recordString[1000];
			while (false == infile.eof())	// 파일이 끝나지 않으면
			{
				infile.getline(recordString, 1000);	// maximum 1000까지 1줄을 읽음

				// Parsing, Record(Token : 게임 최소 단위)
				Json::Value root;
				Json::Reader reader;
				bool isSuccess = reader.parse(recordString, root);	// 파싱 끝!
				// 보통 이런 식으로 파싱작업함!! -> 토큰별로 분리되서 root에 들어가 있음
				
				if (true == isSuccess)
				{
					// 파싱된 값을 적용함
					// 과제!
					// Init 나머지 부분 스크립트화, 다른 이미지 출력관련해서 해보기!
					int x = root["x"].asInt();
					int y = root["y"].asInt();
					int width = root["width"].asInt();
					int height = root["height"].asInt();
					float frameInterval = root["frameInterval"].asDouble();

					
					Frame* frame = new Frame();
					frame->Init(_spriteDX, _textureDX, x, y, width, height, frameInterval, D3DCOLOR_ARGB(255, 255, 255, 255), 0.5f);
					// ID3DXSprite* 변수, IDirect3DTexture9* 변수, 왼, 위, 오, 아, 흐르는 시간!(스피드 조절), 색상설정(투명도,rgb), 이미지 크기
					_frameList.push_back(frame);					
				}

			}
		}

		
	else
		{
			// msg
			exit(0);
		}
	

		_frameIndex = 0;
		_frameDuration = 0.0f;
	}
}

void Sprite::Update(float deltaTime)
{
	_frameDuration += deltaTime;	// 현재 프레임이 출력된 후 흐른 시간
	
	if (_frameList[_frameIndex]->GetFrameInterval() <= _frameDuration)	// 정해진 시간을 넘으면 초기화
	{
		_frameDuration = 0.0f;
		_frameIndex++;
		if (_frameList.size() <= _frameIndex)
			_frameIndex = 0;
	}
}

void Sprite::Render()
{
	// _spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
	// _frame->Render();
	if(_frameIndex<_frameList.size())	// 범위내에 있을 때만 렌더링
		_frameList[_frameIndex]->Render();
}

void Sprite::Release()
{
	if (_textureDX)
	{
		_textureDX->Release();
		_textureDX = NULL;
	}
}

void Sprite::Reset()
{
	HRESULT hr = D3DXCreateTextureFromFileEx(_dxDevice, _fileName.c_str(), _texInfo.Width, _texInfo.Height,
	1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
	&_texInfo, NULL, &_textureDX);
	// 디바이스&스프라이트&텍스처 인터페이스 재 생성
	
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset();
	}
}