#pragma once

#include "../core/ConfigManager.h"
#include "../core/AssetManager.h"
#include "AppData.h"
#include "AppDataLoader.h"
#include "GameData.h"
#include "GameDataLoader.h"

namespace app
{
	class ItemService;
	class CharacterService;
	class InventoryService;
	class TradeService;

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
		GameData m_gameData;
		GameDataLoader m_gameDataLoader;

		std::unique_ptr<ItemService> m_itemService;
		std::unique_ptr<CharacterService> m_characterService;
		std::unique_ptr<InventoryService> m_inventoryService;
		std::unique_ptr<TradeService> m_tradeService;

		sf::Sprite m_backgroundSprite;
		sf::Text m_text;
	};
}