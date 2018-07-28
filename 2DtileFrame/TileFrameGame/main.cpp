#include <Windows.h>
// ������ ���α׷����� main�� ���� ����ϸ� �� ��!
// ���������� ���������� �Ǻ��� ��, window�� WinMain() ������
// ������ : main�� �������� �����, Winmain�� �����찡 ���� �� �����ָ� ���� �ȵ�

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	// ������ ���ν��� �Լ� : ������κ��� ���� �̺�Ʈ�� ó���ϴ� �Լ�(���� ó����)
{	// CALLBACK ������ ��½�ϰ� �����(�̰Ŵ� ã�ƺ���) �ٵ� ������ ������ �ȵǳ�..?
	// ���� �̺�Ʈ�� �߻��ϰ�, �� �Ͼ������ �� -> �ٸ� ������ �߻��� �̺�Ʈ�� �Ͼ ���� ����(�־�)
	// �̺�Ʈ�� ���� ���� �ϳ��� ������(���������� �����찡 �� ó������)
	switch (msg)	// msg�� �� �̺�Ʈ
	{
	case WM_LBUTTONDOWN:	// ���콺 ���� ��ư�� ����
		MessageBox(0, "Hello World", "Hello", MB_OK);
		return 0;
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
// ���� ���� ���α׷��� ���� �ڵ�, �Ⱦ���, �����ϴµ� ���̴� ����� ���� �μ�, �������α׷� ǥ�� ���(�ּ�/�ִ�ȭ �ؼ� �����ٰų� ��)
{
	// 3.
	// ������ ��Ÿ���� ����� �����
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;	// â�� ��Ÿ�� ����(ȭ�� ũ�⸦ �ٲܶ����� �۾��� ���̰� �Ⱥ��̰� �����ϴ� ��)
	wc.lpfnWndProc = WndProc;	// ������ ���ν��� �Լ� ���(ó�� ����) 
	// �ٵ� �̰� �Լ��ƴѰ� �� ��ȣ����? -> �Լ�������!(�Լ��� ��� �ִ����� �˷���)
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

	// ������� �߱��� �����ָ� ��� �� ����!

	// 1.
	ShowWindow(hWnd, nCmdShow);	// hWnd : �ϳ��� �����츦 ����Ű�� �ڵ�(ID)
	UpdateWindow(hWnd);
	// hWnd�� �̸��� ���� �����츦 �����ְ� ������Ʈ ����

	// �����찡 ������ �ʰ� �����ǵ��� ó��
	MSG msg;
	while (-1 != GetMessage(&msg, 0, 0, 0))	// �߻��� �޽����� ������
	{
		TranslateMessage(&msg);	// Ű���� ��ȯ ����
		DispatchMessage(&msg);	// �޽��� ����� ��û��
	}
	return 0;

	/* ����
	1. �����츦 ������ �� -> ���̵� ����
    2. ���̵� ���� -> ����� �Ǿ� ���� ����
	3. ������ ��Ÿ�� ����� ���
	*/
}