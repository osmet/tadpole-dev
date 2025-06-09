#include "Precompiled.h"
#include "App.h"
#include "AppConstants.h"
#include "AssetLoader.h"

namespace app
{
	App::App()
		: m_appDataSource("app_data.json", AppConstants::GetDataRootPath() + "app_data.json")
		, m_gameDataSource("game_data.json", AppConstants::GetDataRootPath() + "game_data.json")
		, m_itemService(m_itemConfig.GetItems())
		, m_characterService(m_gameDataSource.GetData().GetCharacters())
		, m_inventoryService(m_gameDataSource.GetData().GetInventories())
		, m_tradeService(m_inventoryService, m_characterService, m_itemService)
	{
	}

	App::~App()
	{
		m_appDataSource.Save();
		m_gameDataSource.Save();
	}

	void App::Initialize()
	{
		core::JsonDataLoader dataLoader;
		dataLoader.Load(m_appConfig, AppConstants::GetConfigsRootPath() + "app_config.json");
		dataLoader.Load(m_assetConfig, AppConstants::GetConfigsRootPath() + "asset_config.json");
		dataLoader.Load(m_itemConfig, AppConstants::GetConfigsRootPath() + "item_config.json");

		m_appDataSource.Load();
		m_gameDataSource.Load();

		m_renderWindow.create(
			sf::VideoMode(m_appConfig.GetAppWindowWidth(), m_appConfig.GetAppWindowHeight()),
			m_appConfig.GetAppName(),
			m_appConfig.GetAppWindowStyle()
		);

		AssetLoader::LoadAssets(m_assetManager, AppConstants::GetAssetsRootPath(), m_assetConfig);

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