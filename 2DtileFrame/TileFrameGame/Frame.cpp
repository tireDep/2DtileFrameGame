#include "Frame.h"

Frame::Frame()
{
	_frameInterval = 0.2f;
}

Frame::~Frame()
{
}

void Frame::Init(ID3DXSprite* spriteDX, IDirect3DTexture9* textureDX, 
	int left, int top, int width, int height, float frameInterval, 
	D3DCOLOR textureColor)
{
	_spriteDX = spriteDX;
	_textureDX = textureDX;
	_frameInterval = frameInterval;

	_textureRect.left = left;
	_textureRect.top = top;
	_textureRect.right = _textureRect.left + width;
	_textureRect.bottom = _textureRect.top + height;
	// ũ�� : 52, 72

	_textureColor = textureColor;
	_width = width;
	_height = height;
}

void Frame::Render()
{
	D3DXVECTOR2 scale = D3DXVECTOR2(5.0f, 5.0f);	// ũ�� ���(Ȯ��/���),(�⺻ũ�� 1.0f, 1.0f)
	D3DXVECTOR2 rotCenter = D3DXVECTOR2(_width*0.7f, _height*0.5f);	// ȸ���߽�(0.5x0.5�� ���! -> 1.0maxsize)
	// 2d���ӿ����� �̷����� �� Ȱ�뵵 ���� -> ȸ���߽��� �Ȱ��� ���
	
	float rot = 0.0f;	// ȸ�� ����(ȸ�� ����=0.0f)
	
	D3DXVECTOR2 translate = D3DXVECTOR2(500,50);
	// D3DXVECTOR2 translate = D3DXVECTOR2(_width*0.5f, _height*0.5f);	// �̵�(�ػ� �˸� �����ϱ�)


	D3DXMATRIX matrix;	// ��ĺ��� - ũ��, ȸ��, ȸ�� �߽�, ȸ�� ����, �̵� ���� ������ ������ ����
	D3DXMatrixTransformation2D
	(
		&matrix,	// ���� ��� ����
		NULL,	// �������� �߽���
		0.0f,	// Ȯ�� ȸ�� ��ų ��, x,y ���� �ٸ� ��� ���� ��
		&scale,	// Ȯ��/��� ��
		&rotCenter,	//ȸ�� �߽���	
		rot,	//ȸ�� ����
		&translate	// �̵�
	);
	// ���� ���� �ʿ��� �� -> scale, rotCenter, rot, translate �� => �� �����Ӹ��� �ٸ��� ���� ������

		
	_spriteDX->SetTransform(&matrix); 
	_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);

}

void Frame::Reset()
{}