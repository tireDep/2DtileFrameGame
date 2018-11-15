#pragma once
#include <d3dx9.h>

class Frame
{
private:
	RECT _textureRect;
	D3DCOLOR _textureColor;	// �����Ӻ��� ���� ���� ����

	ID3DXSprite * _spriteDX;
	IDirect3DTexture9* _textureDX;
	// �̹� ����������� �޾ƿ��� ���ؼ�

	float _frameInterval;

	int _width;
	int _height;

	float _scale;

	float _x;
	float _y;	// ��ġ


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
		return _frameInterval;	// �ӽ�
	}

	void SetPosition(float x, float y);	// Ÿ�ϸ� �Լ�, ��ġ
};