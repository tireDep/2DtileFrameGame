// Setting
#include <Windows.h>
#include <d3d9.h>
#include "GameTimer.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(0, "Hello World", "Hello", MB_OK);	
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
	bool isWindow = false;

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
	wc.lpszClassName = "2DTileFrameWnd";

	if (RegisterClass(&wc) == FALSE)
		return 0;

	DWORD style;
	if (isWindow)	// 윈도우 모드일 경우 처음 창 요소 그대로 사용
		style = WS_OVERLAPPEDWINDOW;
	else // 풀스크린 모드인 경우, 윈도우에 부가적인 요소를 제거함
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;	// 윈도우 구상 가장 기본적인 요소
	// 윈도우화면 성능과 관련됨, 다이렉트엑스랑 상관x
	
	HWND hWnd = CreateWindow("2DTileFrameWnd", "2D Tile Frame", style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);

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
	// lost device 관련 설정 해줘야함

	if (FAILED(hr))
		return 0;

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