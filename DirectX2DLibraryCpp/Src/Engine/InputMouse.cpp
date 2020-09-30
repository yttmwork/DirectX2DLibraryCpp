#include <Windows.h>
#include <stdio.h>
#include "Window.h"
#include "Input.h"
#include "InputMouse.h"


bool Mouse::Initialize(LPDIRECTINPUT8 input_interface)
{
	if (m_Device != nullptr)
	{
		m_Device->Release();
		m_Device = nullptr;
	}

	// デバイス生成
	HRESULT hr = input_interface->CreateDevice(
		GUID_SysMouse,
		&m_Device,
		nullptr);

	if (FAILED(hr))
	{
		return false;
	}

	// 入力フォーマットの指定
	hr = m_Device->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	// 協調モードの設定
	hr = m_Device->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
	{
		return false;
	}

	m_Device->Acquire();

	return true;
}

void Mouse::Release()
{
	if (m_Device != nullptr)
	{
		// 制御を停止
		m_Device->Unacquire();
		m_Device->Release();
		m_Device = nullptr;
	}
}

void Mouse::Update()
{
	if (m_Device == nullptr)
	{
		return;
	}

	// 更新前に最新マウス情報を保存する
	m_PrevState = m_CurrentState;

	// マウスの状態を取得します
	HRESULT	hr = m_Device->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentState);
	if (FAILED(hr))
	{
		m_Device->Acquire();
		hr = m_Device->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentState);
	}

	Vec2 prev = m_Pos;
	POINT p;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&p);

	// スクリーン座標にクライアント座標に変換する
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	// 変換した座標を保存
	m_Pos.X = (float)p.x;
	m_Pos.Y = (float)p.y;
}

bool Mouse::IsButtonHeld(MouseButton button_type)
{
	if (IsButtonInputed(m_PrevState.rgbButtons[button_type]) == false &&
		IsButtonInputed(m_CurrentState.rgbButtons[button_type]) == true)
	{
		return true;
	}

	return false;
}

bool Mouse::IsButtonPushed(MouseButton button_type)
{
	if (IsButtonInputed(m_PrevState.rgbButtons[button_type]) == true &&
		IsButtonInputed(m_CurrentState.rgbButtons[button_type]) == true)
	{
		return true;
	}

	return false;
}

bool Mouse::IsButtonReleased(MouseButton button_type)
{
	if (IsButtonInputed(m_PrevState.rgbButtons[button_type]) == true &&
		IsButtonInputed(m_CurrentState.rgbButtons[button_type]) == false)
	{
		return true;
	}

	return false;
}

bool Mouse::IsButtonInputed(BYTE button)
{
	const int MouseTrg = 0x80;
	return (button & MouseTrg);
}

Vec2 Mouse::GetPos()
{
	return m_Pos;
}