/**
* @file EngineConstant.h
* @brief エンジン処理(描画、入力、サウンド)に関する関数、定数の宣言
*/
#ifndef ENGINE_CONSTANT_H_
#define ENGINE_CONSTANT_H_

#include <d3dx9.h>

const int SmallFontSize = 16;	//!< フォントサイズ(小)
const int RegularFontSize = 24;	//!< フォントサイズ(中)
const int LargeFontSize = 32;	//!< フォントサイズ(大)
const int MaxKeyNum = 256;		//!< キー最大数

/** @brief 描画用矩形の軸の種類 */
enum PivotType
{
	LeftTop,		//!< X軸：左 Y軸：上
	CenterTop,		//!< X軸：中 Y軸：上
	RightTop,		//!< X軸：右 Y軸：上
	LeftCenter,		//!< X軸：左 Y軸：中
	CenterCenter,	//!< X軸：中 Y軸：中
	RightCenter,	//!< X軸：右 Y軸：中
	LeftBottom,		//!< X軸：左 Y軸：下
	CenterBottom,	//!< X軸：中 Y軸：下
	RightBottom,	//!< X軸：右 Y軸：下
	MaxPivotType
};

/** @brief フォントカラー */
enum FontColor
{
	Black,			//!< 黒
	White,			//!< 白
	Red,			//!< 赤
};

/** @brief フォントサイズ */
enum FontSize
{
	Small,			//!< 小
	Regular,		//!< 中
	Large,			//!< 大
	FontSizeMax,	//!< サイズ最大数
};

/** @brief キーボタンの種類 */
enum ButtonKind
{
	UpButton,
	DownButton,
	LeftButton,
	RightButton,
	Button01,
	Button02,
	Button03,
	Button04,
	Button05,
	Button06,
	Button07,
	Button08,
	ButtonKindMax,
};

/** @brief ボタンの状態 */
enum ButtonState
{
	ButtonStateNone,		//!< 未使用状態
	ButtonStatePushed,		//!< 押した瞬間
	ButtonStateHeld,		//!< 押している
	ButtonStateReleased,	//!< 離した瞬間
	ButtonStateMax,			//!< 最大数
};

/** @brief ゲームパッドボタンの種類 */
enum GamePadKind
{
	GamePadKindUp,
	GamePadKindDown,
	GamePadKindLeft,
	GamePadKindRight,
	GamePadKindButton01,
	GamePadKindButton02,
	GamePadKindButton03,
	GamePadKindButton04,
	GamePadKindButton05,
	GamePadKindButton06,
	GamePadKindButton07,
	GamePadKindButton08,
	GamePadKindMax,
};

/** @brief マウスボタンの種類 */
enum MouseButton
{
	Left,		//!< 左
	Right,		//!< 右
	Center,		//!< 真ん中
};

/** @brief 頂点データ */
struct CustomVertex
{
	float X;			//!< X座標
	float Y;			//!< Y座標
	float Z;			//!< Z座標	
	float Rhw;			//!< 除算数
	DWORD Color;		//!< 頂点カラー
	float TextureX;		//!< テクスチャ座標X
	float TexrureY;		//!< テクスチャ座標Y
};

/** @brief テクスチャデータやサイズを保持する構造体 */
struct Texture
{
	LPDIRECT3DTEXTURE9 TextureData;	//!< テクスチャデータ
	int Width;						//!< 横幅
	int Height;						//!< 縦幅
};

#endif
