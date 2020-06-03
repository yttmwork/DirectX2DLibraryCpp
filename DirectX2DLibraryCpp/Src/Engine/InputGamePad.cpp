#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <D3dx9math.h>
#include "Window.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

bool GamePad::Initialize(LPDIRECTINPUT8 input_interface)
{
	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &m_Device;
	parameter.InputInterface = input_interface;

	// GAMEPADを調べる
	input_interface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
		DeviceFindCallBack,			// 発見時に実行する関数
		&parameter,					// 関数に渡す値
		DIEDFL_ATTACHEDONLY			// 検索方法
	);

	// JOYSTICKを調べる
	input_interface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗
	if (parameter.FindCount == 0)
	{
		return false;
	}

	int count = 0;
	// 制御開始
	while (StartControl() == false)
	{
		Sleep(10);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	// 入力情報の初期化
	for (auto& state : m_States)
	{
		state = ButtonState::ButtonStateNone;
	}

	return true;
}

void GamePad::Release()
{
	// デバイスの解放
	if (m_Device != nullptr)
	{
		// 制御を停止
		m_Device->Unacquire();
		m_Device->Release();
		m_Device = nullptr;
	}
}

bool GamePad::IsButtonHeld(GamePadKind button)
{
	return (m_States[button] == ButtonState::ButtonStateHeld);
}

bool GamePad::IsButtonPushed(GamePadKind button)
{
	return (m_States[button] == ButtonState::ButtonStatePushed);
}

bool GamePad::IsButtonReleased(GamePadKind button)
{
	return (m_States[button] == ButtonState::ButtonStateReleased);
}

void GamePad::Update()
{
	if (m_Device == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;

	// デバイス取得
	HRESULT hr = m_Device->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		// 再度制御開始
		if (FAILED(m_Device->Acquire()))
		{
			for (auto& state : m_States)
			{
				state = ButtonState::ButtonStateNone;
			}

			m_Device->Poll();
		}
		return;
	}

	bool is_push[GamePadKind::GamePadKindMax];
	ZeroMemory(is_push, sizeof(bool) * GamePadKind::GamePadKindMax);

	// スティック判定
	int unresponsive_range = 200;
	if (pad_data.lX < -unresponsive_range)
	{
		is_push[ButtonKind::LeftButton] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{
		is_push[ButtonKind::RightButton] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{
		is_push[ButtonKind::UpButton] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{
		is_push[ButtonKind::DownButton] = true;
	}

	// 十字キー判定
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		float rad = D3DXToRadian((pad_data.rgdwPOV[0] / 100.0f));
		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			is_push[ButtonKind::LeftButton] = true;
		}
		else if (x > 0.01f)
		{
			is_push[ButtonKind::RightButton] = true;
		}

		if (y > 0.01f)
		{
			is_push[ButtonKind::UpButton] = true;
		}
		else if (y < -0.01f)
		{
			is_push[ButtonKind::DownButton] = true;
		}
	}

	int base_index = ButtonKind::Button01;
	// ボタン判定
	for (int i = 0; i < ButtonKind::ButtonKindMax - base_index; i++)
	{
		if (IsButtonInputed(pad_data.rgbButtons[i]) == false)
		{
			continue;
		}

		is_push[base_index + i] = true;
	}

	// 入力情報からボタンの状態を更新する
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		m_States[i] = Input::UpdateButtonState(is_push[i], m_States[i]);
	}
}

bool GamePad::StartControl()
{
	// デバイスが生成されてない
	if (m_Device == nullptr)
	{
		return false;
	}
	
	// 制御開始
	if (FAILED(m_Device->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	m_Device->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(m_Device->Poll()))
		{
			return false;
		}
	}

	return true;
}

BOOL CALLBACK GamePad::DeviceFindCallBack(LPCDIDEVICEINSTANCE pad_instance, LPVOID out_pad_data)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)out_pad_data;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// デバイス生成
	HRESULT hr = parameter->InputInterface->CreateDevice(
		pad_instance->guidInstance,
		parameter->GamePadDevice,
		nullptr);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 入力フォーマットの指定
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return DIENUM_STOP;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	//協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
			FindWindow(WINDOW_CLASS_NAME, nullptr),
			DISCL_EXCLUSIVE | DISCL_FOREGROUND)
		))
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

bool GamePad::IsButtonInputed(BYTE button)
{
	const int ButtonTrg = 0x80;
	return (button & ButtonTrg);
}