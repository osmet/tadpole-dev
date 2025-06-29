#include "Precompiled.h"
#include "AssetManager.h"

namespace core
{
	void AssetManager::SetRootPath(const std::string& filePath)
	{
		m_rootPath = filePath;
	}

	void AssetManager::UnloadAsset(const std::string& id)
	{
		auto it = m_assets.find(id);

		if (it != m_assets.end())
		{
			it->second->Unload();

			m_assets.erase(it);
		}
	}

	void AssetManager::LoadTexture(const std::string& id, const std::string& filePath)
	{
		LoadAsset<TextureAsset>(id, filePath);
	}

	sf::Texture& AssetManager::GetTexture(const std::string& id)
	{
		auto it = m_assets.find(id);

		if (it != m_assets.end())
		{
			return static_cast<TextureAsset*>(it->second.get())->GetTexture();
		}

		return s_emptyTexture;
	}

	void AssetManager::LoadFont(const std::string& id, const std::string& filePath)
	{
		LoadAsset<FontAsset>(id, filePath);
	}

	sf::Font& AssetManager::GetFont(const std::string& id)
	{
		auto it = m_assets.find(id);

		if (it != m_assets.end())
		{
			return static_cast<FontAsset*>(it->second.get())->GetFont();
		}

		return s_emptyFont;
	}

	sf::Texture AssetManager::CreateEmptyTexture()
	{
		sf::Texture texture;
		const sf::Color pinkColor(255, 0, 255);

		sf::Image image;
		image.create(32, 32, pinkColor);

		texture.loadFromImage(image);

		return texture;
	}

	sf::Texture AssetManager::s_emptyTexture = CreateEmptyTexture();
	sf::Font AssetManager::s_emptyFont;

	AssetManager::Asset::~Asset() = default;

	bool AssetManager::TextureAsset::Load(const std::string & filePath)
	{
		bool loaded = m_texture.loadFromFile(filePath);

		if (loaded)
		{
			m_texture.setSmooth(true);

			return true;
		}

		return false;
	}

	void AssetManager::TextureAsset::Unload()
	{
	}

	sf::Texture& AssetManager::TextureAsset::GetTexture()
	{
		return m_texture;
	}

	bool AssetManager::FontAsset::Load(const std::string & filePath)
	{
		return m_font.loadFromFile(filePath);
	}

	void AssetManager::FontAsset::Unload()
	{
	}

	sf::Font& AssetManager::FontAsset::GetFont()
	{
		return m_font;
	}
}