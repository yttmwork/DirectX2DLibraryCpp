/**
* @file Engine.h
* @brief エンジン処理(描画、入力、サウンド)クラスの宣言
*/
#ifndef ENGINE_H_
#define ENGINE_H_

#include "Graphics.h"
#include "TextureManager.h"
#include "Input.h"
#include "Sound.h"
#include "EngineConstant.h"
#include "Window.h"

/** @brief エンジンクラス */
class Engine
{
public:
	// エンジン関連
	/**
	* @brief 初期化関数
	* @details <pre>
	* ゲームで使うエンジン(描画、入力、サウンド)の初期化とウィンドウの作成を行う
	* この関数はゲームループ開始前に１度だけ実行する
	* </pre>
	* @retval true 初期化成功
	* @retval false 初期化失敗
	* @param[in] width ウィンドウの横幅
	* @param[in] height ウィンドウの縦幅
	* @param[in] title_str タイトルバーに表示する文字列
	* @param[in] is_window_mode ウィンドウ or Fullスクリーン設定フラグ(オプション)
	*/
	static bool Initialize(int width, int height, const char* title_str, bool is_window_mode = true);

	/**
	* @brief 解放関数
	* @details <pre>
	* エンジンの解放を行う
	* この関数はゲームループ終了後に１度だけ実行する
	* </pre>
	*/
	static void Release();

	/**
	* @brief 更新関数
	* @details <pre>
	* 更新処理を行う
	* この関数はゲームループ内で毎フレーム実行する
	* </pre>
	*/
	static void Update();
	
	// 描画関連
	/**
	* @brief 描画開始関数
	* @details <pre>
	* 描画開始を宣言し、バックバッファのクリアを行う
	* この関数実行後は必ずFinishDrawingを実行する必要がある
	* </pre>
	* @retval true 描画開始成功
	* @retval false 描画開始失敗
	* @param[in] color クリアカラー
	*/
	static bool StartDrawing(DWORD color);

	/**
	* @brief 描画終了関数
	* @details <pre>
	* 描画の終了を宣言し、バックバッファとフロントバッファを入れ替える
	* この関数は必ずStartDrawingの後に実行する
	* </pre>
	*/
	static void FinishDrawing();

