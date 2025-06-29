#pragma once

namespace core
{
	class AssetManager final
	{
	public:
		void SetRootPath(const std::string& filePath);

		void UnloadAsset(const std::string& id);

		void LoadTexture(const std::string& id, const std::string& filePath);
		sf::Texture& GetTexture(const std::string& id);

		void LoadFont(const std::string& id, const std::string& filePath);
		sf::Font& GetFont(const std::string& id);

	private:
		class Asset;

		static sf::Texture CreateEmptyTexture();

		static sf::Texture s_emptyTexture;
		static sf::Font s_emptyFont;

		template<class TAssetDescriptor>
		void LoadAsset(const std::string& id, const std::string& filePath)
		{
			assert(!id.empty() && "Id must not be empty.");
			assert(!filePath.empty() && "File path must not be empty.");

			auto assetDescriptor = std::make_unique<TAssetDescriptor>();

			std::string fullPath;
			fullPath.reserve(m_rootPath.size() + filePath.size());
			fullPath.append(m_rootPath);
			fullPath.append(filePath);

			if (assetDescriptor->Load(fullPath))
			{
				m_assets[id] = std::move(assetDescriptor);

				return;
			}

			assert(false && "Failed to load the asset.");
		}

		std::string m_rootPath;
		std::unordered_map<std::string, std::unique_ptr<Asset>> m_assets;

	private:
		class Asset
		{
		public:
			virtual ~Asset();

			virtual bool Load(const std::string& filePath) = 0;
			virtual void Unload() = 0;
		};

		class TextureAsset : public Asset
		{
		public:
			bool Load(const std::string& filePath) override;
			void Unload() override;

			sf::Texture& GetTexture();

		private:
			sf::Texture m_texture;
		};

		class FontAsset : public Asset
		{
		public:
			bool Load(const std::string& filePath) override;
			void Unload() override;

			sf::Font& GetFont();

		private:
			sf::Font m_font;
		};
	};
}