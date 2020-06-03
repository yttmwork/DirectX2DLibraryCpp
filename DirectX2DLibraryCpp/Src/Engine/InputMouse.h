/**
* @file InputMouse.h
* @brief <pre>
* マウス入力クラスの宣言
* Inputクラスでインスタンスを作成するので使用者が作成する必要はない
* </pre>
*/
#ifndef INPUT_MOUSE_H_
#define INPUT_MOUSE_H_

#include <dinput.h>
#include "../Useful/Vec.h"

/** @brief マウス入力デバイスクラス */
class Mouse
{
public:
	/**
	* @brief マウス入力初期化関数
	* @details マウスの入力取得に必要な初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	* @param[in] input_interface DirectInputのインターフェース
	*/
	bool Initialize(LPDIRECTINPUT8 input_interface);

	/**
	* @brief マウス入力解放関数
	* @details マウスの入力機能を終了する
	*/
	void Release();

	/**
	* @brief 入力情報の更新関数
	* @details <pre>
	* マウスの入力情報の更新を行う
	* 毎フレーム実行する必要がある
	* </pre>
	*/
	void Update();

	/**
	* @brief マウスボタンの押下状態判定関数
	* @retval true 押されている
	* @retval false 押されていない
	* @param[in] button_type 判定したいボタンの種類
	*/
	bool IsHeld(MouseButton button_type);

	/**
	* @brief マウスボタンが押された瞬間の判定関数
	* @retval true 押した瞬間
	* @retval false 押した瞬間以外
	* @param[in] button_type 判定したいボタンの種類
	*/
	bool IsPushed(MouseButton button_type);

	/**
	* @brief マウスボタンを離した瞬間の判定関数
	* @retval true 離した瞬間
	* @retval false 離した瞬間以外
	* @param[in] button_type 判定したいボタンの種類
	*/
	bool IsReleased(MouseButton button_type);

public:
	LPDIRECTINPUTDEVICE8 m_Device;	// Mouse用Deviceのポインタ
	DIMOUSESTATE m_CurrentState;	// マウスの現在の入力情報
	DIMOUSESTATE m_PrevState;		// マウスの現在の入力情報
	Vec2 m_Pos;						// マウス座標
};


#endif
