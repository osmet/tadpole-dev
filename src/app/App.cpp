#include "Precompiled.h"
#include "App.h"
#include "AppConstants.h"
#include "AppConfig.h"
#include "AssetConfig.h"
#include "AssetLoader.h"
#include "AppDataLoader.h"
#include "ItemConfig.h"
#include "ItemService.h"
#include "CharacterService.h"
#include "InventoryService.h"
#include "TradeService.h"

namespace app
{
	App::App()
		: m_configManager(AppConstants::GetConfigsRootPath())
		, m_appDataLoader(m_appData, AppConstants::GetDataRootPath(), "app_data.json")
		, m_gameDataLoader(m_gameData, AppConstants::GetDataRootPath(), "game_data.json")
	{
	}

	App::~App()
	{
		m_appDataLoader.Save();
		m_gameDataLoader.Save();
	}

	void App::Initialize()
	{
		m_configManager.LoadConfig<AppConfig>("app_config.json");
		m_configManager.LoadConfig<AssetConfig>("asset_config.json");
		m_configManager.LoadConfig<ItemConfig>("item_config.json");

		m_appDataLoader.Load();
		m_gameDataLoader.Load();

		const auto& appConfig = m_configManager.Get<AppConfig>();
		const auto& assetConfig = m_configManager.Get<AssetConfig>();
		const auto& itemConfig = m_configManager.Get<ItemConfig>();

		m_renderWindow.create(
			sf::VideoMode(appConfig.GetAppWindowWidth(), appConfig.GetAppWindowHeight()),
			appConfig.GetAppName(),
			appConfig.GetAppWindowStyle()
		);

		AssetLoader::LoadAssets(m_assetManager, AppConstants::GetAssetsRootPath(), assetConfig);

		m_itemService = std::make_unique<ItemService>(itemConfig.GetItems());
		m_characterService = std::make_unique<CharacterService>(m_gameData.Characters);
		m_inventoryService = std::make_unique<InventoryService>(m_gameData.Inventories);
		m_tradeService = std::make_unique<TradeService>(*m_inventoryService , *m_characterService, *m_itemService);

		m_backgroundSprite.setTexture(m_assetManager.GetTexture("background"));
	}

	void App::Run()
	{
		while (m_renderWindow.isOpen())
		{
			HandleEvents();
			Render();
		}
	}

	sf::Vector2f App::GetRenderWindowSize() const
	{
		sf::Vector2u renderWindowSize = m_renderWindow.getSize();

		return sf::Vector2f(static_cast<float>(renderWindowSize.x), static_cast<float>(renderWindowSize.y));
	}

	core::ConfigManager& App::GetConfigManager()
	{
		return m_configManager;
	}

	core::AssetManager& App::GetAssetManager()
	{
		return m_assetManager;
	}

	void App::HandleEvents()
	{
		sf::Event inputEvent;
		while (m_renderWindow.pollEvent(inputEvent))
		{
			if (inputEvent.type == sf::Event::Closed)
			{
				m_renderWindow.close();
				return;
			}
		}
	}

	void App::Render()
	{
		m_renderWindow.clear();

		m_renderWindow.draw(m_backgroundSprite);

		m_renderWindow.display();
	}
}