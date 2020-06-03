/**
* @file InputKeyboard.h
* @brief <pre>
* キーボード入力クラスの宣言
* Inputクラスでインスタンスを作成するので使用者が作成する必要はない
* </pre>
*/
#ifndef INPUT_KEYBOARD_H_
#define INPUT_KEYBOARD_H_

#include <dinput.h>
#include "EngineConstant.h"

/** @brief キーボード入力デバイスクラス */
class Keyboard
{
public:
	/**
	* @brief キーボード入力初期化関数
	* @details キーボードの入力取得に必要な初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	* @param[in] input_interface DirectInputのインターフェース
	*/
	bool Initialize(LPDIRECTINPUT8 input_interface);

	/**
	* @brief キーボード入力解放関数
	* @details キーボードの入力機能を終了する
	*/
	void Release();

	/**
	* @brief 入力情報の更新
	* @details <pre>
	* キーボードの入力情報の更新を行う
	* 毎フレーム実行する必要がある
	* </pre>
	*/
	void Update();

	/**
	* @brief キーボードのキーの押下状態判定関数
	* @retval true 押されている
	* @retval false 押されていない
	* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
	*/
	bool IsKeyHeld(UINT key_code);

	/**
	* @brief キーボードのキーが押された瞬間の判定関数
	* @retval true 押された瞬間
	* @retval false 押された瞬間ではない
	* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
	*/
	bool IsKeyPushed(UINT key_code);

	/**
	* @brief キーボードのキーが離された瞬間の判定関数
	* @retval true 離された瞬間
	* @retval false 離された瞬間ではない
	* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
	*/
	bool IsKeyReleased(UINT key_code);

private:
	/**
	* @brief キーの入力判定関数
	* @retval true 入力状態
	* @retval false 非入力状態
	* @param[in] key 判定したいキーの種類
	*/
	bool IsKeyInputed(UINT key);

private:
	LPDIRECTINPUTDEVICE8 m_Device;		//!< Keyboard用Deviceのポインタ
	ButtonState m_State[MaxKeyNum];		//!< キーボード入力状態
};

#endif
