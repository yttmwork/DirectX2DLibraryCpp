#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <D3dx9math.h>
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

bool Input::Initialize()
{
	// インターフェース作成
	if (CreateInterface() == false)
	{
		return false;
	}

	// キーボード初期化
	if (m_Keyboard.Initialize(m_Interface) == false)
	{
		Release();
		return false;
	}

	// マウス初期化
	if (m_Mouse.Initialize(m_Interface) == false)
	{
		Release();
		return false;
	}

	// ゲームパッド初期化
	// ※接続されていないこともあるので、失敗の判定はしていない
	m_GamePad.Initialize(m_Interface);

	return true;
}

void Input::Release()
{
	m_Keyboard.Release();
	m_Mouse.Release();
	m_GamePad.Release();

	// インターフェースの解放
	if (m_Interface != nullptr)
	{
		m_Interface->Release();
		m_Interface = nullptr;
	}
}

void Input::Update()
{
	m_Keyboard.Update();
	m_Mouse.Update();
	m_GamePad.Update();
}

ButtonState Input::UpdateButtonState(bool is_push, ButtonState state)
{
	if (is_push == true)
	{
		if (state == ButtonState::ButtonStateNone)
		{
			return ButtonState::ButtonStatePushed;
		}
		else
		{
			return ButtonState::ButtonStateHeld;
		}
	}
	else
	{
		if (state == ButtonState::ButtonStateHeld)
		{
			return ButtonState::ButtonStateReleased;
		}
		else
		{
			return ButtonState::ButtonStateNone;
		}
	}

	return ButtonState::ButtonStateNone;
}

bool Input::CreateInterface()
{
	// インターフェース作成
	HRESULT result = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル
		DIRECTINPUT_VERSION,		// DirectInputのバージョン
		IID_IDirectInput8,			// 使用する機能
		(void**)&m_Interface,		// 作成されたインターフェース代入用
		nullptr						// NULL固定
	);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

