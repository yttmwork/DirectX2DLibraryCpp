#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "TextureManager.h"
#include "Sound.h"
#include "Engine.h"

Engine* Engine::m_Instance = nullptr;

bool Engine::Initialize(int width, int height, const char* title_str, bool is_window_mode)
{
	m_Instance = new Engine();

	if (m_Instance->GetWindow()->MakeWindow(width, height, title_str) == false)
	{
		return false;
	}

	if (m_Instance->GetGraphics()->Initialize(is_window_mode) == false)
	{
		return false;
	}

	if (m_Instance->GetInput()->Initialize() == false)
	{
		return false;
	}

	if (m_Instance->GetSound()->Initialize() == false)
	{
		return false;
	}

	m_Instance->GetTextureManager()->Initialize();

	return true;
}

void Engine::Release()
{
	m_Instance->GetTextureManager()->Release();
	m_Instance->GetSound()->ReleaseAllSoundFiles();

	m_Instance->GetGraphics()->Release();
	m_Instance->GetInput()->Release();
	m_Instance->GetSound()->Release();

	delete m_Instance;
}

void Engine::Update()
{
	m_Instance->GetInput()->Update();
	m_Instance->GetSound()->EraseDuplicateSound();
}

bool Engine::StartDrawing(DWORD color)
{
	return m_Instance->GetGraphics()->StartDraw(color);
}

void Engine::FinishDrawing()
{
	m_Instance->GetGraphics()->FinishDraw();
}

void Engine::DrawRect(float x, float y, float width, float height, DWORD color, UCHAR alpha, float angle, float scale_x, float scale_y)
{
	m_Instance->GetGraphics()->DrawRect(x, y, width, height, color, alpha, angle, scale_x, scale_y);
}

void Engine::DrawTexture(float x, float y, const char* texture_keyword, UCHAR alpha, float angle, float scale_x, float scale_y)
{
	m_Instance->GetGraphics()->DrawTexture(x, y, texture_keyword, alpha, angle, scale_x, scale_y);
}

void Engine::DrawTextureUV(float x, float y, const char* texture_keyword, float tex_x, float tex_y, float sprite_width, float sprite_height, UCHAR alpha, float angle, float scale_x, float scale_y)
{
	m_Instance->GetGraphics()->DrawTextureUV(x, y, texture_keyword, tex_x, tex_y, sprite_width, sprite_height, alpha, angle, scale_x, scale_y);
}

void Engine::DrawFont(float x, float y, const char* text, FontSize size, FontColor color)
{
	m_Instance->GetGraphics()->DrawFont(x, y, text, size, color);
}

void Engine::SetPivotType(PivotType pivot_type)
{
	m_Instance->GetGraphics()->SetPivotType(pivot_type);
}

bool Engine::IsGamePadButtonHeld(GamePadKind button)
{
	GamePad* game_pad = m_Instance->GetInput()->GetGamePad();
	return game_pad->IsButtonHeld(button);
}

bool Engine::IsGamePadButtonPushed(GamePadKind button)
{
	GamePad* game_pad = m_Instance->GetInput()->GetGamePad();
	return game_pad->IsButtonPushed(button);
}

bool Engine::IsGamePadButtonReleased(GamePadKind button)
{
	GamePad* game_pad = m_Instance->GetInput()->GetGamePad();
	return game_pad->IsButtonReleased(button);
}

bool Engine::IsKeyboardKeyHeld(UINT key_code)
{
	Keyboard* keyboard = m_Instance->GetInput()->GetKeyboard();
	return keyboard->IsKeyHeld(key_code);
}

bool Engine::IsKeyboardKeyPushed(UINT key_code)
{
	Keyboard* keyboard = m_Instance->GetInput()->GetKeyboard();
	return keyboard->IsKeyPushed(key_code);
}

bool Engine::IsKeyboardKeyReleased(UINT key_code)
{
	Keyboard* keyboard = m_Instance->GetInput()->GetKeyboard();
	return keyboard->IsKeyReleased(key_code);
}

bool Engine::IsMouseButtonHeld(MouseButton button_type)
{
	Mouse* mouse = m_Instance->GetInput()->GetMouse();
	return mouse->IsButtonHeld(button_type);
}

bool Engine::IsMouseButtonPushed(MouseButton button_type)
{
	Mouse* mouse = m_Instance->GetInput()->GetMouse();
	return mouse->IsButtonPushed(button_type);
}

bool Engine::IsMouseButtonReleased(MouseButton button_type)
{
	Mouse* mouse = m_Instance->GetInput()->GetMouse();
	return mouse->IsButtonReleased(button_type);
}

Vec2 Engine::GetMousePos()
{
	Mouse* mouse = m_Instance->GetInput()->GetMouse();
	return mouse->GetPos();
}

void Engine::PlaySound(const char* keyword, bool is_loop)
{
	m_Instance->GetSound()->Play(keyword, is_loop);
}

void Engine::PlayDuplicateSound(const char* keyword)
{
	m_Instance->GetSound()->PlayDuplicate(keyword);
}

void Engine::StopSound(const char* keyword)
{
	m_Instance->GetSound()->Stop(keyword);
}

bool Engine::LoadSoundFile(const char* keyword, const char* file_name)
{
	return m_Instance->GetSound()->LoadSoundFile(keyword, file_name);
}

void Engine::ReleaseSoundFile(const char* keyword)
{
	m_Instance->GetSound()->ReleaseSoundFile(keyword);
}

void Engine::ReleaseAllSoundFiles()
{
	m_Instance->GetSound()->ReleaseAllSoundFiles();
}

bool Engine::LoadTexture(const char* keyword, const char* file_name)
{
	return m_Instance->GetTextureManager()->LoadTexture(keyword, file_name);
}

void Engine::ReleaseAllTextures()
{
	m_Instance->GetTextureManager()->ReleaseAllTextures();
}

void Engine::ReleaseTexture(const char* keyword)
{
	m_Instance->GetTextureManager()->ReleaseTexture(keyword);
}

Texture* Engine::GetTexture(const char* keyword)
{
	return m_Instance->GetTextureManager()->GetTexture(keyword);
}

bool Engine::CreateTexture(const char* file_name, Texture* texture_data)
{
	return m_Instance->GetGraphics()->CreateTexture(file_name, texture_data);
}
