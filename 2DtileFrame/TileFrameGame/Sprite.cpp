#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}


void Sprite::Init(std::wstring fileName, LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	D3DXIMAGE_INFO texInfo;	// ���Ϸκ��� �̹����� �ʺ�� ���̸� ����
	HRESULT hr;
	_dxDevice = dxDevice;
	_spriteDX = spriteDX;
	{
		hr = D3DXGetImageInfoFromFile(fileName.c_str(), &texInfo);
		if (FAILED(hr))
			return;

		// �̹��� ������ �ε�
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName.c_str(), texInfo.Width, texInfo.Height,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
		&texInfo, NULL, &_textureDX);

		if (FAILED(hr))
			return;

		_textureRect.left = 0;
		_textureRect.right = _textureRect.left + texInfo.Width;
		_textureRect.top = 0;
		_textureRect.bottom = _textureRect.top + texInfo.Height;
		// ��� ���� ���� -> ���� �̹��� ��ü ���

		_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);	// ����ä�� �۵�, ���� �״�� ���(���)
	}
}

void Sprite::Render()
{
	_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
}

void Sprite::Release()
{

}

void Sprite::Reset()
{

}