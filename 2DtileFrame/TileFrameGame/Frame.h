#pragma once
#include <d3dx9.h>

class Frame
{
public:
	Frame();
	~Frame();

	void Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX, int left, int top, int width, int height, float frameInterval);
	void Render();
	void Reset();
	float GetFrameInterval()
	{
		return _frameInterval;	// �ӽ�
	}

private:
	RECT _textureRect;
	D3DCOLOR _textureColor;	// �����Ӻ��� ���� ���� ����

	ID3DXSprite * _spriteDX;
	IDirect3DTexture9* _textureDX;
	// �̹� ����������� �޾ƿ��� ���ؼ�

	float _frameInterval;
};