#include "Graphics.h"
#include "Engine.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define VERTEX_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

bool Graphics::Initialize(bool is_window_mode)
{
	D3DPRESENT_PARAMETERS present_param;
	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	if (CreateInterface() == false)
	{
		return false;
	}

	if (CreateDevice(&present_param, is_window_mode) == false)
	{
		return false;
	}

	if (SetUpViewPort(&present_param) == false)
	{
		return false;
	}

	if (CreateFontDevice() == false)
	{
		return false;
	}

	SetPivotType(PivotType::LeftTop);

	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャの設定
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	return true;
}

void Graphics::Release()
{
	for (auto& device : m_FontList)
	{
		if (device == nullptr)
		{
			continue;
		}
		device->Release();
		device = nullptr;
	}

	if (m_D3DDevice != nullptr)
	{
		m_D3DDevice->Release();
		m_D3DDevice = nullptr;
	}

	if (m_D3DInterface != nullptr)
	{
		m_D3DInterface->Release();
		m_D3DInterface = nullptr;
	}
}

bool Graphics::StartDraw(DWORD color)
{
	if (m_D3DDevice == nullptr)
	{
		return false;
	}

	m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 0.0f, 0);

	if (D3D_OK == m_D3DDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void Graphics::FinishDraw()
{
	if (m_D3DDevice == nullptr)
	{
		return;
	}

	m_D3DDevice->EndScene();
	m_D3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void Graphics::DrawRect(float x, float y, float width, float height, DWORD color, UCHAR alpha, float angle, float scale_x, float scale_y)
{
	Size size = Size(width, height);

	color += alpha << 24;

	CustomVertex v[4] =
	{
		{ 0.0f, 0.0f, 0.0f, 1.0f, color, 0.0f, 0.0f },
		{ size.Width, 0.0f, 0.0f, 1.0f, color, 1.0f, 0.0f },
		{ size.Width, size.Height, 0.0f, 1.0f, color, 1.0f, 1.0f },
		{ 0.0f, size.Height, 0.0f, 1.0f, color, 0.0f, 1.0f },
	};

	Vec2 offset = CalculatePivotOffset(&size);

	for (int i = 0; i < 4; i++)
	{
		v[i].X += offset.X;
		v[i].Y += offset.Y;
	}

	TransformRect(v, x, y, angle, scale_x, scale_y);

	// 頂点構造の指定
	m_D3DDevice->SetFVF(VERTEX_FVF);

	m_D3DDevice->SetTexture(0, nullptr);

	m_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CustomVertex));
}

void Graphics::DrawTextureUV(float x, float y, const char* texture_keyword, float tex_x, float tex_y, float sprite_width, float sprite_height, UCHAR alpha, float angle, float scale_x, float scale_y)
{
	Texture* texture_data = Engine::GetTexture(texture_keyword);
	if (texture_data == nullptr)
	{
		return;
	}

	/*
		tex_x => テクスチャ座標X
		tex_y => テクスチャ座標Y
		sprite_width => 矩形サイズ(横)
		sprite_height => 矩形サイズ(縦)
	
	*/

	// テクスチャ座標 => UV変換
	float u_left = tex_x / texture_data->Width;
	float u_right = (tex_x + sprite_width) / texture_data->Width;
	float v_top = tex_y / texture_data->Height;
	float v_bottom = (tex_y + sprite_height) / texture_data->Height;

	Size size = Size(sprite_width, sprite_height);

	DWORD color = D3DCOLOR_RGBA(0xff, 0xff, 0xff, alpha);
	CustomVertex v[4] =
	{
		{ 0.0f, 0.0f, 0.0f, 1.0f, color, u_left, v_top },
		{ size.Width, 0.0f, 0.0f, 1.0f, color, u_right, v_top },
		{ size.Width, size.Height, 0.0f, 1.0f, color, u_right, v_bottom },
		{ 0.0f, size.Height, 0.0f, 1.0f, color, u_left, v_bottom },
	};

	Vec2 offset = CalculatePivotOffset(&size);

	for (int i = 0; i < 4; i++)
	{
		v[i].X += offset.X;
		v[i].Y += offset.Y;
	}

	TransformRect(v, x, y, angle, scale_x, scale_y);

	// 頂点構造の指定
	m_D3DDevice->SetFVF(VERTEX_FVF);

	m_D3DDevice->SetTexture(0, texture_data->TextureData);

	m_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CustomVertex));
}

void Graphics::DrawTexture(float x, float y, const char* texture_keyword, UCHAR alpha, float angle, float scale_x, float scale_y)
{
	Texture* texture_data = Engine::GetTexture(texture_keyword);
	if (texture_data == nullptr)
	{
		return;
	}

	Size size = Size((float)texture_data->Width, (float)texture_data->Height);

	DWORD color = D3DCOLOR_RGBA(0xff, 0xff, 0xff, alpha);
	CustomVertex v[4] =
	{
		{ 0.0f, 0.0f, 0.0f, 1.0f, color, 0.0f, 0.0f },
		{ size.Width, 0.0f, 0.0f, 1.0f, color, 1.0f, 0.0f },
		{ size.Width, size.Height, 0.0f, 1.0f, color, 1.0f, 1.0f },
		{ 0.0f, size.Height, 0.0f, 1.0f, color, 0.0f, 1.0f },
	};

	Vec2 offset = CalculatePivotOffset(&size);

	for (int i = 0; i < 4; i++)
	{
		v[i].X += offset.X;
		v[i].Y += offset.Y;
	}

	TransformRect(v, x, y, angle, scale_x, scale_y);

	// 頂点構造の指定
	m_D3DDevice->SetFVF(VERTEX_FVF);

	m_D3DDevice->SetTexture(0, texture_data->TextureData);

	m_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CustomVertex));
}

