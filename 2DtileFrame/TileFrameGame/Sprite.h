// Sprite class
#pragma once
#include <string>	// wstring 이용
#include <d3dx9.h>	// DirectX에러 해결

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
	// 클래스가 필요한 속성
};