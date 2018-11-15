#include <Windows.h>
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
				OutputDebugString("Update\n");
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