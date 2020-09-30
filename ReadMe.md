# ReadMe

## 概要
DirectX9を使用して作成した2Dゲーム用の簡易ライブラリです。  
テクスチャ描画、キーボードやマウスの入力、サウンドの再生を行うことができます。

## 素材
サンプルで使用している音素材は[魔王魂](https://maoudamashii.jokersounds.com/)の素材を使用しております。

## 環境構築
ライブラリにはDirectX9を使用しています。 
- DirectX9は[こちら](https://yttm-work.jp/directx/directx_0023.html)を参考にインストールまで行って下さい。  
- 各機能を使用するためのパス指定は以下のURLから確認できます。
 - [DirectGraphics](https://yttm-work.jp/directx/directx_0003.html#common_headline_direct_graphics_setting)
 - [DirectInput](https://yttm-work.jp/directx/directx_0011.html#common_headline_direct_input_setting)
 - [DirectSound](https://yttm-work.jp/directx/directx_0033.html#head_line_02)

## 使用方法
### include
使用するソースファイルでEngine.hを#includeに設定して下さい。  
使用する関数は「Engine::～」で実行します。

```
#include "Engine.h"
```

### 必須処理
#### 初期化
プログラム開始直後にInitialize関数を使用してDirectGraphics、DirectInput、DirectSoundの初期化を行います。  
この関数はプログラム内で1度だけ実行します。

```
// エンジンの初期化
if (Engin::Initialize(
	640, 		// ウィンドウサイズ(横幅)
	480, 		// ウィンドウサイズ(縦幅)
	"Sample"	// タイトルバーのタイトル
	true		// ウィンドウモード
	) == false)
{
	return 0;
}
```

#### 更新
毎フレームUpdate関数を実行します。  
エンジン側で毎フレーム更新を行わなければいけない処理を実行していますので、必ず実行してください。

```
// 更新
Engine::Update();
```

#### 解放
プログラム終了直前にRelease関数を使用してDirectGraphics、DirectInput、DirectSoundの解放を行います。  
この関数もInitialize同様にプログラム中に1度しか実行しません。

```
// エンジンの解放
Engine::Release();
```

### Texture関連
#### テクスチャ読み込み
```
// テクスチャ読み込み
// 読み込むファイル名と登録用のキーワードを設定する
// 読み込みが完了後は登録したキーワードを使用してデータの取得や解放を行う
bool is_success = Engine::LoadTexture("Enemy", "Res/Enemy.png");
```

#### テクスチャ取得
```
// テクスチャ情報の取得
// 指定されたキーワードが存在したらデータが返る
// Textureにはテクスチャデータやテクスチャのサイズが保存されている
Texture* Engine::GetTexutre("Enemy");
```

#### テクスチャ解放
```
// 単体解放
// 指定されたキーワードのテクスチャを解放する
Engine::ReleaseTexture("Enemy")

// 全解放
// 読み込まれているテクスチャを全て解放する
Engine::ReleaseAllTextures();
```

### 描画関連
#### 描画開始/終了
```
// 描画開始
// 描画処理を実行する場合、必ず最初実行する
// 1フレームに1度だけ実行する
Engine::StartDrawing(0);

// テクスチャなどの描画処理を実行する

// 描画終了
// 描画処理を終了する場合に必ず実行する
Engine::FinishDrawing();
```

#### 矩形描画
``` 
// 矩形描画
// 描画は座標を矩形の左上の位置として描画する
// 矩形の色や回転、拡縮はオプション
Engine::DrawRect(
	g_Position.X,  // 描画座標X
	g_Position.Y,  // 描画座標Y
	g_Width,       // 矩形の横幅
	g_Height,      // 矩形の縦幅
	g_Color,       // 矩形の色
	g_Alpha,       // 透過値
	g_Angle,       // 回転角度
	g_Scale.X,     // 拡縮率X
	g_Scale.Y);    // 拡縮率Y

```


#### テクスチャ描画
``` 通常
// テクスチャ描画
// 描画は座標を矩形の左上の位置として描画する
// 回転角度、拡縮率はオプション
Engine::DrawTexture(
	g_Position.X,  // 描画座標X
	g_Position.Y,  // 描画座標Y
	"Enemy",       // 使用するテクスチャのキーワード
	g_Alpha,       // 透過値
	g_Angle,       // 回転角度
	g_Scale.X,     // 拡縮率X
	g_Scale.Y);    // 拡縮率Y

// UV座標指定
Engine::DrawTextureUV(
	g_Position.X, // 描画座標X
	g_Position.Y, // 描画座標Y
	"Enemy",      // 使用するテクスチャのキーワード
	0.0f,         // テクスチャ座標
	0.0f,         // テクスチャ座標
	64.0f,        // テクスチャの描画サイズ(横幅)
	64.0f,        // テクスチャの描画サイズ(縦幅)
	g_Alpha,      // 透過値
	g_Angle,      // 回転角度
	g_Scale.X,    // 拡縮率X
	g_Scale.Y);   // 拡縮率Y

```

#### フォント描画
```
// フォント描画
Engine::DrawFont(
	0.0f,              // 描画座標X
	0.0f,              // 描画座標Y
	"FontSize:Small",  // 描画する文字列
	FontSize::Small,   // フォントサイズ
	FontColor::Black); // フォントカラー
```

#### 軸の指定
```
// 描画に使用する矩形の軸を設定する
// DrawTexture or DrawTextureUVで指定する座標指定や拡縮率、回転は軸を中心に行う
Engine::SetPivotType(PivotType::LeftTop);
```

### DirectInput
#### 入力情報
以下の入力情報を取得できる
- 押された瞬間
- 押している間
- 離された瞬間

#### キーボード入力取得
```
// キーの指定はDIK～で行う
// 押された瞬間
if (Engine::IsKeyboardKeyPushed(DIK_A) == true)
{
	// Aキーが押された瞬間
}

// 押している間
if (Engine::IsKeyboardKeyHeld(DIK_A) == true)
{
	// Aキーが押されている
}

// 離された瞬間
if (Engine::IsKeyboardKeyReleased(DIK_A) == true)
{
	// Aキーが離された瞬間
}
```

#### マウス入力取得
```
// 押された瞬間
if (Engine::IsMouseButtonPushed(MouseButton::Left) == true)
{
	// マウスの左ボタンを押した瞬間
}

// 押している間
if (Engine::IsMouseButtonHeld(MouseButton::Left) == true)
{
	// マウスの左ボタンを押している
}

// 離された瞬間
if (Engine::IsMouseButtonReleased(MouseButton::Left) == true)
{
	// マウスの左ボタンを離した瞬間
}
```

#### ゲームパッド入力取得
```
// 押された瞬間
if (Engine::IsGamePadButtonPushed(GamePadKind::GamePadKindButton01) == true)
{
	// ゲームパッドのボタン1を押した瞬間
}

// 押している間
if (Engine::IsGamePadButtonHeld(GamePadKind::GamePadKindButton01) == true)
{
	// ゲームパッドのボタン1を押している
}

// 離された瞬間
if (Engine::IsGamePadButtonReleased(GamePadKind::GamePadKindButton01) == true)
{
	// ゲームパッドのボタン1を離した瞬間
}
```

#### マウス座標の取得
```
// マウス座標の取得
Vec2 mouse_pos = Engine::GetMousePos();
```


### DirectSound
#### サウンドファイル読み込み
``` 
// ファイルの読み込み
// 読み込むファイル名と登録用のキーワードを設定する
// 読み込みが完了後は登録したキーワードを使用してデータの取得や解放を行う
// 対応フォーマットはwav
Engine::LoadSoundFile("Bgm", "Res/Bgm.wav");
```

#### サウンドファイル解放
```
// 指定したキーワードのサウンドファイルを解放する
Engine::ReleaseSoundFile("Bgm");

// サウンドファイルを全て解放する
Engine::ReleaseAllSoundFiles();
```

#### サウンド再生
```
// 通常再生
// 停止時に先頭に戻す処理を実装しているため再生は先頭から行われる
// Bgmの再生に効果的
// Stop関数で再生を止めることが出来る
Engine::PlaySound("Bgm", true);

// 複製再生
// 同じ音を同時に再生することが出来る
// SEの再生に効果的
// Stopによる停止はできない
Engine::PlayDuplicateSound("Se");
```

#### サウンド停止
```
// 停止
// 指定したキーワードの音を停止する
Engine::StopSound("Bgm");
```