	/**
	* @brief 矩形描画関数
	* @details 指定された位置に矩形を描画する
	* @param[in] x X軸描画座標
	* @param[in] y Y軸描画座標
	* @param[in] width 矩形横幅
	* @param[in] height 矩形縦幅
	* @param[in] color 矩形の色(RGB形式)(オプション)
	* @param[in] alpha 透過値(オプション)
	* @param[in] angle 回転角度(オプション)
	* @param[in] scale_x 拡縮率X(オプション)
	* @param[in] scale_y 拡縮率Y(オプション)
	*/
	static void DrawRect(float x, float y, float width, float height, DWORD color = 0x00, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

	/**
	* @brief テクスチャ描画関数
	* @details 指定された位置にテクスチャを描画する
	* @param[in] x X軸描画座標
	* @param[in] y Y軸描画座標
	* @param[in] texture_keyword 描画で使うテクスチャのキーワード
	* @param[in] alpha 透過値(オプション)
	* @param[in] angle 回転角度(オプション)
	* @param[in] scale_x 拡縮率X(オプション)
	* @param[in] scale_y 拡縮率Y(オプション)
	*/
	static void DrawTexture(float x, float y, const char* texture_keyword, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

	/**
	* @brief テクスチャ描画関数 UV指定バージョン
	* @details 指定された位置にUV指定されたテクスチャを描画する
	* @param[in] x X軸描画座標
	* @param[in] y Y軸描画座標
	* @param[in] texture_keyword 描画で使うテクスチャのキーワード
	* @param[in] tex_x テクスチャのX座標
	* @param[in] tex_y テクスチャのY座標
	* @param[in] sprite_width スプライト横幅
	* @param[in] sprite_height スプライト縦幅
	* @param[in] alpha 透過値(オプション)
	* @param[in] angle 回転角度(オプション)
	* @param[in] scale_x 拡縮率X(オプション)
	* @param[in] scale_y 拡縮率Y(オプション)
	*/
	static void DrawTextureUV(float x, float y, const char* texture_keyword, float tex_x, float tex_y, float sprite_width, float sprite_height, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

	/**
	* @brief フォント描画関数
	* @details 指定された位置にフォントを描画する
	* @param[in] x X軸描画座標
	* @param[in] y Y軸描画座標
	* @param[in] text 描画するテキスト
	* @param[in] size フォントサイズの種類
	* @param[in] color フォントの色
	*/
	static 	void DrawFont(float x, float y, const char* text, FontSize size, FontColor color);

	/**
	* @brief 描画用矩形の軸設定関数
	* @details 指定された軸の値をm_CurrentPivotに設定する
	* @param[in] pivot_type 更新する軸の値
	*/
	static void SetPivotType(PivotType pivot_type);

	// 入力関連
	/**
	* @brief ゲームパッドボタンの押下状態判定関数
	* @retval true 押されている
	* @retval false 押されていない
	* @param[in] button 判定したいボタンの種類
	*/
	static bool IsGamePadButtonHeld(GamePadKind button);

	/**
	* @brief ゲームパッドのボタンが押された瞬間の判定関数
	* @retval true 押した瞬間
	* @retval false 押した瞬間以外
	* @param[in] button 判定したいボタンの種類
	*/
	static bool IsGamePadButtonPushed(GamePadKind button);

	/**
	* @brief ゲームパッドのボタンを離した瞬間の判定関数
	* @retval true 離した瞬間
	* @retval false 離した瞬間以外
	* @param[in] button 判定したいボタンの種類
	*/
	static bool IsGamePadButtonReleased(GamePadKind button);

	/**
	* @brief キーボードのキーの押下状態判定関数
	* @retval true 押されている
	* @retval false 押されていない
	* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
	*/
	static bool IsKeyboardKeyHeld(UINT key_code);

	/**
	* @brief キーボードのキーが押された瞬間の判定関数
	* @retval true 押された瞬間
	* @retval false 押された瞬間ではない
	* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
	*/
	static bool IsKeyboardKeyPushed(UINT key_code);

	/**
	* @brief キーボードのキーが離された瞬間の判定関数
	* @retval true 離された瞬間
	* @retval false 離された瞬間ではない
	* @param[in] key_code 判定したいキーの種類(DIK_～を使用する)
	*/
	static bool IsKeyboardKeyReleased(UINT key_code);

	/**
	* @brief マウスボタンの押下状態判定関数
	* @retval true 押されている
	* @retval false 押されていない
	* @param[in] button_type 判定したいボタンの種類
	*/
	static bool IsMouseButtonHeld(MouseButton button_type);

	/**
	* @brief マウスボタンが押された瞬間の判定関数
	* @retval true 押した瞬間
	* @retval false 押した瞬間以外
	* @param[in] button_type 判定したいボタンの種類
	*/
	static bool IsMouseButtonPushed(MouseButton button_type);

	/**
	* @brief マウスボタンを離した瞬間の判定関数
	* @retval true 離した瞬間
	* @retval false 離した瞬間以外
	* @param[in] button_type 判定したいボタンの種類
	*/
	static bool IsMouseButtonReleased(MouseButton button_type);

	/**
	* @brief マウス座標のゲッター
	* @retval Vec2 マウスの座標
	*/
	static Vec2 GetMousePos();

	// サウンド関連
	/**
	* @brief サウンド再生関数
	* @details 指定されたキーワードのサウンドファイルを再生する
	* @param[in] keyword 再生するサウンドのキーワード
	* @param[in] is_loop ループ設定(デフォルトは非ループ)
	*/
	static void PlaySound(const char* keyword, bool is_loop = false);

	/**
	* @brief 複製再生関数
	* @details <pre>
	* キーワード指定されたサウンドファイルを複製再生する
	* SEのように同じサウンドを重複して再生する場合に効果的
	* </pre>
	* @param[in] keyword 再生するサウンドのキーワード
	*/
	static void PlayDuplicateSound(const char* keyword);

	/**
	* @brief サウンド停止関数
	* @details <pre>
	* キーワード指定されたサウンドを停止する
	* ※PlayDuplicateSoundを使用して再生しているサウンドは対象外
	* </pre>
	* @param[in] keyword 停止するサウンドのキーワード
	*/
	static void StopSound(const char* keyword);

	/**
	* @brief サウンドファイルの読み込み関数
	* @details 指定されたファイル名のサウンドファイルを読み込み、keywordの文字列で登録する
	* @retval true 読み込み成功
	* @retval false 読み込み失敗
	* @param[in] keyword 登録用キーワード
	* @param[in] file_name ファイル名
	*/
	static bool LoadSoundFile(const char* keyword, const char* file_name);

	/**
	* @brief サウンドファイルの解放関数
	* @details 指定されたキーワードのサウンドファイルを解放する
	* @param[in] keyword 解放するサウンドファイルのキーワード
	*/
	static void ReleaseSoundFile(const char* keyword);

	/**
	* @brief サウンドファイルの全解放関数
	* @details サウンドファイルを全て解放する
	*/
	static void ReleaseAllSoundFiles();

	// テクスチャ関連
	/**
	* @brief テクスチャ読み込み関数
	* @details 指定したされたテクスチャファイルを読み込み、keywordの文字列で登録する
	* @retval true 読み込み成功
	* @retval false 読み込み失敗
	* @param[in] keyword 登録用キーワード
	* @param[in] file_name 読み込むテクスチャファイル名
	*/
	static bool LoadTexture(const char* keyword, const char* file_name);

	/**
	* @brief テクスチャ全解放関数
	* @details 読み込んでいるすべてのテクスチャを解放する
	*/
	static void ReleaseAllTextures();

	/**
	* @brief テクスチャ解放関数
	* @details 指定されたーキーワードのテクスチャを解放する
	* @param[in] keyword 解放するテクスチャのキーワード
	*/
	static void ReleaseTexture(const char* keyword);

	/**
	* @brief テクスチャデータの取得関数
	* @details 指定されたキーワードのテクスチャデータを取得する
	* @retval Texture* テクスチャデータ(取得失敗時はnullptr)
	* @param[in] keyword 取得したいテクスチャのキーワード
	*/
	static Texture* GetTexture(const char* keyword);

	/**
	* @brief テクスチャ作成関数
	* @details <pre>
	* 指定された情報から読み込み、テクスチャを作成する
	* この関数はエンジン側で使用するので使用者は使わない
	* </pre>
	* @retval true 作成成功
	* @retval false 作成失敗
	* @param[in] file_name 読み込むテクスチャの名前(パス込み)
	* @param[out] texture_data 読み込まれたテクスチャを反映するデータ
	*/
	static bool CreateTexture(const char* file_name, Texture* texture_data);

private:
	/**
	* @brief Graphicsインスタンスのゲッター
	* @retval Graphics* Graphicsインスタンス
	*/
	Graphics* GetGraphics()
	{
		return &m_Graphics;
	}

	/**
	* @brief Inputインスタンスのゲッター
	* @retval Input* Inputインスタンス
	*/
	Input* GetInput()
	{
		return &m_Input;
	}

	/**
	* @brief Soundインスタンスのゲッター
	* @retval Sound* Soundインスタンス
	*/
	Sound* GetSound()
	{
		return &m_Sound;
	}

	/**
	* @brief TextureManagerインスタンスのゲッター
	* @retval TextureManager* TextureManagerインスタンス
	*/
	TextureManager* GetTextureManager()
	{
		return &m_TextureManager;
	}

	/**
	* @brief Windowインスタンスのゲッター
	* @retval Window* Windowインスタンス
	*/
	Window* GetWindow()
	{
		return &m_Window;
	}

private:
	static Engine* m_Instance;			//!< インスタンス

private:
	Graphics m_Graphics;				//!< 描画クラス
	Input m_Input;						//!< 入力クラス
	Sound m_Sound;						//!< サウンドクラス
	TextureManager m_TextureManager;	//!< テクスチャ管理クラス
	Window m_Window;					//!< ウィンドウクラス
};

#endif
