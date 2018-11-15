// Setting
// fullscreen ����ϰ�� ������ ����x�̹Ƿ� â���� ������
#include <Windows.h>
#include <d3dx9.h>
#include "GameTimer.h"
#include "Sprite.h"	

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
	if (isWindow)	// ������ ����� ��� ó�� â ��� �״�� ���
		style = WS_OVERLAPPEDWINDOW;	// ��ġ�� â�� ����(�⺻�����ε�)
	else // Ǯ��ũ�� ����� ���, �����쿡 �ΰ����� ��Ҹ� ������
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;	// ������ ���� ���� �⺻���� ���
	// �� â�� ������ ������ â�� nontopmost windows�� ��ġ | ó�� ǥ�õǴ� â ���� | �˾�â ����(ws_chlid�� ��� ����)
	// ������ȭ�� ���ɰ� ���õ�, ���̷�Ʈ������ ���x
	
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
	if (isWindow)	// ������ ������ ����
		format = D3DFMT_UNKNOWN;
	else // Ǯ��ũ���϶� ���� ���� ������ �����
		format = D3DFMT_X8R8G8B8;
	// ���̷�Ʈx�� �������� ���� ����

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
	// directX �� lost device(����̽� ����) ���� ���� ������� -> alt + tab ���� �������� ���ư��� ����, ��üȭ���� ��쿡�� ������!

	if (FAILED(hr))
		return 0;

	// sprite com �������̽� ����
	// ���� -> ��Ŀ -> �Է� -> d3dx9d.lib(���߿�) or d3dx9.lib(��ÿ�) �߰�, 
	// d3d9 : ���̷�Ʈ ���� �ʼ�, d3dx9d : ���� ��� Ȯ�尳��
	ID3DXSprite* spriteDX;
	hr = D3DXCreateSprite(dxDevice, &spriteDX);
	if (FAILED(hr))
		return 0;

	// �̹��� �ε�
	LPCWSTR fileName = L"../Resources/Images/bonus1_full.png";

	// Sprite ��ü ����
	Sprite* testSprite = new Sprite();
	//Sprite* testSprite2 = new Sprite();
	testSprite->Init(fileName,dxDevice,spriteDX);
	//testSprite2->Init(fileName, dxDevice, spriteDX);
	
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

			testSprite->Update(deltaTime);
			//testSprite2->Update(deltaTime);

			frameTime += deltaTime;

			if (frameInterval <= frameTime)
			{
				frameTime = 0.0f;
				dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(100, 100, 100), 0.0f, 0);
				{	
					// Ư�� ����(scene)�� �̹��� ���
					dxDevice->BeginScene();	// ���� / �ʼ�!
					{
						// scene �۾� : ����ȭ��� ���õ� ��� �۾� ����
						// �ݵ�� beginscene ~ endscene ���̿� �̷��������
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);	
						{
							// Sprite Render
							// testSprite->Render();

							// Ÿ�� ����

							float startX = 0.0f;
							float startY = 0.0f;
							float posX = startX;
							float posY = startY;
							int tileSize = 32;

							for (int y = 0; y < 16; y++)
							{
								for (int x = 0; x < 16; x++)
								{
									testSprite->SetPosition(posX, posY);	// ��ġ ����
									testSprite->Render();	// ���
									
									//testSprite2->SetPosition(posX, posY);	// ��ġ ����
									//testSprite2->Render();	// ���

									posX += tileSize;	// ��ġ �̵�

								}
								posX = startX;
								posY += tileSize;
							}

						}
						spriteDX->End();
					}
					/*
					Device Lost ó�� -> ���� ���ӳ������� ���� ������ ��(Update �ƶ��ε�)
					Device ���� Ȯ��(Update����) -> Device ���¿� ���� ������ ���
					Device�� �� �� Device�� ���� ������ ��� ���ҽ��� ������
					=> ������ �� �˾Ƶξ�� ��!
					*/
					hr = dxDevice->TestCooperativeLevel();	// ����̽� ������ ���� Ȯ��(���̷�Ʈ���� �⺻���� �Լ�)
					if (FAILED(hr))
					{
						// ���¿� ���� ���� ��, ����̽� + ����̽��� ���� ������ ��� ���ҽ� ����
						// ���� ���� : 3���� -> ���� ������ �ٸ� ó���� �ʿ���
						if (D3DERR_DEVICELOST == hr)	// ���� �ƹ��͵� �� �� x(�ϵ����� ���� ������ ��)
							Sleep(100);	// ���ð�
						else if (D3DERR_DEVICENOTRESET == hr)	// ������ ���� but ���� ������ ���� => ���� ������
						{
							// ������ ������� �ִ� �� reset
							// �ٸ����� �̹� ���������Ƿ� �̰Ÿ� release, ���ص� �������
							testSprite->Release();

							/*
							if (textureDX)
							{
								textureDX->Release();
								textureDX = NULL;
							}
							*/

							// ���� ����(����)
							direct3D = Direct3DCreate9(D3D_SDK_VERSION);
							if (direct3D != NULL)	// ���������� ���� ��
							{
								HRESULT hr = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
								if(SUCCEEDED(hr))
									hr = D3DXCreateSprite(dxDevice, &spriteDX);
								if (SUCCEEDED(hr))
									testSprite->Reset();
							}
						}
					}
					dxDevice->EndScene();	// �� / �ʼ�!
				}
				dxDevice->Present(NULL, NULL, NULL, NULL);
			} // if��
		}	// else ��
	}

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
	if (NULL != testSprite)
		delete testSprite;

	return 0;
}