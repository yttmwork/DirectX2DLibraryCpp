#include "Window.h"

LRESULT CALLBACK Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:				
		if (wparam == VK_ESCAPE)
		{	
			// アプリケーションを終了する
			PostMessage(window_handle, WM_CLOSE, 0, 0);
		}
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

bool Window::MakeWindow(int width, int height, const char* title)
{
	HWND hWnd;
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),					// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,			// クラスのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 補助メモリ
		0,									// 補助メモリ
		GetModuleHandle(nullptr),			// このプログラムのインスタンスハンドル
		LoadIcon(nullptr, IDI_APPLICATION),// アイコン画像
		LoadCursor(nullptr, IDC_ARROW),		// カーソル画像
		nullptr,							// 背景ブラシ(背景色)
		nullptr,							// メニュー名
		WINDOW_CLASS_NAME,					// クラス名									
		nullptr								// 小さいアイコン
	};

	// 構造体の登録
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	// ウィンドウ作成
	hWnd = CreateWindow(
		WINDOW_CLASS_NAME,
		title,
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
		CW_USEDEFAULT,
		0,
		width,
		height,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	if (hWnd == nullptr)
	{
		return 0;
	}

	RECT window_rect;
	RECT client_rect;

	GetWindowRect(hWnd, &window_rect);
	GetClientRect(hWnd, &client_rect);

	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	SetWindowPos(hWnd, nullptr, CW_USEDEFAULT, CW_USEDEFAULT, frame_size_x + width, frame_size_y + height, SWP_NOMOVE);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return hWnd;
}
