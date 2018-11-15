#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
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

		_textureRect.left = 104;
		_textureRect.right = _textureRect.left + 52;
		_textureRect.top = 0;
		_textureRect.bottom = _textureRect.top + 72;
		// ũ�� : 52, 72

		_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);	// ����ä�� �۵�, ���� �״�� ���(���)
	}
}

void Sprite::Render()
{
	_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
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
}