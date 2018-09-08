#include "Frame.h"

Frame::Frame()
{
	_frameInterval = 0.2f;
}

Frame::~Frame()
{
}

void Frame::Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX, int left, int top, int width, int height, float frameInterval)
{
	_spriteDX = spriteDX;
	_textureDX = textureDX;
	_frameInterval = frameInterval;

	_textureRect.left = left;
	_textureRect.top = top;
	_textureRect.right = _textureRect.left + width;
	_textureRect.bottom = _textureRect.top + height;
	// 크기 : 52, 72

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);	// 알파채널 작동, 원본 그대로 출력(흰색)
}

void Frame::Render()
{
	_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);
}

void Frame::Reset()
{}