#include "Precompiled.h"
#include "AppContext.h"
#include "AppConstants.h"
#include "AssetLoader.h"

namespace app
{
    AppContext::AppContext(sf::RenderWindow& renderWindow)
        : m_renderWindow(renderWindow)
        , m_appDataSource(AppConstants::GetDataRootPath() + "AppData.json", AppConstants::GetDataDefaultsRootPath() + "AppData.json")
        , m_gameDataSource(AppConstants::GetDataRootPath() + "GameData.json", AppConstants::GetDataDefaultsRootPath() + "GameData.json")
        , m_itemService(m_itemConfig.GetItems())
        , m_characterService(m_gameDataSource.GetData().GetCharacters())
        , m_inventoryService(m_gameDataSource.GetData().GetInventories(), m_itemService)
        , m_tradeService(m_itemService, m_characterService, m_inventoryService)
    {
    }

    AppContext::~AppContext()
    {
    }

    void AppContext::Initialize()
    {
        core::JsonDataLoader dataLoader;
        dataLoader.Load(m_appConfig, AppConstants::GetConfigsRootPath() + "AppConfig.json");
        dataLoader.Load(m_assetConfig, AppConstants::GetConfigsRootPath() + "AssetConfig.json");
        dataLoader.Load(m_itemConfig, AppConstants::GetConfigsRootPath() + "ItemConfig.json");

        AssetLoader::LoadAssets(m_assetManager, AppConstants::GetAssetsRootPath(), m_assetConfig);
    }

    sf::RenderWindow& AppContext::GetRenderWindow()
    {
        return m_renderWindow;
    }

    sf::Vector2f AppContext::GetRenderWindowSize() const
    {
        sf::Vector2u renderWindowSize = m_renderWindow.getSize();

        return sf::Vector2f(static_cast<float>(renderWindowSize.x), static_cast<float>(renderWindowSize.y));
    }

    core::AssetManager& AppContext::GetAssetManager() 
    { 
        return m_assetManager; 
    }

    const AppConfig& AppContext::GetAppConfig() const 
    { 
        return m_appConfig; 
    }

    const AssetConfig& AppContext::GetAssetConfig() const 
    { 
        return m_assetConfig; 
    }

    const ItemConfig& AppContext::GetItemConfig() const 
    { 
        return m_itemConfig; 
    }

    core::DataSource<AppData, core::JsonDataLoader>& AppContext::GetAppDataSource() 
    { 
        return m_appDataSource; 
    }

    core::DataSource<GameData, core::JsonDataLoader>& AppContext::GetGameDataSource() 
    { 
        return m_gameDataSource; 
    }

    app_domain::ItemService& AppContext::GetItemService()
    { 
        return m_itemService; 
    }

    app_domain::CharacterService& AppContext::GetCharacterService() 
    { 
        return m_characterService; 
    }

    app_domain::InventoryService& AppContext::GetInventoryService() 
    { 
        return m_inventoryService; 
    }

    app_domain::TradeService& AppContext::GetTradeService() 
    { 
        return m_tradeService; 
    }
}
