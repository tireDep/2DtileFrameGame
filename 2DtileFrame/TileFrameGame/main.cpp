#include <Windows.h>
#include <d3d9.h>
#include "GameTimer.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	// ������ ���ν��� �Լ� : ������κ��� ���� �̺�Ʈ�� ó���ϴ� �Լ�(���� ó����)
{
	// �̺�Ʈ�� ���� ���� �ϳ��� ������(���������� �����찡 �� ó������)
	switch (msg)	// msg�� �� �̺�Ʈ
	{
	case WM_LBUTTONDOWN:	// ���콺 ���� ��ư�� ����
		MessageBox(0, "Hello World", "Hello", MB_OK);	// 0 : �Ϲ����� â�� ������ ���� ���� => WindowOS�� ���� ������ ����! / â�� ���� ��쿡�� â�� ���� �ٲ�
		return 0;
	case WM_KEYDOWN:	// Ű �Է�
		if (VK_ESCAPE == wParam)	// �Է�Ű�� esc
		{
			DestroyWindow(hWnd);	// â �ı� msg
			// !����! - PostQuitMessage(0); �ۼ� �� �ı� msg�� �߱� x�̹Ƿ� ������� ����
		}
		break;
	case WM_DESTROY:	// �ı� msg
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
// ���� ���� ���α׷��� ���� �ڵ�, �Ⱦ���, �����ϴµ� ���̴� ����� ���� �μ�, �������α׷� ǥ�� ���(�ּ�/�ִ�ȭ �ؼ� �����ٰų� ��)
{
	// 3. ������ ��Ÿ���� ����� �����
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;	// â�� ��Ÿ�� ����(ȭ�� ũ�⸦ �ٲܶ����� �۾��� ���̰� �Ⱥ��̰� �����ϴ� ��)
	wc.lpfnWndProc = WndProc;	// ������ ���ν��� �Լ� ���(ó�� ����) 
	// �Լ�������!(�Լ��� ��� �ִ����� �˷���)
	// ���°� ��ӵǾ� �ֱ� ������ �۵��� ������! �ٸ� ������ ��� �۵����� ����!!(��ŵ� ã�ƺ��ô�)
	wc.cbClsExtra = 0;	// ���x(�����) / �����൵ �Ǳ� �ϴµ� ��������δ� �ۼ�����
	wc.cbWndExtra = 0;	// ���x(�����) / �����൵ �Ǳ� �ϴµ� ��������δ� �ۼ�����
	wc.hInstance = hInstance;	// ������� OS ����
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);	// ������ ������ �� � ������ �������(���� �⺻���� ������)
	wc.hCursor = LoadCursor(0, IDC_ARROW);// ������ ������ �� � ȭ��ǥ �������(���� �⺻���� ȭ��ǥ)
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ���ȭ��(����� ������� ���)
	wc.lpszMenuName = 0;	// �޴� ����(���x �̹Ƿ� 0)
	wc.lpszClassName = "2DTileFrameWnd";	// �� ������ ��Ÿ���� �̸�
	// �� ������ ���ؼ� ����� ������ ��Ÿ�� �ϳ� ����

	// ���� ������ ��Ÿ�� ���
	if (RegisterClass(&wc) == FALSE)// ��ϵ��� ���� ��� // (!RegisterClass(&wc))
		return 0;

	// 2.
	HWND hWnd = CreateWindow("2DTileFrameWnd", "2D Tile Frame", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, 0, 0, hInstance, 0);	// â �ڵ�(ID) �߱�
	// ����� ������ ��Ÿ�� �̸�, OS�� ��ϵǾ� ����, �ֻ�� ����, ������ ��Ÿ��(overlappedwindow �̰Ű� ���� ǥ��), �����찡 ���� x, y ��ǥ(������ġ -> �˾Ƽ� ��������),
	// �ػ�, �ʺ�/����, [�θ� â�� �ڵ�, �޴� �ڵ�] => �� �ΰ��� ������(0���� ǥ��), OS�� ������ ���� -> OS���� �����츦 ������ �� ����(�����쿡�� �߱޹��� ID�� �����) ���� �߿�!!, ������� ����

	if (NULL == hWnd)	// ���������� ������ �ȵ� ���
		return 0;

	// 1.
	ShowWindow(hWnd, nCmdShow);	// hWnd : �ϳ��� �����츦 ����Ű�� �ڵ�(ID)
	UpdateWindow(hWnd);

	// DirectX - ������(DirectX)���� �ϵ��� ���� ���� �� �� �ִ� device��(dxDevice) �����ؼ� �޶�� ��û
	LPDIRECT3D9 direct3D;	// �׷��� ��� DirectX
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);	// �����쿡 �޶�� ��û�ϴ� ��
	
	if (direct3D == NULL)	// ���� x�� ����
		return 0;

	// device�� �����ϱ� ��, device�� ���ؼ� ȭ�鿡 ��� ����� �� ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));	// ������ 0���� �ʱ�ȭ

	d3dpp.BackBufferWidth = 1280;	// ȭ���ػ�
	d3dpp.BackBufferHeight = 768;	// ȭ���ػ�
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		
	// ȭ�鿡 � ����bit�� ǥ���� ��(Unknown : ������ ������ ���� => ������ ����ϰ�� �̰� ���� / ���ϴ� ����������� ��üȭ�� �ؾ���) 
	d3dpp.BackBufferCount = 1;	// ���� ���۸� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;	// ������ ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	// device ���� �غ� �Ϸ�

	// device ����
	LPDIRECT3DDEVICE9 dxDevice;	// LP : ������
	HRESULT hr = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &dxDevice);
	// D3DADAPTER_DEFAULT, D3DCREATE_HARDWARE_VERTEXPROCESSING => GPU�� ������ �ްڴٴ� ��!


	float fps = 60.0f;
	float frameInterval = 1.0f / fps;	// frameInterval�� frame ����
	float frameTime = 0.0f;

	GameTimer gameTimer;	// class
	gameTimer.Init();
	// hWnd�� �̸��� ���� �����츦 �����ְ� ������Ʈ ����
	// ������ ��� - msg�� ������ �ִ´��, ������ ���� ��� ó���Ǿ�� ��
	// ���ӿ� �°� ������ �κ�
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)	// app ���� != destroy -> �����켺�� �� �ִ�� ����
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) == TRUE)	// window�� window msg ����x -> �ֿ켱 ����!
		{
			/* 
			! ������ !
			GetMessage -> msg�� ���� ��쿡�� ���� / ���� ������ �� => �̺�Ʈ�� ���� ��쿡�� �����
			PeekMessage -> msg ������ ��� ���� ���� / ���ϳ��� ó���� => �̺�Ʈ ��� x�� �����
			*/
			TranslateMessage(&msg);	// Ű���� ��ȯ ����
			DispatchMessage(&msg);	// �޽��� ����� ��û��
		}
		else
		{
			// �̷л����δ� �� �� ���µ� �װ� 1�ʿ� ���� ��Ÿ
			// ������ �� ���� ���� -> ƽ ���
			
			gameTimer.Update();
			float deltaTime = gameTimer.GetDeltaTimer();	// deltaTime : ���� �����ӿ��� ���ݱ��� �帥 �ð�, ���� �����쿡������ ���ݱ��� �帥 �ð�
			frameTime += deltaTime;

			if (frameInterval <= frameTime)	// ������ �ð� <= �帥 �ð� �� ��� Update
			{
				/*
				! todo !
				- gameUpdate .60fps
				- ��ü �� 60fps�� �����ϴ� �� => ���� while���� ��� x
				*/
				frameTime = 0.0f;
				
				// OutputDebugString("Update\n");
				
				// �� �����Ӹ��� ȭ�鿡 ���� ä�� 
				// dxDevice : �� ��ü
				dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(254, 100, 100), 0.0f, 0);

				// ä�� ���� ����ͷ� �����
				dxDevice->Present(NULL, NULL, NULL, NULL);
			}
		}
	}

	return 0;

	/* ����
	1. �����츦 ������ �� -> ���̵� ����
    2. ���̵� ���� -> ����� �Ǿ� ���� ����
	3. ������ ��Ÿ�� ����� ���
	*/
}