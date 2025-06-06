#pragma once

#include "../core/ConfigManager.h"
#include "../core/AssetManager.h"
#include "AppData.h"
#include "AppDataLoader.h"

namespace app
{
	class App final
	{
	public:
		App();
		~App();

		void Initialize();
		void Run();

		sf::Vector2f GetRenderWindowSize() const;

		core::ConfigManager& GetConfigManager();
		core::AssetManager& GetAssetManager();

	private:
		void HandleEvents();
		void Render();

		sf::RenderWindow m_renderWindow;

		core::ConfigManager m_configManager;
		core::AssetManager m_assetManager;

		AppData m_appData;
		AppDataLoader m_appDataLoader;

		sf::Sprite m_backgroundSprite;
		sf::Text m_text;
	};
}