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
    class AppContext
    {
    public: 
        AppContext(sf::RenderWindow& renderWindow);
        ~AppContext();

        void Initialize();

        sf::RenderWindow& GetRenderWindow();
        sf::Vector2f GetRenderWindowSize() const;

        core::AssetManager& GetAssetManager();

        const AppConfig& GetAppConfig() const;
        const AssetConfig& GetAssetConfig() const;
        const ItemConfig& GetItemConfig() const;

        core::DataSource<AppData, core::JsonDataLoader>& GetAppDataSource();
        core::DataSource<GameData, core::JsonDataLoader>& GetGameDataSource();

        app_domain::ItemService& GetItemService();
        app_domain::CharacterService& GetCharacterService();
        app_domain::InventoryService& GetInventoryService();
        app_domain::TradeService& GetTradeService();

    private:
        sf::RenderWindow& m_renderWindow;

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
    };
}