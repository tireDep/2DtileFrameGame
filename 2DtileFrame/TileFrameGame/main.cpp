// Setting
// fullscreen ����ϰ�� ������ ����x�̹Ƿ� â���� ������
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
	IDirect3DTexture9* textureDX;
	RECT textureRect;
	D3DCOLOR textureColor;	// ����
	{
		LPCWSTR fileName = L"../Resources/Images/bonus1_full.png";	// �ε��� ���ϸ�, L�ٿ��� ��(�����ڵ� ���)
		// �Ӽ� -> �������� -> �����ڵ� => ""�տ� L�ٿ���� ��!
		// ��� �� ��� ������ ����..
		
		// ���Ϸκ��� �̹����� �ʺ�� ���̸� ����
		D3DXIMAGE_INFO texInfo;
		hr = D3DXGetImageInfoFromFile(fileName, &texInfo);
		if (FAILED(hr))
			return 0;

		// �̹��� ������ �ε�
		hr = D3DXCreateTextureFromFileEx(dxDevice, fileName, texInfo.Width, texInfo.Height, 
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255), 
			&texInfo, NULL, &textureDX);
		/*
		pDevice - [in] IDirect3DDevice9 �������̽��� ������. �ؽ�ó�� �������� �� �ִ� ��ġ�� ��Ÿ����.
		pSrcFile - [in] ���ϸ��� �����ϴ� ĳ���� ������ ������. �����Ϸ��� ������ Unicode �� �䱸�ϰ� �ִ� ���, ������ Ÿ�� LPCTSTR �� LPCWSTR �� �ȴ�. �� �̿��� ����, �� ĳ���� ������ ������ Ÿ���� LPCSTR �� �ȴ�. �����ǡ��� �����Ұ�.
		Width - [in] �� (�ȼ� ����). �� ���� 0 �Ǵ� D3DX_DEFAULT �� ���, ���̴� ���Ϸκ��� ���ȴ�.
		Height - [in] ���� (�ȼ� ����). �� ���� 0 �Ǵ� D3DX_DEFAULT �� ���, ���̴� ���Ϸκ��� ���ȴ�.
		MipLevels - [in] �䱸�Ǵ� �ӷ����� ��. �� ���� 0 �Ǵ� D3DX_DEFAULT �� ����, ������ �Ӹ� ü���� ���� �ȴ�.
		Usage - [in] 0, D3DUSAGE_RENDERTARGET, �Ǵ� D3DUSAGE_DYNAMIC. �� �÷��׸� D3DUSAGE_RENDERTARGET �� ���� �ϸ�, �� ǥ���� ������ Ÿ�����μ� ���Ǵ� ���� ��Ÿ����. ���ҽ���,IDirect3DDevice9::SetRenderTarget �޼����� pNewRenderTarget �Ķ���Ϳ� �ǳ��� ���� �ִ�. D3DUSAGE_RENDERTARGET �Ǵ� D3DUSAGE_DYNAMIC �� �����ϴ� ���,Pool �� D3DPOOL_DEFAULT �� ������, ���ø����̼��� IDirect3D9::CheckDeviceFormat �� ȣ����, ��ġ�� �� ó���� ���� �ϰ� �ִ� ���� Ȯ���� �ʿ䰡 �ִ�. D3DUSAGE_DYNAMIC ��, ǥ���� �������� ó���� �ʿ䰡 �ִ� ���� ��Ÿ����. ���� �ؽ�ó�� ������ �� �ڼ��� ������, ������ �ؽ�ó�� �������� �����Ұ�.
		Format - D3DFORMAT �������� ���. �ؽ�ó�� ���ؼ� �䱸�� �ȼ� ������ ����Ѵ�. �����޴� �ؽ�ó�� ������,Format �� ������ ���˰� �ٸ� ��찡 �ִ�. ���ø����̼���, �����־��� �ؽ�ó�� ������ Ȯ���� �ʿ䰡 �ִ�. Format �� ���� D3DFMT_UNKNOWN �� ���, ������ ���Ϸκ��� ���ȴ�.
		Pool - [in] D3DPOOL �������� ���. �ؽ�ó�� ��ġó�� �Ǵ� �޸� Ŭ������ ����Ѵ�.
		Filter - [in] �̹����� ���͸� �ϴ� ����� �����ϴ� 1 �� Ȥ�� ������ D3DX_FILTER �� ��. �� �Ķ���Ϳ� D3DX_DEFAULT �� �����ϴ� ����, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER �� �����ϴ� ������ �����ϴ�.
		MipFilter - [in] �̹����� ���͸� �ϴ� ����� �����ϴ� 1 �� Ȥ�� ������ D3DX_FILTER �� ��. �� �Ķ���Ϳ� D3DX_DEFAULT �� �����ϴ� ����, D3DX_FILTER_BOX �� �����ϴ� ������ �����ϴ�.
		ColorKey - [in] ������ �Ǵ� D3DCOLOR �� ��. �÷� Ű�� ��ȿ�� �ϴ� ���� 0 �� �����Ѵ�. �ҽ� �̹����� ���˰��� �������, �̰��� �׻� 32 ��Ʈ�� ARGB �÷��̴�. ���İ� �ǹ̰� �ְ�, ������ �÷� Ű�� �������ϰ� �ϴ� ���� FF �� �����Ѵ�. ����, �������� ���� ���, ���� 0xFF000000 �� �ȴ�.
		pSrcInfo - [in, out] �ҽ� �̹��� ���ϳ��� �������� ����� ���� �ϴ� D3DXIMAGE_INFO ����ü�� ������, �Ǵ� NULL.
		pPalette - [out] ���� �ϴ� 256 ���ȷ�Ʈ�� ��Ÿ���� PALETTEENTRY ����ü�� ������, �Ǵ� NULL.
		ppTexture - [out] ���� �� ť�� �ؽ�ó ��ü�� ��Ÿ����,IDirect3DTexture9 �������̽��� ������ �ּ�.
		*/

		if (FAILED(hr))
			return 0;

		textureRect.left = 0;
		textureRect.right = textureRect.left + texInfo.Width;
		textureRect.top = 0;
		textureRect.bottom = textureRect.top + texInfo.Height;
		// ��� ���� ���� -> ���� �̹��� ��ü ���

		textureColor = D3DCOLOR_ARGB(255, 255/2, 255/2, 255/2);	// ����ä�� �۵�, ���� �״�� ���(���)
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
				// ������
				{	
					// Ư�� ����(scene)�� �̹��� ���
					dxDevice->BeginScene();	// ���� / �ʼ�!
					{
						// scene �۾� : ����ȭ��� ���õ� ��� �۾� ����
						// �ݵ�� beginscene ~ endscene ���̿� �̷��������
						spriteDX->Begin(D3DXSPRITE_ALPHABLEND);	// �ʼ��� x
						{
							// ����ä�� �����!
							// 2D�̹��� ��� ���� -> Texture 1�� ���
							spriteDX->Draw(textureDX, &textureRect, NULL, NULL, textureColor);
							// �׸� �ؽ�ó ������ ����ִ� �������̽�, ���� �̹������� �׸� �κ�, NULL(�ϴ���) ,NULL(�ϴ���) , ��������Ʈ�� ����&����ä��
							// ����ä�� : Ư������(����) ���� �ϴ� �� ->  �� ���� ���x ==> ����ä���̶�� �������� �������(�����̿��� �ٸ� �� ����� �ǹ���)
							/*	ex)
							D3DFMT_R8G8B8	20	24-bit RGB pixel format with 8 bits per channel.
							D3DFMT_A8R8G8B8	21	32-bit ARGB pixel format with alpha, using 8 bits per channel.
							D3DFMT_X8R8G8B8	22	32-bit RGB pixel format, where 8 bits are reserved for each color.
							*/
						}
						spriteDX->End();
					}
					dxDevice->EndScene();	// �� / �ʼ�!
				}

				dxDevice->Present(NULL, NULL, NULL, NULL);
			}
		}
	}

	// �� �������̽��� ���������� ��� ���� �۾� �ʿ���
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