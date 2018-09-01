// Setting
// fullscreen 모드일경우 윈도우 관련x이므로 창모드로 진행함
#include <Windows.h>
#include <d3dx9.h>
#include "GameTimer.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello World", L"Hello", MB_OK);	
		return 0;
	case WM_KEYDOWN:	
		if (VK_ESCAPE == wParam)	
			DestroyWindow(hWnd);	
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	int width = 1024;
	int height = 768;
	bool isWindow = true;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;	
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"2DTileFrameWnd";

	if (RegisterClass(&wc) == FALSE)
		return 0;

	DWORD style;
	if (isWindow)	// 윈도우 모드일 경우 처음 창 요소 그대로 사용
		style = WS_OVERLAPPEDWINDOW;	// 겹치는 창을 만듦(기본설정인듯)
	else // 풀스크린 모드인 경우, 윈도우에 부가적인 요소를 제거함
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;	// 윈도우 구상 가장 기본적인 요소
	// 이 창을 제외한 나머지 창을 nontopmost windows로 배치 | 처음 표시되는 창 생성 | 팝업창 생성(ws_chlid랑 사용 가능)
	// 윈도우화면 성능과 관련됨, 다이렉트엑스랑 상관x
	
	HWND hWnd = CreateWindow(L"2DTileFrameWnd", L"2D Tile Frame", style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);

	if (NULL == hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow);	
	UpdateWindow(hWnd);

	if (isWindow == true)
	{
		RECT clientRect;

		GetClientRect(hWnd, &clientRect);
		MoveWindow(hWnd, 0, 0, width + (width - clientRect.right), height + (height - clientRect.bottom), TRUE);
	}

	LPDIRECT3D9 direct3D;
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	if (direct3D == NULL)
		return 0;

	D3DFORMAT format;
	if (isWindow)	// 윈도우 설정에 따름
		format = D3DFMT_UNKNOWN;
	else // 풀스크린일때 전용 색상 포멧을 사용함
		format = D3DFMT_X8R8G8B8;
	// 다이렉트x랑 직접적인 연관 있음

	D3DPRESENT_PARAMETERS d3dpp;	
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = isWindow;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	LPDIRECT3DDEVICE9 dxDevice;
	HRESULT hr = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
	// directX 중 lost device(디바이스 유실) 관련 설정 해줘야함 -> alt + tab 이후 게임으로 돌아가지 않음, 전체화면일 경우에만 에러남!



	if (FAILED(hr))
		return 0;

	// sprite com 인터페이스 생성
	// 설정 -> 링커 -> 입력 -> d3dx9d.lib(개발용) or d3dx9.lib(출시용) 추가, 
	// d3d9 : 다이렉트 엑스 필수, d3dx9d : 기존 기술 확장개념
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(dxDevice, &spriteDX);
	if (FAILED(hr))
		return 0;

	// 이미지 로드
	IDirect3DTexture9* textureDX;
	RECT textureRect;
	D3DCOLOR textureColor;	// 색상
	{
		LPCWSTR fileName = L"../Resources/Images/bonus1_full.png";	// 로드할 파일명, L붙여야 함(유니코드 사용)
		// 속성 -> 문자집합 -> 유니코드 => ""앞에 L붙여줘야 함!
		// 경로 쓸 경우 슬래시 주의..
		
		// 파일로부터 이미지의 너비와 높이를 얻음
		D3DXIMAGE_INFO texInfo;
		hr = D3DXGetImageInfoFromFile(fileName, &texInfo);
		if (FAILED(hr))
			return 0;

		// 이미지 데이터 로드
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName, texInfo.Width, texInfo.Height, 
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255), 
			&texInfo, NULL, &textureDX);
		/*
		pDevice - [in] IDirect3DDevice9 인터페이스의 포인터. 텍스처에 관련지을 수 있는 장치를 나타낸다.
		pSrcFile - [in] 파일명을 지정하는 캐릭터 라인의 포인터. 컴파일러의 설정이 Unicode 를 요구하고 있는 경우, 데이터 타입 LPCTSTR 는 LPCWSTR 가 된다. 그 이외의 경우는, 이 캐릭터 라인의 데이터 타입은 LPCSTR 가 된다. 「주의」를 참조할것.
		Width - [in] 폭 (픽셀 단위). 이 값이 0 또는 D3DX_DEFAULT 의 경우, 넓이는 파일로부터 취득된다.
		Height - [in] 높이 (픽셀 단위). 이 값이 0 또는 D3DX_DEFAULT 의 경우, 넓이는 파일로부터 취득된다.
		MipLevels - [in] 요구되는 밉레벨의 수. 이 값이 0 또는 D3DX_DEFAULT 의 경우는, 완전한 밉맵 체인이 생성 된다.
		Usage - [in] 0, D3DUSAGE_RENDERTARGET, 또는 D3DUSAGE_DYNAMIC. 이 플래그를 D3DUSAGE_RENDERTARGET 로 설정 하면, 그 표면은 렌더링 타겟으로서 사용되는 것을 나타낸다. 리소스는,IDirect3DDevice9::SetRenderTarget 메서드의 pNewRenderTarget 파라미터에 건네줄 수가 있다. D3DUSAGE_RENDERTARGET 또는 D3DUSAGE_DYNAMIC 를 지정하는 경우,Pool 를 D3DPOOL_DEFAULT 로 설정해, 애플리케이션은 IDirect3D9::CheckDeviceFormat 를 호출해, 장치가 이 처리를 지원 하고 있는 것을 확인할 필요가 있다. D3DUSAGE_DYNAMIC 는, 표면을 동적으로 처리할 필요가 있는 것을 나타낸다. 동적 텍스처의 사용법의 더 자세한 정보는, 「동적 텍스처의 사용법」을 참조할것.
		Format - D3DFORMAT 열거형의 멤버. 텍스처에 대해서 요구된 픽셀 포맷을 기술한다. 돌려받는 텍스처의 포맷은,Format 로 지정한 포맷과 다른 경우가 있다. 애플리케이션은, 돌려주어진 텍스처의 포맷을 확인할 필요가 있다. Format 의 값이 D3DFMT_UNKNOWN 의 경우, 포맷은 파일로부터 취득된다.
		Pool - [in] D3DPOOL 열거형의 멤버. 텍스처의 배치처가 되는 메모리 클래스를 기술한다.
		Filter - [in] 이미지를 필터링 하는 방법을 제어하는 1 개 혹은 복수의 D3DX_FILTER 의 편성. 이 파라미터에 D3DX_DEFAULT 를 지정하는 것은, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER 를 지정하는 것으로 동일하다.
		MipFilter - [in] 이미지를 필터링 하는 방법을 제어하는 1 개 혹은 복수의 D3DX_FILTER 의 편성. 이 파라미터에 D3DX_DEFAULT 를 지정하는 것은, D3DX_FILTER_BOX 를 지정하는 것으로 동일하다.
		ColorKey - [in] 투명이 되는 D3DCOLOR 의 값. 컬러 키를 무효로 하는 경우는 0 을 지정한다. 소스 이미지의 포맷과는 관계없이, 이것은 항상 32 비트의 ARGB 컬러이다. 알파가 의미가 있고, 보통은 컬러 키를 불투명하게 하는 경우는 FF 를 지정한다. 따라서, 불투명한 흑의 경우, 값은 0xFF000000 가 된다.
		pSrcInfo - [in, out] 소스 이미지 파일내의 데이터의 기술을 저장 하는 D3DXIMAGE_INFO 구조체의 포인터, 또는 NULL.
		pPalette - [out] 저장 하는 256 색팔레트를 나타내는 PALETTEENTRY 구조체의 포인터, 또는 NULL.
		ppTexture - [out] 생성 된 큐브 텍스처 개체를 나타내는,IDirect3DTexture9 인터페이스의 포인터 주소.
		*/

		if (FAILED(hr))
			return 0;

		textureRect.left = 0;
		textureRect.right = textureRect.left + texInfo.Width;
		textureRect.top = 0;
		textureRect.bottom = textureRect.top + texInfo.Height;
		// 출력 영역 지정 -> 원본 이미지 전체 출력

		textureColor = D3DCOLOR_ARGB(255, 255/2, 255/2, 255/2);	// 알파채널 작동, 원본 그대로 출력(흰색)
	}

	float fps = 60.0f;
	float frameInterval = 1.0f / fps;
	float frameTime = 0.0f;

	GameTimer gameTimer;
	gameTimer.Init();
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTimer();
			frameTime += deltaTime;

			if (frameInterval <= frameTime)
			{
				frameTime = 0.0f;
				dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 100, 100), 0.0f, 0);
				// 가독성
				{	
					// 특정 영역(scene)에 이미지 출력
					dxDevice->BeginScene();	// 시작 / 필수!
					{
						// scene 작업 : 게임화면과 관련된 모든 작업 공간
						// 반드시 beginscene ~ endscene 사이에 이루어져야함
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);	// 필수는 x
						{
							// 알파채널 사용함!
							// 2D이미지 출력 공간 -> Texture 1장 출력
							spriteDX->Draw(textureDX, &textureRect, NULL, NULL, textureColor);
							// 그릴 텍스처 정보가 들어있는 인터페이스, 원본 이미지에서 그릴 부분, NULL(일단은) ,NULL(일단은) , 스프라이트의 색상&알파채널
							// 알파채널 : 특정색상(투명값) 제외 하는 것 ->  한 색상 사용x ==> 알파채널이라는 설정으로 배경제외(투명값이외의 다른 값 사용함 의미함)
							/*	ex)
							D3DFMT_R8G8B8	20	24-bit RGB pixel format with 8 bits per channel.
							D3DFMT_A8R8G8B8	21	32-bit ARGB pixel format with alpha, using 8 bits per channel.
							D3DFMT_X8R8G8B8	22	32-bit RGB pixel format, where 8 bits are reserved for each color.
							*/
						}
						spriteDX->End();
					}
					dxDevice->EndScene();	// 끝 / 필수!
				}

				dxDevice->Present(NULL, NULL, NULL, NULL);
			}
		}
	}

	// 컴 인터페이스들 생성되있을 경우 삭제 작업 필요함
	if (dxDevice)	
	{
		dxDevice->Release();
		dxDevice = NULL;
	}
	if (direct3D)
	{
		direct3D->Release();
		direct3D = NULL;
	}
	return 0;
}