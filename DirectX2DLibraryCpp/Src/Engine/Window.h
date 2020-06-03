/**
* @file Window.h
* @brief <pre>
* ウィンドウクラスの宣言
* Engineクラスでインスタンスを作成するので使用者が作成する必要はない
* </pre>
*/
#ifndef WINDOW_H_
#define WINDOW_H_

#include <Windows.h>

#define WINDOW_CLASS_NAME "Window"	//!< ウィンドウクラス名

/** @brief ウィンドウクラス */
class Window
{
public:
	/**
	* @brief ウィンドウ生成関数
	* @details 引数で指定された内容でウィンドウを作成する
	* @retval true 生成成功
	* @retval false 生成失敗
	* @param[in] width 横幅
	* @param[in] height 縦幅
	* @param[in] title タイトルバーに表示される文字列
	*/
	bool MakeWindow(int width, int height, const char* title);

	/**
	* @brief ウィンドウプロシージャ関数
	* @details Windowsのメッセージ対応を行う
	* @retval LRESULT 対応結果
	* @param[in] window_handle 対象になっているウィンドウハンドル
	* @param[in] message_id メッセージ
	* @param[in] wparam メッセージ情報その①
	* @param[in] lparam メッセージ情報その②
	*/
	static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam);
};


#endif
