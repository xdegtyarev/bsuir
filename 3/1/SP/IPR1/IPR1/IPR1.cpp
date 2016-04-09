#include "stdafx.h"
#include "IPR1.h"

#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBRUSH hbrWhite, hbrGray;
HDC frameBuffer;
HBITMAP hBitmap;
BITMAP bitmap;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL TryGetBitmap(HWND,HDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IPR1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IPR1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IPR1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			hbrWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
			hbrGray = (HBRUSH)GetStockObject(GRAY_BRUSH);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_SELECTBACKGROUNDIMAGE:
				if(TryGetBitmap(hWnd,(HDC)wParam)){
					InvalidateRect(hWnd, NULL, TRUE); //Clearing screen to draw newely acquired image
				}
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			if (!IsIconic(hWnd)) {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				if (hBitmap != 0) {
					frameBuffer = CreateCompatibleDC(hdc);
					SelectObject(frameBuffer, hBitmap);
					BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, frameBuffer, 0, 0, SRCCOPY);
					DeleteDC(frameBuffer);
					RECT validationRect;
					SetRect(&validationRect, 0, 0, bitmap.bmWidth, bitmap.bmHeight);
					ValidateRect(hWnd, &validationRect);
				}
				else {
					
					RECT drawTextRect;
					GetClientRect(hWnd, &drawTextRect);
					DrawText(hdc, L"No image loaded, add one with file menu", -1, &drawTextRect, DT_CENTER);
				}
				EndPaint(hWnd, &ps);
			}
        }
        break;
	case WM_ERASEBKGND:
	{
		HDC hdc = (HDC)wParam;
		RECT rc;
		GetClientRect(hWnd, &rc);
		SetMapMode(hdc, MM_ANISOTROPIC);
		if (hBitmap != NULL) {
			FillRect(hdc, &rc, hbrWhite);
		}
		else {
			FillRect(hdc, &rc, hbrWhite);
		}
	}
		break;
    case WM_DESTROY:
		DeleteObject(hBitmap);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}		

BOOL TryGetBitmap(HWND hwnd, HDC hdc) {
	OPENFILENAMEW ofn;
	wchar_t szFileName[MAX_PATH] = L"()";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"(BMP (*.bmp)\0*.bmp\0)";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"(bmp)";
	if (GetOpenFileName(&ofn))
	{
		hBitmap = (HBITMAP)LoadImage(0, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBitmap == 0) {
			MessageBox(hwnd, L"(Error loading bgImage)", L"(Error)", MB_OK);
			return FALSE;
		} else {
			GetObject(hBitmap, sizeof(bitmap), &bitmap);
			return TRUE;
		}
	}
	return FALSE;
}