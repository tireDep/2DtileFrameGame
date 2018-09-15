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
	// 크기 : 52, 72

	_textureColor = textureColor;
	_width = width;
	_height = height;
}

void Frame::Render()
{
	D3DXVECTOR2 scale = D3DXVECTOR2(5.0f, 5.0f);	// 크기 출력(확대/축소),(기본크기 1.0f, 1.0f)
	D3DXVECTOR2 rotCenter = D3DXVECTOR2(_width*0.7f, _height*0.5f);	// 회전중심(0.5x0.5가 가운데! -> 1.0maxsize)
	// 2d게임에서는 이런식이 더 활용도 높음 -> 회전중심은 똑같이 가운데
	
	float rot = 0.0f;	// 회전 각도(회전 없음=0.0f)
	
	D3DXVECTOR2 translate = D3DXVECTOR2(500,50);
	// D3DXVECTOR2 translate = D3DXVECTOR2(_width*0.5f, _height*0.5f);	// 이동(해상도 알면 수정하기)


	D3DXMATRIX matrix;	// 행렬변수 - 크기, 회전, 회전 중심, 회전 각도, 이동 등의 정보를 가지고 있음
	D3DXMatrixTransformation2D
	(
		&matrix,	// 연산 결과 저장
		NULL,	// 스케일의 중심점
		0.0f,	// 확대 회전 시킬 때, x,y 비율 다를 경우 조정 값
		&scale,	// 확대/축소 값
		&rotCenter,	//회전 중심점	
		rot,	//회전 각도
		&translate	// 이동
	);
	// 실제 조정 필요한 값 -> scale, rotCenter, rot, translate 들 => 매 프레임마다 다르게 조정 가능함

		
	_spriteDX->SetTransform(&matrix); 
	_spriteDX->Draw(_textureDX, &_textureRect, NULL, NULL, _textureColor);

}

void Frame::Reset()
{}