#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}


void Sprite::Init(std::wstring fileName, LPDIRECT3DDEVICE9 dxDevice, ID3DXSprite* spriteDX)
{
	D3DXIMAGE_INFO texInfo;	// 파일로부터 이미지의 너비와 높이를 얻음
	HRESULT hr;
	_dxDevice = dxDevice;
	_spriteDX = spriteDX;
	{
		hr = D3DXGetImageInfoFromFile(fileName.c_str(), &texInfo);
		if (FAILED(hr))
			return;

		// 이미지 데이터 로드
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName.c_str(), texInfo.Width, texInfo.Height,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
		&texInfo, NULL, &_textureDX);

		if (FAILED(hr))
			return;

		_textureRect.left = 0;
		_textureRect.right = _textureRect.left + texInfo.Width;
		_textureRect.top = 0;
		_textureRect.bottom = _textureRect.top + texInfo.Height;
		// 출력 영역 지정 -> 원본 이미지 전체 출력

		_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);	// 알파채널 작동, 원본 그대로 출력(흰색)
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