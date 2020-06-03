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
		m_TextureList[keyword].TexutreData != nullptr)
	{
		m_TextureList[keyword].TexutreData->Release();
		m_TextureList[keyword].TexutreData = nullptr;
		m_TextureList.erase(keyword);
	}
}

void TextureManager::ReleaseAllTextures()
{
	for (auto itr : m_TextureList)
	{
		if (itr.second.TexutreData == nullptr)
		{
			continue;
		}

		itr.second.TexutreData->Release();
		itr.second.TexutreData = nullptr;
	}
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
