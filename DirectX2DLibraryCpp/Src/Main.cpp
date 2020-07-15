#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 g_Position = Vec2(300.0f, 200.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;
int g_PivotType = PivotType::LeftTop;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

/*
	エントリポイント
*/
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	// テクスチャ読み込み
	// 第一引数の文字列で読み込んだテクスチャを登録する
	// 描画や取得は登録した文字列で指定する
	Engine::LoadTexture("Enemy", "Res/Enemy.png");

	// サウンド読み込み
	// 第一引数の文字列で読み込んだサウンドを登録する
	// 描画や取得は登録した文字列で指定する
	Engine::LoadSoundFile("Bgm", "Res/Bgm.wav");
	Engine::LoadSoundFile("Se", "Res/Se.wav");

	// サウンド再生
	// 指定されたキーワードのサウンドファイルを再生する
	Engine::PlaySound("Bgm", true);

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;

} // プログラム終了

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	float speed = 2.0f;

	g_Angle += 1.0f;

	// キーボードの入力取得
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	{
		g_Position.X -= speed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) == true)
	{
		g_Position.X += speed;
	}

	// ゲームパッドの入力取得
	if (Engine::IsGamePadButtonHeld(GamePadKind::GamePadKindUp))
	{
		g_Position.Y -= speed;
	}
	else if (Engine::IsGamePadButtonHeld(GamePadKind::GamePadKindDown))
	{
		g_Position.Y += speed;
	}

	// マウスの入力取得
	if (Engine::IsMouseButtonPushed(MouseButton::Left))
	{
		g_Position = Vec2(200, 200);
	}
	else if (Engine::IsMouseButtonPushed(MouseButton::Right))
	{
		g_Position = Vec2(400, 300);
	}

	// 軸の切り替え
	if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
	{
		g_PivotType++;
		if (g_PivotType >= PivotType::MaxPivotType)
		{
			g_PivotType = PivotType::LeftTop;
		}
	}

	// 
	if (Engine::IsKeyboardKeyReleased(DIK_A))
	{
		// 重複再生
		// 指定されたキーワードのサウンドファイル再生する
		// StartPlayingDuplicateSoundでは同じサウンドファイルでも重複して再生可能
		Engine::PlayDuplicateSound("Se");
		Engine::PlayDuplicateSound("Se");
	}
	
	static bool is_add = true;
	float add = (is_add == true) ? 0.01f : -0.01f;
	g_Scale.X += add;
	g_Scale.Y += add;

	if (g_Scale.X > 3.0f ||
		g_Scale.X < 0.2f)
	{
		is_add = !is_add;
	}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// テクスチャ描画
	// キーワードで指定されたテクスチャを描画する
	// DrawTextureはテクスチャをそのまま描画する
	// 一部切り取って描画する場合はDrawTextureUVを使用する
	Engine::SetPivotType(PivotType::LeftTop);
	Engine::DrawTexture(300, 200, "Enemy", 128, 0.0f, 1.0f, 1.0f);

	Engine::SetPivotType((PivotType)g_PivotType);
	Engine::DrawTextureUV(300, 200, "Enemy", 0.0f, 0.0f, 64.0f, 64.0f, 128, 0.0f, g_Scale.X, g_Scale.Y);

	// フォント描画
	Engine::DrawFont(0.0f, 5.0f, "FontSize:Small", FontSize::Small, FontColor::White);
	Engine::DrawFont(0.0f, 30.0f, "FontSize:Regular", FontSize::Regular, FontColor::White);
	Engine::DrawFont(0.0f, 60.0f, "FontSize:Large", FontSize::Large, FontColor::White);

	// 軸描画
	const char* pivot_string_list[] =
	{
		"X:Left Y:Top",
		"X:Center Y:Top",
		"X:Right Y:Top",

		"X:Left Y:Center",
		"X:Center Y:Center",
		"X:Right Y:Center",

		"X:Left Y:Bottom",
		"X:Center Y:Bottom",
		"X:Right Y:Bottom",
	};
	Engine::DrawFont(300.0f, 15.0f, pivot_string_list[(int)g_PivotType], FontSize::Regular, FontColor::White);


	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}
