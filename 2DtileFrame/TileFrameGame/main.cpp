#include <Windows.h>

// 윈도우 프로그래밍은 main을 직접 사용하면 안 됨!
// 내부적으로 정상적인지 판별한 후, window가 WinMain() 실행함
// 차이점 : main은 에러나도 실행됨, Winmain은 윈도우가 실행 안 시켜주면 실행 안됨

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
// 현재 응용 프로그램의 메인 핸들, 안쓰임, 실행하는데 쓰이는 명령줄 도는 인수, 응용프로그램 표시 방식(최소/최대화 해서 보여줄거냐 등)
{
	return 0;
}
