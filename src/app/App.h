#pragma once

#include "../core/AssetManager.h"
#include "../core/DataSource.h"
#include "../core/JsonDataLoader.h"
#include "AppConfig.h"
#include "AssetConfig.h"
#include "ItemConfig.h"
#include "AppData.h"
#include "GameData.h"
#include "../app_domain/ItemService.h"
#include "../app_domain/CharacterService.h"
#include "../app_domain/InventoryService.h"
#include "../app_domain/TradeService.h"

namespace app
{
    class App final
    {
    public:
        App();
        ~App();

        void Initialize();
        void Run();

    private:
        void HandleEvents();
        void Render();

        sf::RenderWindow m_renderWindow;

        core::AssetManager m_assetManager;

        AppConfig m_appConfig;
        AssetConfig m_assetConfig;
        ItemConfig m_itemConfig;
        core::DataSource<AppData, core::JsonDataLoader> m_appDataSource;
        core::DataSource<GameData, core::JsonDataLoader> m_gameDataSource;

        app_domain::ItemService m_itemService;
        app_domain::CharacterService m_characterService;
        app_domain::InventoryService m_inventoryService;
        app_domain::TradeService m_tradeService;

        sf::Sprite m_backgroundSprite;
    };
}