#include "Sound.h"

LRESULT CALLBACK    HdnWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;// LoadIcon(hInstance, MAKEINTRESOURCE(109));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCE(109);//IDC_WIN32NOBUTTON=109
	wcex.lpszClassName = "EmreCpp";
	wcex.hIconSm = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));//IDI_SMALL = 108

	return RegisterClassEx(&wcex);
}

ATOM MyRegisterHidden(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = HdnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)19;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "EmreCpp2";
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	hInstance = hThisInstance;
	MyRegisterClass(hThisInstance);
	MyRegisterHidden(hThisInstance);

	CreateWindow("EmreCpp2", "", WS_OVERLAPPEDWINDOW | WS_EX_TOPMOST | WS_EX_COMPOSITED, 0, 0, 0, 0, 0, nullptr, hInstance, nullptr);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(109)); //IDC_WIN32NOBUTTON =109	
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Refresh, 0, 0, 0);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdcStatic = NULL;
	switch (message)
	{
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 1;
		lpMMI->ptMinTrackSize.y = 50;
		break;
	}
	case WM_CLOSE://Right click to label for exit
	{
		DestroyWindow(hwnd);
		CoUninitialize();
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rc;
		
		HDC hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);


		SetBkColor(hdc, 0x0); // black
		ExtTextOut(hdc, 0, 0, BLACKNESS, &rc, 0, 0, 0);//ETO_OPAQUE

		

		EndPaint(hwnd, &ps);	
		break;
	}

	case WM_CTLCOLORSTATIC:
	{
		hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 0));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LONG)GetStockObject(BLACK_BRUSH);//COLOR_BACKGROUND
	}
	case WM_MOUSEWHEEL:
	{
		if (!*vaf->formgozukuyor) break;
		if (!mouse(MOUSEEVENTS::ON_PROGRESSBAR))break;

		if ((short)HIWORD(wParam) > 0)		
			vaf->SetVolume(vaf->GetVolume() + 2);
		else
			vaf->SetVolume(vaf->GetVolume() - 2);
		
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK HdnWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)//Taskbarda gizli pencere icin
{
	HDC hdcStatic = NULL;
	switch (message)
	{
	case WM_CREATE:
	{
		vaf->hwndpencere = CreateWindow("EmreCpp", "", WS_POPUP | WS_EX_TOPMOST | WS_VISIBLE | WS_EX_COMPOSITED, sizes->Window.right, sizes->Window.bottom, sizes->Window.left, sizes->Window.top, hwnd, NULL, hInstance, NULL);//Right = loc X, Bottom = Loc Y, Left = Size X, Top = Size Y
		SetWindowPos(vaf->hwndpencere, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CTLCOLORSTATIC:
	{
		hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 0));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LONG)GetStockObject(BLACK_BRUSH);//COLOR_BACKGROUND
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}