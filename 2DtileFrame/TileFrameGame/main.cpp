#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)	// 윈도우 프로시저 함수 : 윈도우로부터 받은 이벤트를 처리하는 함수(내가 처리함)
{
	// 이벤트를 받을 공간 하나만 생성함(아직까지는 윈도우가 다 처리해줌)
	switch (msg)	// msg가 곧 이벤트
	{
	case WM_LBUTTONDOWN:	// 마우스 왼쪽 버튼이 눌림
		MessageBox(0, "Hello World", "Hello", MB_OK);	// 0 : 일반적인 창에 영향을 받지 않음 => WindowOS에 직접 관리를 받음! / 창을 넣을 경우에는 창에 따라 바뀜
		return 0;
	case WM_KEYDOWN:	// 키 입력
		if (VK_ESCAPE == wParam)	// 입력키가 esc
		{
			DestroyWindow(hWnd);	// 창 파괴 msg
			// !주의! - PostQuitMessage(0); 작성 시 파괴 msg가 발급 x이므로 실행되지 않음
		}
		break;
	case WM_DESTROY:	// 파괴 msg
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
// 현재 응용 프로그램의 메인 핸들, 안쓰임, 실행하는데 쓰이는 명령줄 도는 인수, 응용프로그램 표시 방식(최소/최대화 해서 보여줄거냐 등)
{
	// 3. 윈도우 스타일을 만들고 등록함
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;	// 창의 스타일 지정(화면 크기를 바꿀때마다 글씨가 보이고 안보이고를 갱신하는 것)
	wc.lpfnWndProc = WndProc;	// 윈도우 프로시저 함수 등록(처리 관련) 
	// 함수포인터!(함수가 어디에 있는지만 알려줌)
	// 형태가 약속되어 있기 때문에 작동이 가능함! 다른 형태일 경우 작동되지 않음!!(요거도 찾아봅시다)
	wc.cbClsExtra = 0;	// 사용x(사장됨) / 안해줘도 되긴 하는데 명시적으로는 작성해줌
	wc.cbWndExtra = 0;	// 사용x(사장됨) / 안해줘도 되긴 하는데 명시적으로는 작성해줌
	wc.hInstance = hInstance;	// 윈도우와 OS 연결
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);	// 윈도우 실행할 때 어떤 아이콘 출력인지(가장 기본적인 아이콘)
	wc.hCursor = LoadCursor(0, IDC_ARROW);// 윈도우 실행할 때 어떤 화살표 출력인지(가장 기본적인 화살표)
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 배경화면(배경을 흰색으로 출력)
	wc.lpszMenuName = 0;	// 메뉴 형식(사용x 이므로 0)
	wc.lpszClassName = "2DTileFrameWnd";	// 이 윈도우 스타일의 이름
	// 이 과정을 통해서 사용할 윈도우 스타일 하나 생성

	// 만든 윈도우 스타일 등록
	if (RegisterClass(&wc) == FALSE)// 등록되지 않을 경우 // (!RegisterClass(&wc))
		return 0;

	// 2.
	HWND hWnd = CreateWindow("2DTileFrameWnd", "2D Tile Frame", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, 0, 0, hInstance, 0);	// 창 핸들(ID) 발급
	// 사용할 윈도우 스타일 이름, OS에 등록되어 있음, 최상단 제목, 윈도우 스타일(overlappedwindow 이거가 가장 표준), 윈도우가 찍힐 x, y 좌표(시작위치 -> 알아서 지정해줌),
	// 해상도, 너비/높이, [부모 창의 핸들, 메뉴 핸들] => 이 두개는 사용안함(0으로 표시), OS와 윈도우 연결 -> OS에서 윈도우를 관리할 수 있음(윈도우에서 발급받은 ID를 사용함) 가장 중요!!, 사용하지 않음

	if (NULL == hWnd)	// 정상적으로 실행이 안될 경우
		return 0;

	// 1.
	ShowWindow(hWnd, nCmdShow);	// hWnd : 하나의 윈도우를 가리키는 핸들(ID)
	UpdateWindow(hWnd);
	// hWnd란 이름을 가진 윈도우를 보여주고 업데이트 해줌

	// 윈도우가 꺼지지 않고 유지되도록 처리
	/*MSG msg;
	while (-1 != GetMessage(&msg, 0, 0, 0))	// 발생된 메시지가 있으면
	{
		TranslateMessage(&msg);	// 키보드 변환 수행
		DispatchMessage(&msg);	// 메시지 배분을 요청함
	}
	*/

	// 게임의 경우 - msg가 있으면 있는대로, 없으면 없는 대로 처리되어야 함
	// 게임에 맞게 개조된 부분
	MSG msg = { 0 };
	// 윈도우성능 중 최대로 실행
	while (WM_QUIT != msg.message)	// app 종료 != destroy
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) == TRUE)	// window는 window msg 무시x -> 최우선 순위!
		{
			/* 
			! 차이점 !
			GetMessage -> msg가 있을 경우에만 나옴 / 언제 나올지 모름 => 이벤트가 있을 경우에만 사용함
			PeekMessage -> msg 유무에 상관 없이 나옴 / 한턴내에 처리됨 => 이벤트 상관 x로 사용함
			*/
			TranslateMessage(&msg);	// 키보드 변환 수행
			DispatchMessage(&msg);	// 메시지 배분을 요청함
		}
		else
		{
			// 이론상으로는 들어갈 수 없는데 그게 1초에 만번 연타
			// 정수로 할 수도 있음 -> 틱 사용
			float frameTime = 0;
			float frameInterval = 1.0f / 60.0f;	// frameInterval로 frame 조정
			if (frameInterval <= frameTime)	// 프레임 시간 <= 흐른 시간 일 경우 Update
			{
				/*
				! todo !
				- gameUpdate .60fps
				- 전체 중 60fps로 제한하는 것 => 게임 while문과 상관 x
				*/
				frameTime = 0.0f;
			}
		}
	}

	return 0;

	/* 순서
	1. 윈도우를 띄우려고 함 -> 아이디가 없음
    2. 아이디를 생성 -> 등록이 되어 있지 않음
	3. 윈도우 스타일 만들고 등록
	*/
}