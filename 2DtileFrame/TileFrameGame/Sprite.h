// Sprite class
#pragma once
#include <string>	// wstring �̿�
#include <d3dx9.h>	// DirectX���� �ذ�
#include <vector>	

class Frame;	// ���� ����
// Ŭ���� ���� �ٸ� Ŭ���� ������ ������� �߰� -> �⺻������ ����Ȱ��� �ƴϹǷ� �����Ϸ� ������
/*
< �ذ�å >
1) ������� ���� - ���� �˷��ִ� ���(������), but include ����
2) ���漱�� - ������ ������, �ϴ� �Ѿ + ��򰡿� �����ϹǷ� ����� �� �˻��غ�

+) �̹� ����Ǿ��ִ� ���, ��Ӱ��õ� ������ include ���
*/

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init(std::wstring fileName, LPDIRECT3DDEVICE9 dxDevice,ID3DXSprite* spriteDX);
	void Update(float deltaTime);	// �Ϲ������� Render ���� Update �Լ� ������
	void Render();
	void Release(); 
	void Reset();

private:
	IDirect3DTexture9* _textureDX;
	LPDIRECT3DDEVICE9 _dxDevice;
	ID3DXSprite* _spriteDX;

	std::wstring _fileName;
	D3DXIMAGE_INFO _texInfo;	// ���Ϸκ��� �̹����� �ʺ�� ���̸� ����
	// �������
	// Ŭ������ �ʿ��� �Ӽ�

	std::vector<Frame*>_frameList;
	int _frameIndex;
	float _frameDuration;
	// Frame* _frame;	// ������ ����
};