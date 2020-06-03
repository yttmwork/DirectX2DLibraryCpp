/**
* @file InputGamePad.h
* @brief <pre>
* ゲームパッド入力クラスの宣言
* Inputクラスでインスタンスを作成するので使用者が作成する必要はない
* </pre>
*/
#ifndef INPUT_GAME_PAD_H_
#define INPUT_GAME_PAD_H_

/** @brief デバイス列挙の結果取得用構造体 */
struct DeviceEnumParameter
{
	LPDIRECTINPUT8 InputInterface;			//!< 結果取得に使ったInputInterface
	LPDIRECTINPUTDEVICE8* GamePadDevice;	//!< デバイス保存用
	int FindCount;							//!< 発見数
};

/** @brief ゲームパッド入力デバイスクラス */
class GamePad
{
public:
	/**
	* @brief ゲームパッド入力初期化関数
	* @details ゲームパッドの入力取得に必要な初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	* @param[in] input_interface DirectInputのインターフェース
	*/
	bool Initialize(LPDIRECTINPUT8 input_interface);

	/**
	* @brief ゲームパッド入力解放関数
	* @details ゲームパッドの入力機能を終了する
	*/
	void Release();

	/**
	* @brief 入力情報の更新
	* @details <pre>
	* ゲームパッドの入力情報の更新を行う
	* 毎フレーム実行する必要がある
	* </pre>
	*/
	void Update();

	/**
	* @brief ゲームパッドボタンの押下状態判定関数
	* @details 指定されたゲームパッドボタンが押されている状態かどうかを判定する
	* @retval true 押されている
	* @retval false 押されていない
	* @param[in] button 判定したいボタンの種類
	*/
	bool IsButtonHeld(GamePadKind button);

	/**
	* @brief ゲームパッドのボタンが押された瞬間の判定関数
	* @details 指定されたゲームパッドボタンが押された瞬間かどうかを判定する
	* @retval true 押した瞬間
	* @retval false 押した瞬間以外
	* @param[in] button 判定したいボタンの種類
	*/
	bool IsButtonPushed(GamePadKind button);

	/**
	* @brief ゲームパッドのボタンを離した瞬間の判定関数
	* @details 指定されたゲームパッドボタンが離された瞬間かどうかを判定する
	* @retval true 離した瞬間
	* @retval false 離した瞬間以外
	* @param[in] button 判定したいボタンの種類
	*/
	bool IsButtonReleased(GamePadKind button);

	/**
	* @brief ゲームパッド制御始動関数
	* @details ゲームパッドの入力受付を開始する
	* @retval true 制御開始成功
	* @retval false 制御開始失敗
	*/
	bool StartControl();

	/**
	* @brief ゲームパッド列挙結果受信関数
	* @details ゲームパッドの接続有無の結果通知を受信する関数
	* @retval true 列挙探索続行
	* @retval false 列挙探索終了
	* @param[in] pad_instance 列挙されたゲームパッドインスタンス
	* @param[out] out_pad_data ゲームパッドデータ保存用
	*/
	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE pad_instance, LPVOID out_pad_data);

private:
	/**
	* @brief ボタンの入力判定関数
	* @retval true 入力状態
	* @retval false 非入力状態
	* @param[in] button 判定したいボタンの種類
	*/
	bool IsButtonInputed(BYTE button);

private:
	LPDIRECTINPUTDEVICE8 m_Device;						// ゲームパッド用Deviceのポインタ
	ButtonState m_States[GamePadKind::GamePadKindMax];	// ゲームパッドの入力状態
};

#endif
