// galsPanic.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "galsPanic.h"
#include "movingPoint.h"
//#include <commdlg.h>

#pragma comment(lib, "msimg32.lib");

#define MAX_LOADSTRING 100

using namespace std;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


HBITMAP hBackImage, hBackImage2;
BITMAP bitBack, bitBack2;

HBITMAP hTransparentImage;
BITMAP bitTransparent;

HBITMAP hAniImage;
BITMAP bitAni;
//const int Sprite_Size_X = 57;
//const int Sprite_Size_Y = 52;
//int Run_Frame_Max = 0;
//int Run_Frame_Min = 0;
//int cur_Frame = Run_Frame_Min;
//int keyDown = 0;
//int distance = 0;
//int yStart = 0;
//int framePlus = Run_Frame_Min;

bool sizeChangeFlag = 0;

TCHAR sKeyState[128];
vector<POINT> polyCont;
vector<POINT> moveCont;
POINT ptPool[1360];
POINT moveT, tmp;

movingPoint player;

RECT rectView;
RECT rectView2;
HBITMAP hDoubleBufferImage;

void DrawRectText(HDC);
void DrawBitmapDoubleBuffering(HWND, HDC);

void CreateBitmap();
void DrawBitmap(HWND, HDC);
void DeleteBitmap();
//void UpdateFrame(HWND);
//VOID CALLBACK AniProc(HWND, UINT, UINT, DWORD);
VOID CALLBACK KeyStateProc(HWND, UINT, UINT, DWORD);
//void territory(HDC, POINT&);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GALSPANIC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALSPANIC));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALSPANIC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GALSPANIC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
	case WM_CREATE:
		CreateBitmap();
		//SetTimer(hWnd, 123, 100, AniProc);
		SetTimer(hWnd, 111, 100, KeyStateProc);
		GetClientRect(hWnd, &rectView);
		GetWindowRect(hWnd, &rectView2);
		polyCont.push_back({ 500, 300 });
		polyCont.push_back({ 700, 300 });
		polyCont.push_back({ 700, 400 });
		polyCont.push_back({ 500, 400 });
		moveT = polyCont[3];
		tmp = moveT;

		player.setPosition(polyCont[3]);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		//case VK_DOWN:
		//	moveT.y += 1;

		/*case 'A':
			framePlus = 0;
			yStart = Sprite_Size_Y;
			keyDown = -8;
			break;
		case 'D':
			framePlus = 0;
			yStart = 0;
			keyDown = 8;
			break;*/
		}
		break;
	case WM_KEYUP:
		/*switch (wParam)
		{
		case 'A':
		case 'D':
			keyDown = 0;
			break;
		}*/
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		//DrawBitmap(hWnd, hdc);
		DrawBitmapDoubleBuffering(hWnd, hdc);
		DrawRectText(hdc);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DeleteBitmap();
		PostQuitMessage(0);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


//static int yPos = 0;
void DrawRectText(HDC hdc)
{
	//static int yPos = 0;
	/*yPos += 5;
	if (yPos > rectView.bottom) yPos = 0;*/
	TCHAR strTest[] = _T("텍스트 출력");
	TextOut(hdc, 10, 0, strTest, _tcslen(strTest));
	//wsprintf(sKeyState, TEXT("%s"), _T("A-Key"));
	TextOut(hdc, 100, 0, sKeyState, _tcslen(sKeyState));
}

