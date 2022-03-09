#include "windows.h"
#include <string>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ScrollProc(HWND, UINT, WPARAM, LPARAM);

void RegisterPanel(void);

//COLORREF ShowColorDialog(HWND);
COLORREF gColor = RGB(0, 0, 0);

static HWND hwndRedText;
static HWND hwndGreenText;
static HWND hwndBlueText;

static HWND hwndPanel;
static HWND hwndSrollbarRed;
static HWND hwndSrollbarGreen;
static HWND hwndSrollbarBlue;

static int redPos;        // current red scrolling position 
static int greenPos;        // current green scrolling position 
static int bluePos;        // current blue scrolling position 

static int idActive;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wc = { 0 };
	wc.lpszClassName = TEXT("Color Change");
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;

	RegisterClass(&wc);
	CreateWindow(wc.lpszClassName, TEXT("Color Change"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		500, 350, 800, 400, 0, 0, hInstance, 0);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

/** Create Dialog ColorPicker to test command change background's panel

COLORREF ShowColorDialog(HWND hwnd) {
	CHOOSECOLOR cc;
	static COLORREF crCustClr[16];
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)crCustClr;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);
	return cc.rgbResult;
}
*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	switch (msg)
	{
	case WM_CREATE:
	{

		/** Create Button to test handle change background's pannel
		
		* CreateWindow(TEXT("button"), TEXT("Color"),
				WS_VISIBLE | WS_CHILD,
				20, 30, 80, 25,
				hwnd, (HMENU)1, NULL, NULL);
		*/

		CreateWindow(TEXT("STATIC"), L"RED", WS_CHILD | WS_VISIBLE, 50, 15, 50, 50, hwnd, NULL, NULL, NULL);
		CreateWindow(TEXT("STATIC"), L"GREEN", WS_CHILD | WS_VISIBLE, 170, 15, 50, 50, hwnd, NULL, NULL, NULL);
		CreateWindow(TEXT("STATIC"), L"BLUE", WS_CHILD | WS_VISIBLE, 300, 15, 50, 50, hwnd, NULL, NULL, NULL);


		hwndSrollbarRed = CreateWindow(TEXT("SCROLLBAR"), (LPCWSTR)NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 25, 40, 80, 255, hwnd, (HMENU)1, NULL, NULL);
		hwndSrollbarGreen = CreateWindow(TEXT("SCROLLBAR"), (LPCWSTR)NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 150, 40, 80, 255, hwnd, (HMENU)2, NULL, NULL);
		hwndSrollbarBlue = CreateWindow(TEXT("SCROLLBAR"), (LPCWSTR)NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 280, 40, 80, 255, hwnd, (HMENU)3, NULL, NULL);
		SetScrollRange(hwndSrollbarRed, SB_CTL, 0, 255, FALSE);
		SetScrollRange(hwndSrollbarGreen, SB_CTL, 0, 255, FALSE);
		SetScrollRange(hwndSrollbarBlue, SB_CTL, 0, 255, FALSE);

		redPos = GetScrollPos(hwndSrollbarRed, SB_CTL);
		greenPos = GetScrollPos(hwndSrollbarRed, SB_CTL);
		bluePos = GetScrollPos(hwndSrollbarRed, SB_CTL);

		wchar_t str_initRedValue[256];
		swprintf_s(str_initRedValue, L"%d", redPos);
		wchar_t str_initGreenValue[256];
		swprintf_s(str_initGreenValue, L"%d", greenPos);
		wchar_t str_initBlueValue[256];
		swprintf_s(str_initBlueValue, L"%d", bluePos);

		hwndRedText = CreateWindow(TEXT("STATIC"), str_initRedValue, WS_CHILD | WS_VISIBLE, 50, 320, 100, 30, hwnd, NULL, NULL, NULL);
		hwndGreenText = CreateWindow(TEXT("STATIC"), str_initGreenValue, WS_CHILD | WS_VISIBLE, 170, 320, 100, 30, hwnd, NULL, NULL, NULL);
		hwndBlueText = CreateWindow(TEXT("STATIC"), str_initBlueValue, WS_CHILD | WS_VISIBLE, 300, 320, 100, 30, hwnd, NULL, NULL, NULL);

		RegisterPanel();
		hwndPanel = CreateWindow(TEXT("Panel"), NULL,
			WS_CHILD | WS_VISIBLE,
			400, 0, 500, 400,
			hwnd, (HMENU)4, NULL, NULL);
		break;
	}
	case WM_VSCROLL:
	{
		//* How to get the element being interacted with by the user for set ScrollProc foreach ScrollBar

			ScrollProc(hwndSrollbarRed, msg, wParam,lParam);
			//ScrollProc(hwndSrollbarGreen, msg, wParam, lParam);
			//ScrollProc(hwndSrollbarBlue, msg, wParam, lParam);
	}
	case WM_COMMAND:
	{
		//gColor = ShowColorDialog(hwnd); get ColorCode for set PanelColor
		InvalidateRect(hwndPanel, NULL, TRUE);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}



LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam
)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	switch (msg)
	{
	case WM_PAINT:
	{
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		SetBkColor(hdc, gColor);
		ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, TEXT(""), 0, NULL);
		EndPaint(hwnd, &ps);
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
LRESULT CALLBACK ScrollProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	int position = GetScrollPos(hwnd,SB_CTL);
	switch (LOWORD(wParam))
	{
	case SB_LINEUP: {
		if (position > 0) {
			position--;
		}
		break;
	}
	case SB_LINEDOWN: {
		if (position < 255) {
			position++;
		}
		break;
	}
	case SB_THUMBTRACK: {
		position = HIWORD(wParam);
		break;
	}
	}
	SetScrollPos(hwnd, SB_CTL, position, true);
	int ojID = GetDlgCtrlID(hwnd);

	switch (ojID)
	{
	case 1: {
		redPos = position;
		wchar_t str[256];
		swprintf_s(str, L"%d", position);
		SetWindowText(hwndRedText, str);
		break;
		}
	case 2: {
		greenPos = position;
		wchar_t str[256];
		swprintf_s(str, L"%d", position);
		SetWindowText(hwndGreenText, str);
		break;
		}
	case 3: {
		bluePos = position;
		wchar_t str[256];
		swprintf_s(str, L"%d", position);
		SetWindowText(hwndBlueText, str);
		break;
		}
	}
	gColor = RGB(redPos, greenPos, bluePos);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
void RegisterPanel(void) {
	WNDCLASS rwc = { 0 };
	rwc.lpszClassName = TEXT("Panel");
	rwc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	rwc.lpfnWndProc = PanelProc;
	RegisterClass(&rwc);
}


