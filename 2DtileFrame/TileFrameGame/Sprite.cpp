#include "Sprite.h"
#include "Frame.h"
#include <d3dx9.h>

#include <string.h>
#include <fstream>	// ����
#include <reader.h>	// json ���

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
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

void Sprite::Init(std::wstring fileName, std::wstring scriptName, LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	HRESULT hr;
	_dxDevice = dxDevice;
	_spriteDX = spriteDX;
	_fileName = fileName;

	{
		hr = D3DXGetImageInfoFromFile(fileName.c_str(), &_texInfo);
		if (FAILED(hr))
			return;

		// �̹��� ������ �ε�
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName.c_str(), _texInfo.Width, _texInfo.Height,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo, NULL, &_textureDX);

		if (FAILED(hr))
			return;

		// ��ũ��Ʈ�� ������ �ҷ���
		// std::string fileName = "Test.json";
		std::ifstream infile(scriptName);	// ���� �ε�

		if (infile.is_open())	// ������ �� ���� ���
		{
			char recordString[1000];
			while (false == infile.eof())	// ������ ������ ������
			{
				infile.getline(recordString, 1000);	// maximum 1000���� 1���� ����

				// Parsing, Record(Token : ���� �ּ� ����)
				Json::Value root;
				Json::Reader reader;
				bool isSuccess = reader.parse(recordString, root);	// �Ľ� ��!
				// ���� �̷� ������ �Ľ��۾���!! -> ��ū���� �и��Ǽ� root�� �� ����
				
				if (true == isSuccess)
				{
					// �Ľ̵� ���� ������
					int x = root["x"].asInt();
					int y = root["y"].asInt();
					int width = root["width"].asInt();
					int height = root["height"].asInt();
					float frameInterval = root["frameInterval"].asDouble();

					int alpha = root["alpha"].asInt();
					int red = root["red"].asInt();
					int green = root["green"].asInt();
					int blue = root["blue"].asInt();
					float scale = root["scale"].asDouble();

					
					Frame* frame = new Frame();
					frame->Init(_spriteDX, _textureDX, x, y, width, height, frameInterval, D3DCOLOR_ARGB(alpha, red, green, blue), scale);
					// ID3DXSprite* ����, IDirect3DTexture9* ����, ��, ��, ��, ��, �帣�� �ð�!(���ǵ� ����), ������(����,rgb), �̹��� ũ��
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

void Sprite::Init(std::wstring fileName,int x, int y, int width, int height, float frameInterval,LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	HRESULT hr;
	_dxDevice = dxDevice;
	_spriteDX = spriteDX;
	_fileName = fileName;

	{
		hr = D3DXGetImageInfoFromFile(fileName.c_str(), &_texInfo);
		if (FAILED(hr))
			return;

		// �̹��� ������ �ε�
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName.c_str(), _texInfo.Width, _texInfo.Height,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
			&_texInfo, NULL, &_textureDX);

		if (FAILED(hr))
			return;


		Frame* frame = new Frame();
		frame->Init(_spriteDX, _textureDX, x, y, width, height, frameInterval, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0);
		// ID3DXSprite* ����, IDirect3DTexture9* ����, ��, ��, ��, ��, �帣�� �ð�!(���ǵ� ����), ������(����,rgb), �̹��� ũ��
		// �ܺο��� �Ѱ���
		
		_frameList.push_back(frame);
		_frameIndex = 0;
		_frameDuration = 0.0f;
	}
}

void Sprite::Update(float deltaTime)
{
	_frameDuration += deltaTime;	// ���� �������� ��µ� �� �帥 �ð�
	
	if (_frameList[_frameIndex]->GetFrameInterval() <= _frameDuration)	// ������ �ð��� ������ �ʱ�ȭ
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
	if (_frameIndex < _frameList.size())	// �������� ���� ���� ������
	{
		_frameList[_frameIndex]->SetPosition(_x, _y);
		_frameList[_frameIndex]->Render();
	}
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
	// ����̽�&��������Ʈ&�ؽ�ó �������̽� �� ����
	
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset();
	}
}

void Sprite::SetPosition(float x, float y)	// Ÿ�ϸ� �Լ�, ��ġ
{
	_x = x;
	_y = y;
}