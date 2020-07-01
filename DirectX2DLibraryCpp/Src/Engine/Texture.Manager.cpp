#include <d3dx9.h>
#include <stdlib.h>
#include "Engine.h"
#include "Graphics.h"

void TextureManager::Initialize()
{
	m_TextureList.clear();
}

void TextureManager::Release()
{
	ReleaseAllTextures();
}

void TextureManager::ReleaseTexture(const char* keyword)
{
	if (m_TextureList.count(keyword) > 0 &&
		m_TextureList[keyword].TextureData != nullptr)
	{
		m_TextureList[keyword].TextureData->Release();
		m_TextureList[keyword].TextureData = nullptr;
		m_TextureList.erase(keyword);
	}
}

void TextureManager::ReleaseAllTextures()
{
	for (auto& texture : m_TextureList)
	{
		if (texture.second.TextureData == nullptr)
		{
			continue;
		}

		texture.second.TextureData->Release();
		texture.second.TextureData = nullptr;
	}

	m_TextureList.clear();
}

bool TextureManager::LoadTexture(const char* keyword, const char* file_name)
{
	if (file_name == nullptr ||
		keyword == nullptr)
	{
		return false;
	}

	if (m_TextureList.count(keyword) > 0)
	{
		return true;
	}

	return Engine::CreateTexture(file_name, &m_TextureList[keyword]);
}

Texture* TextureManager::GetTexture(const char* keyword)
{
	if (m_TextureList.count(keyword) == 0)
	{
		return nullptr;
	}

	return &m_TextureList[keyword];
}
