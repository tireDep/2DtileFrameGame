// Sprite class
#pragma once
#include <string>	// wstring �̿�
#include <d3dx9.h>	// DirectX���� �ذ�

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init(std::wstring fileName, LPDIRECT3DDEVICE9 dxDevice,ID3DXSprite* spriteDX);
	void Render();
	void Release(); 
	void Reset();

private:
	IDirect3DTexture9* _textureDX;
	RECT _textureRect;
	D3DCOLOR _textureColor;
	LPDIRECT3DDEVICE9 _dxDevice;
	ID3DXSprite* _spriteDX;
	// Ŭ������ �ʿ��� �Ӽ�
};