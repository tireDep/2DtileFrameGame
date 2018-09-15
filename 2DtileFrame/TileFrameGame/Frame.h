#pragma once
#include <d3dx9.h>

class Frame
{
private:
	RECT _textureRect;
	D3DCOLOR _textureColor;	// 프레임별로 색상 지정 가능

	ID3DXSprite * _spriteDX;
	IDirect3DTexture9* _textureDX;
	// 이미 만들어진것을 받아오기 위해서

	float _frameInterval;

	int _width;
	int _height;

	float _scale;

public:
	Frame();
	~Frame();

	void Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX,
		int left, int top, int width, int height, float frameInterval,
		D3DCOLOR textureColor, float scale);
	void Render();
	void Reset();
	float GetFrameInterval()
	{
		return _frameInterval;	// 임시
	}
};