void CreateBitmap()
{
	{
		hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/수지.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBackImage, sizeof(BITMAP), &bitBack);
	}
	{
		hBackImage2 = (HBITMAP)LoadImage(NULL, TEXT("images/boeing1.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBackImage2, sizeof(BITMAP), &bitBack2);
	}
	/*{
	hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/zero_run.bmp"),
	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hAniImage, sizeof(BITMAP), &bitAni);
	Run_Frame_Max = bitAni.bmWidth / Sprite_Size_X - 1;
	Run_Frame_Min = 2;
	cur_Frame = Run_Frame_Min;
	}*/
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;

	HDC hMemDC2;
	HBITMAP hOldBitmap2;

	hMemDC = CreateCompatibleDC(hdc);
	if (hDoubleBufferImage == NULL || sizeChangeFlag)
	{
		hDoubleBufferImage = CreateCompatibleBitmap(
			hdc, rectView.right, rectView.bottom);
		sizeChangeFlag = 0;
	}
	hOldBitmap = (HBITMAP)SelectObject(hMemDC,
		hDoubleBufferImage);

	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);
		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage2);
		bx = bitBack2.bmWidth;
		by = bitBack2.bmHeight;


		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, myBrush);
		
		for (unsigned int i = 0; i < polyCont.size(); i++)
			ptPool[i] = polyCont[i];
		Polygon(hMemDC2, ptPool, polyCont.size());

		TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2,
			0, 0, bx, by, RGB(255, 0, 255));
		SelectObject(hMemDC2, oldBrush);
		DeleteObject(myBrush);

		//POINT tmp = moveT;
		MoveToEx(hMemDC, tmp.x, tmp.y, NULL);
		LineTo(hMemDC, moveT.x, moveT.y);


		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	//{
	//	hMemDC2 = CreateCompatibleDC(hMemDC);
	//	hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);
	//	bx = bitAni.bmWidth / 16;
	//	by = bitAni.bmHeight / 2;

	//	int xStart = cur_Frame * bx;
	//	//int yStart = 0;
	//	distance += keyDown;
	//	TransparentBlt(hMemDC, 200 + distance, 100, bx * 4, by * 4,
	//		hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));

	//	SelectObject(hMemDC2, hOldBitmap2);
	//	DeleteDC(hMemDC2);
	//}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom,
		hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hMemDC);
}

//void territory(HDC hMemDC2, POINT& b)
//{
//	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
//	HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, myBrush);
//	Rectangle(hMemDC2, 500, 300, 700, 400);
//
//	TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2,
//		0, 0, bx, by, RGB(255, 0, 255));
//	SelectObject(hMemDC2, oldBrush);
//	DeleteObject(myBrush);
//}

void DeleteBitmap()
{
	DeleteObject(hBackImage);
	DeleteObject(hTransparentImage);
}

VOID CALLBACK KeyStateProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	/*if (GetKeyState('A') & 0x8000)
	{
	wsprintf(sKeyState, TEXT("%s"), _T("A-Key"));
	}
	else if (GetKeyState('S') & 0x8000)
	{
	wsprintf(sKeyState, TEXT("%s"), _T("S-Key"));
	}
	else if (GetKeyState('D') & 0x8000)
	{
	wsprintf(sKeyState, TEXT("%s"), _T("D-Key"));
	}
	else if (GetKeyState('W') & 0x8000)
	{
	wsprintf(sKeyState, TEXT("%s"), _T("W-Key"));
	}*/

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		wsprintf(sKeyState, TEXT("%s"), _T("LEFT-Key"));
	}
	else if (GetKeyState(VK_UP) & 0x8000)
	{
		wsprintf(sKeyState, TEXT("%s"), _T("UP-Key"));
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		wsprintf(sKeyState, TEXT("%s"), _T("RIGHT-Key"));
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		wsprintf(sKeyState, TEXT("%s"), _T("DOWN-Key"));
		moveT.y += 1;
	}
	else if ((GetKeyState(VK_LEFT) & 0x8000) && (GetKeyState(VK_UP) & 0x8000))
	{
		wsprintf(sKeyState, TEXT("%s"), _T("LEFT&UP-Key"));
	}
	else
	{
		wsprintf(sKeyState, TEXT(""));
	}
	InvalidateRgn(hWnd, NULL, false);
}

//void UpdateFrame(HWND hWnd)
//{
//	cur_Frame += (abs(keyDown) / 8) % (Run_Frame_Max - 1);
//	if (cur_Frame > Run_Frame_Max)
//		cur_Frame = Run_Frame_Min;
//}
//
//VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
//{
//	UpdateFrame(hWnd);
//	InvalidateRgn(hWnd, NULL, false);
//	//return VOID();
//
//	yPos += 5;
//	if (yPos > rectView.bottom) yPos = 0;
//}