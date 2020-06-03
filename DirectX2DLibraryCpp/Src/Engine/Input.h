/**
* @file Input.h
* @brief <pre>
* 入力クラスの宣言
* Engineクラスでインスタンスを作成するので使用者が作成する必要はない
* Inputクラスの関数はEngineクラス、またはEngine内で使用するため、利用者が直接使用しない
* </pre>
*/
#ifndef INPUT_H_
#define INPUT_H_

#define DIRECTINPUT_VERSION 0x0800	//!< DirectInputのWarning対策

#include <dinput.h>
#include "InputKeyboard.h"
#include "InputGamePad.h"
#include "InputMouse.h"
#include "EngineConstant.h"

/** @brief 入力クラス */
class Input
{
public:
	/**
	* @brief Input機能初期化関数
	* @details 入力取得に必要な初期化を行う	
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize();

	/**
	* @brief Input機能終了関数
	* @details Input機能を終了させる
	*/
	void Release();

	/**
	* @brief 入力情報の更新
	* @details デバイスの入力情報の更新を行う
	*/
	void Update();


	/**
	* @brief ボタン状態の更新
	* @details ボタンの押下情報から最新のボタンの状態を返す
	* @retval 更新されたボタンの状態
	* @param[in] is_push ボタンの押下状態
	* @param[in] state ボタンの状態
	*/
	static ButtonState UpdateButtonState(bool is_push, ButtonState state);

	/**
	* @brief Inputインタフェース作成
	* @details DirectInputのインターフェースを作成する
	* @retval true 作成成功
	* @retval false 作成失敗
	*/
	bool CreateInterface();

	/**
	* @brief Keyboardインスタンスのゲッター
	* @retval Keyboard* Keyboardインスタンス
	*/
	Keyboard* GetKeyboard()
	{
		return &m_Keyboard;
	}

	/**
	* @brief GamePadインスタンスのゲッター
	* @retval GamePad* GamePadインスタンス
	*/
	GamePad* GetGamePad()
	{
		return &m_GamePad;
	}

	/**
	* @brief Mouseインスタンスのゲッター
	* @retval Mouse* Mouseインスタンス
	*/
	Mouse* GetMouse()
	{
		return &m_Mouse;
	}

private:
	LPDIRECTINPUT8 m_Interface;	//!< DIRECTINPUT8のポインタ
	Keyboard m_Keyboard;		//!< 入力デバイス(キーボード)
	GamePad m_GamePad;			//!< 入力デバイス(ゲームパッド)
	Mouse m_Mouse;				//!< 入力デバイス(マウス)
};

#endif