void Graphics::DrawFont(float x, float y, const char* text, FontSize font_type, FontColor color)
{
	if (m_FontList[font_type] == nullptr)
	{
		return;
	}

	RECT rect =
	{
		(long)x,
		(long)y,
		(long)x + 800,
		(long)y + 300,
	};

	int r, g, b;
	r = g = b = 255;

	if (color == FontColor::Black)
	{
		r = g = b = 0;
	}
	else if (color == FontColor::Red)
	{
		r = 255;
		g = b = 0;
	}

	m_FontList[font_type]->DrawText(
		nullptr,
		text,
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_XRGB(r, g, b)
	);
}

void Graphics::SetPivotType(PivotType pivot_type)
{
	m_CurrentPivot = pivot_type;
}

bool Graphics::CreateTexture(const char* file_name, Texture* texture_data)
{
	D3DXIMAGE_INFO info;

	// 2の累乗じゃないケースを想定して元のサイズを取得してD3DXCreateTextureFromFileExで使う
	D3DXGetImageInfoFromFile(file_name, &info);

	if (FAILED(D3DXCreateTextureFromFileEx(
		m_D3DDevice,
		file_name,
		info.Width,
		info.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0000ff00,
		nullptr,
		nullptr,
		&texture_data->TextureData)))
	{
		return false;
	}
	else
	{
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;

		if (FAILED(texture_data->TextureData->GetLevelDesc(0, &desc)))
		{
			texture_data->TextureData->Release();
			return false;
		}
		texture_data->Width = desc.Width;
		texture_data->Height = desc.Height;
	}

	return true;
}

bool Graphics::CreateInterface()
{
	// インターフェース作成
	m_D3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_D3DInterface == NULL)
	{
		// 作成失敗
		return false;
	}

	// 作成成功
	return true;
}

bool Graphics::CreateDevice(D3DPRESENT_PARAMETERS* present_param, bool is_window_mode)
{
	present_param->BackBufferCount = 1;
	present_param->BackBufferFormat = D3DFMT_X8R8G8B8;
	present_param->SwapEffect = D3DSWAPEFFECT_DISCARD;

	if (is_window_mode == true)
	{
		present_param->Windowed = is_window_mode;
	}
	else
	{
		HWND window_handle = FindWindow(WINDOW_CLASS_NAME, nullptr);

		RECT rect;
		GetClientRect(window_handle, &rect);

		present_param->Windowed = is_window_mode;
		present_param->BackBufferHeight = rect.bottom - rect.top;
		present_param->BackBufferWidth = rect.right - rect.left;
	}

	// DirectDeviceの作成
	if (FAILED(m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		present_param,
		&m_D3DDevice)))
	{
		return false;
	}

	return true;
}

bool Graphics::SetUpViewPort(D3DPRESENT_PARAMETERS* present_param)
{
	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;
	// ビューポートの幅
	view_port.Width = present_param->BackBufferWidth;
	// ビューポートの高さ
	view_port.Height = present_param->BackBufferHeight;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(m_D3DDevice->SetViewport(&view_port)))
	{
		return false;
	}

	return true;
}

bool Graphics::CreateFontDevice()
{
	int size_list[] =
	{
		SmallFontSize,
		RegularFontSize,
		LargeFontSize
	};

	for (int i = 0; i < FontSize::FontSizeMax; i++)
	{
		if (FAILED(D3DXCreateFont(m_D3DDevice,
			size_list[i],
			size_list[i] / 2,
			FW_REGULAR,
			0,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_SCRIPT,
			TEXT("ＭＳ　Ｐゴシック"),
			&m_FontList[i])))
		{
			return false;
		}
	}

	return true;
}

void Graphics::TransformRect(CustomVertex* vertices, float pos_x, float pos_y, float angle, float scale_x, float scale_y)
{
	for (int i = 0; i < 4; i++)
	{
		vertices[i].X *= scale_x;
		vertices[i].Y *= scale_y;
	}

	float sinY = sinf(D3DXToRadian(angle));
	float cosX = cosf(D3DXToRadian(angle));
	float x = 0.0f;
	float y = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		x = (vertices[i].X * cosX) + (vertices[i].Y * -sinY) + pos_x;
		y = (vertices[i].X * sinY) + (vertices[i].Y * cosX) + pos_y;

		vertices[i].X = x;
		vertices[i].Y = y;
	}
}

Vec2 Graphics::CalculatePivotOffset(Size* size)
{
	Vec2 offset[] =
	{
		{ Vec2(0.0f, 0.0f) },
		{ Vec2(-size->Width / 2.0f, 0.0f) },
		{ Vec2(-size->Width, 0.0f) },
	
		{ Vec2(0.0f, -size->Height / 2.0f) },
		{ Vec2(-size->Width / 2.0f, -size->Height / 2.0f) },
		{ Vec2(-size->Width, -size->Height / 2.0f) },

		{ Vec2(0.0f, -size->Height) },
		{ Vec2(-size->Width / 2.0f, -size->Height) },
		{ Vec2(-size->Width, -size->Height) },
	};

	return offset[m_CurrentPivot];
}
