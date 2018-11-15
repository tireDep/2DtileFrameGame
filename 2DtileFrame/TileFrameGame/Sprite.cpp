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

		// �̹��� ������ �ε�
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

		_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);	// ����ä�� �۵�, ���� �״�� ���(���)
		*/
		//Frame* frame = new Frame();
		//frame->Init(_spriteDX, _textureDX, 0, 0, 52, 72);
		// ũ�� : 52, 72
		// left, top, width, height
		
		/*{
			Frame* frame = new Frame();
			frame->Init(_spriteDX, _textureDX, 0, 0, 52, 72, 0.5f, D3DCOLOR_ARGB(100, 255, 255, 255),0.5f);
			// ��, ��, ��, ��, �帣�� �ð�!(���ǵ� ����), ������(����,rgb), �̹��� ũ��
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

		// ��ũ��Ʈ�� ������ �ҷ���
		std::string fileName = "Test.json";
		std::ifstream infile(fileName);	// ���� �ε�

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
					// ����!
					// Init ������ �κ� ��ũ��Ʈȭ, �ٸ� �̹��� ��°����ؼ� �غ���!
					int x = root["x"].asInt();
					int y = root["y"].asInt();
					int width = root["width"].asInt();
					int height = root["height"].asInt();
					float frameInterval = root["frameInterval"].asDouble();

					
					Frame* frame = new Frame();
					frame->Init(_spriteDX, _textureDX, x, y, width, height, frameInterval, D3DCOLOR_ARGB(255, 255, 255, 255), 0.5f);
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
	if(_frameIndex<_frameList.size())	// �������� ���� ���� ������
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
	// ����̽�&��������Ʈ&�ؽ�ó �������̽� �� ����
	
	for (int i = 0; i < _frameList.size(); i++)
	{
		_frameList[i]->Reset();
	}
}