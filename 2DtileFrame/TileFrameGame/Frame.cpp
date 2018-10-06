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
	D3DCOLOR textureColor, float scale)
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
	_scale = scale;
	_width = width * _scale;
	_height = height * _scale;
}

void Frame::Render()
{
	D3DXVECTOR2 scale = D3DXVECTOR2(_scale,_scale);	// 크기 출력(확대/축소),(기본크기 1.0f, 1.0f)
	D3DXVECTOR2 rotCenter = D3DXVECTOR2(_width*0.5f, _height*0.5f);	// 회전중심(0.5x0.5가 가운데! -> 1.0maxsize)
	// 2d게임에서는 이런식이 더 활용도 높음 -> 회전중심은 똑같이 가운데
	
	float rot = 0.0f;	// 회전 각도(회전 없음=0.0f)
	
	// D3DXVECTOR2 translate = D3DXVECTOR2(0.0f,0.0f);
	D3DXVECTOR2 translate = D3DXVECTOR2(_x-_width * 0.5f, _y -_height * 0.5f);	// 이동
	// 피벗 가운데 -> -_width*0.5f, -_height*0.5f
	// default는 좌측상단
	// 피벗을 가운데 => 좌표위치랑 피벗위치랑 같게 해주는 것 ==> +/-로 설정 가능
	/*					|------|
	피벗--|				| 피벗--|------|
	|	  |				|---|---이미지 |
	|-----|	// 좌측상단,		|----------|	// 피벗 이동시
	=> 찍힌 좌표는 고정, 피벗위치로 이동시키는 것
		// 출력 위치가 바뀌는 것이라고 생각하면 될 듯(이미지가 피벗 위치에 출력됨)
	*/

	/* ! 회전, 이동 등 모든 중심(피벗)이 0,0인 경우 -> 왼쪽정렬(왼쪽 최상단) !
	- 기본은 가운데, but 상황에 따라서 위치 바뀔 수 있음
	- ex) 나무 키를 키울때 -> 피벗 위치 아래에 두는 것
	*/

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

void Frame::SetPosition(float x, float y)	// 타일맵 함수, 위치
{
	_x = x;
	_y = y;
}