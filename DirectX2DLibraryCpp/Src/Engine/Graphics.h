/**
* @file Graphics.h
* @brief <pre>
* 描画クラスの宣言
* Engineクラスでインスタンスを作成するので使用者が作成する必要はない
* Graphicsクラスの関数はEngineクラス、またはEngine内で使用するため、利用者が直接使用しない
* </pre>
*/
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "EngineConstant.h"
#include "../Common/Vec.h"
#include "../Common/Size.h"

/** @brief 描画クラス */
class Graphics
{
public:
	/**
	* @brief Graphics機能初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	* @param[in] is_window_mode ウィンドウ or Fullスクリーン設定フラグ
	*/
	bool Initialize(bool is_window_mode);

	/**
	* @brief Graphics機能終了関数
	* @details 描画関連の処理の後始末を行う処理
	*/
	void Release();

	/**
	* @brief 描画開始関数
	* @details <pre>
	* 描画開始を宣言し、バックバッファのクリアを行う
	* この関数実行後は必ずDrawEndを実行する必要がある
	* </pre>
	* @retval true 描画開始成功
	* @retval false 描画開始失敗
	* @param[in] color クリアカラー
	*/
	bool StartDraw(DWORD color);

	/**
	* @brief 描画終了関数
	* @details <pre>
	* 描画の終了を宣言し、バックバッファとフロントバッファを入れ替える
	* この関数は必ずDrawStartの後に実行する必要がある
	* </pre>
	*/
	void FinishDraw();

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
	void DrawRect(float x, float y, float width, float height, DWORD color = 0x00, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

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
	void DrawTexture(float x, float y, const char* texture_keyword, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

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
	void DrawTextureUV(float x, float y, const char* texture_keyword, float tex_x, float tex_y, float sprite_width, float sprite_height, UCHAR alpha = 255, float angle = 0.0f, float scale_x = 1.0f, float scale_y = 1.0f);

	/**
	* @brief フォント描画関数
	* @details 指定された位置にフォントを描画する
	* @param[in] x X軸描画座標
	* @param[in] y Y軸描画座標
	* @param[in] text 描画するテキスト
	* @param[in] size フォントサイズの種類
	* @param[in] color フォントの色
	*/
	void DrawFont(float x, float y, const char* text, FontSize size, FontColor color);

	/**
	* @brief 描画用矩形の軸設定関数
	* @details 指定された軸の値をm_CurrentPivotに設定する
	* @param[in] pivot_type 更新する軸の値
	*/
	void SetPivotType(PivotType pivot_type);

	/**
	* @brief テクスチャ作成関数
	* @details 指定された情報から読み込み、テクスチャを作成する
	* @retval true 作成成功
	* @retval false 作成失敗
	* @param[in] file_name 読み込むテクスチャの名前(パス込み)
	* @param[out] texture_data 読み込まれたテクスチャを反映するデータ
	*/
	bool CreateTexture(const char* file_name, Texture* texture_data);

private:
	/**
	* @brief Graphicsインタフェース作成関数
	* @details DirectGraphicsのインターフェースを作成する
	* @retval true 作成成功
	* @retval false 作成失敗
	*/
	bool CreateInterface();

	/**
	* @brief Graphicsdデバイス作成関数
	* @details DirectGraphicsのデバイスを作成する
	* @retval true 作成成功
	* @retval false 作成失敗
	* @param[in] present_param デバイス設定に使用するプレゼントパラメータ
	* @param[in] is_window_mode ウィンドウモード
	*/
	bool CreateDevice(D3DPRESENT_PARAMETERS* present_param, bool is_window_mode);

	/**
	* @brief ビューポート設定関数
	* @retval true 作成成功
	* @retval false 作成失敗
	* @param[in] present_param バッファサイズを保持するデータ
	*/
	bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param);

	/**
	* @brief フォントデバイス作成関数
	* @details DirectFontのデバイスを作成する
	* @retval true 作成成功
	* @retval false 作成失敗
	*/
	bool CreateFontDevice();

	/**
	* @brief 矩形変換関数
	* @details 引数の矩形に対し、移動、回転、拡大を行う
	* @param[out] vertices 変換対象の矩形
	* @param[in] pos_x 移動座標(X軸)
	* @param[in] pos_y 移動座標(Y軸)
	* @param[in] angle 回転角度
	* @param[in] scale_x 拡縮率(X軸)
	* @param[in] scale_y 拡縮率(Y軸)
	*/
	void TransformRect(CustomVertex* vertices, float pos_x, float pos_y, float angle, float scale_x, float scale_y);

	/**
	* @brief オフセット値計算関数
	* @details m_CurrentPivotに設定されている値を参考にして軸のオフセット値を計算して返す
	* @retval オフセット値
	* @param[in] rect_size オフセット値の参考に使う矩形のサイズ
	*/
	Vec2 CalculatePivotOffset(Size* rect_size);
private:
	LPDIRECT3D9 m_D3DInterface;						//!< DirectGraphicsインターフェース
	LPDIRECT3DDEVICE9 m_D3DDevice;					//!< DirectGraphicsデバイス
	LPD3DXFONT m_FontList[FontSize::FontSizeMax];	//!< フォントデバイスリスト
	PivotType m_CurrentPivot;						//!< 描画用矩形の軸
};

#endif
