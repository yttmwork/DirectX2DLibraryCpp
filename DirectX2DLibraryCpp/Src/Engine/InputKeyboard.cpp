#include <Windows.h>
#include <math.h>
#include <D3dx9math.h>
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

bool Keyboard::Initialize(LPDIRECTINPUT8 input_interface)
{
	// IDirectInputDevice8インターフェイスの取得
	HRESULT hr = input_interface->CreateDevice(GUID_SysKeyboard, &m_Device, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// デバイスのフォーマットの設定
	hr = m_Device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	// 協調モードの設定
	hr = m_Device->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr))
	{
		return false;
	}

	// デバイスの取得開始
	m_Device->Acquire();

	return true;
}

void Keyboard::Release()
{
	if (m_Device != nullptr)
	{
		// 制御を停止
		m_Device->Unacquire();
		m_Device->Release();
		m_Device = nullptr;
	}
}

void Keyboard::Update()
{
	// キー情報取格納用
	BYTE key_states[MaxKeyNum];
	HRESULT hr;

	if (m_Device == nullptr)
	{
		return;
	}

	// キーボードデバイスのゲッター
	hr = m_Device->GetDeviceState(MaxKeyNum, key_states);
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < MaxKeyNum; i++)
		{
			bool is_push = false;
			if (IsKeyInputed(key_states[i]))
			{
				is_push = true;
			}
			
			m_State[i] = Input::UpdateButtonState(is_push, m_State[i]);
		}
	}
	else if (hr == DIERR_INPUTLOST)
	{
		m_Device->Acquire();
	}
}

bool Keyboard::IsKeyHeld(UINT key_code)
{
	return (m_State[key_code] == ButtonState::ButtonStateHeld);
}

bool Keyboard::IsKeyPushed(UINT key_code)
{
	return (m_State[key_code] == ButtonState::ButtonStatePushed);
}

bool Keyboard::IsKeyReleased(UINT key_code)
{
	return (m_State[key_code] == ButtonState::ButtonStateReleased);
}

bool Keyboard::IsKeyInputed(UINT key)
{
	const int KeyTrg = 0x80;
	return (key & KeyTrg);
}