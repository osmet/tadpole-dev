#include "Precompiled.h"
#include "TradeUIViewModel.h"

namespace app
{
    TradeUIViewModel::TradeUIViewModel(AppContext& appContext)
        : m_itemService(appContext.GetItemService())
        , m_characterService(appContext.GetCharacterService())
        , m_inventoryService(appContext.GetInventoryService())
        , m_tradeService(appContext.GetTradeService())
    {
    }

    tl::expected<void, app_domain::TradeError> TradeUIViewModel::BeginTrade(
        const std::string& playerCharacterId,
        const std::string& traderCharacterId)
    {
        auto contextResult = m_tradeService.MakeContext(playerCharacterId, traderCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        const auto& context = contextResult.value();
        const auto& playerCharacter = context.BuyerCharacter;
        const auto& traderCharacter = context.SellerCharacter;

        m_playerCharacterId = playerCharacter.Id;
        m_traderCharacterId = traderCharacter.Id;

        m_playerInventoryId = playerCharacter.InventoryId;
        m_traderInventoryId = traderCharacter.InventoryId;

        m_playerCharacterName = playerCharacter.Name;
        m_traderCharacterName = traderCharacter.Name;

        m_playerPortrait = playerCharacter.PortraitTextureId;
        m_traderPortrait = traderCharacter.PortraitTextureId;

        m_playerMaxWeight = playerCharacter.MaxWeight;

        if (m_onTradeBegin)
            m_onTradeBegin();

        InvokeOnInventoryUpdates();

        return {};
    }

    void TradeUIViewModel::SetOnTradeBegin(OnTradeBegin callback)
    {
        m_onTradeBegin = std::move(callback);
    }

    void TradeUIViewModel::SetOnPlayerInventoryUpdate(OnPlayerInventoryUpdate callback)
    {
        m_onPlayerInventoryUpdate = std::move(callback);
    }

    void TradeUIViewModel::SetOnTraderInventoryUpdate(OnTraderInventoryUpdate callback)
    {
        m_onTraderInventoryUpdate = std::move(callback);
    }

    tl::expected<void, app_domain::TradeError> TradeUIViewModel::CanBuyItem(std::size_t itemIndex) const
    {
        return m_tradeService.CanTradeItem(*m_playerCharacterId, *m_traderCharacterId, itemIndex);
    }

    tl::expected<void, app_domain::TradeError> TradeUIViewModel::CanSellItem(std::size_t itemIndex) const
    {
        return m_tradeService.CanTradeItem(*m_traderCharacterId, *m_playerCharacterId, itemIndex);
    }

    tl::expected<void, app_domain::TradeError> TradeUIViewModel::BuyItem(std::size_t itemIndex)
    {
        auto result = m_tradeService.TradeItem(*m_playerCharacterId, *m_traderCharacterId, itemIndex);
        if (result)
            InvokeOnInventoryUpdates();

        return result;
    }

    tl::expected<void, app_domain::TradeError> TradeUIViewModel::SellItem(std::size_t itemIndex)
    {
        auto result = m_tradeService.TradeItem(*m_traderCharacterId, *m_playerCharacterId, itemIndex);
        if (result)
            InvokeOnInventoryUpdates();

        return result;
    }

    const std::string& TradeUIViewModel::GetPlayerName() const
    {
        return m_playerCharacterName;
    }

    const std::string& TradeUIViewModel::GetTraderName() const
    {
        return m_traderCharacterName;
    }

    const std::string& TradeUIViewModel::GetPlayerPortraitTextureId() const
    {
        return m_playerPortrait;
    }

    const std::string& TradeUIViewModel::GetTraderPortraitTextureId() const
    {
        return m_traderPortrait;
    }

    uint32_t TradeUIViewModel::GetPlayerMoney() const
    {
        return GetInventoryMoney(*m_playerInventoryId);
    }

    uint32_t TradeUIViewModel::GetTraderMoney() const
    {
        return GetInventoryMoney(*m_traderInventoryId);
    }

    float TradeUIViewModel::GetPlayerCurrentWeight() const
    {
        auto weight = m_inventoryService.CalculateCurrentWeight(*m_playerInventoryId);
        return weight.value_or(0.0f);
    }

    float TradeUIViewModel::GetPlayerMaxWeight() const
    {
        return m_playerMaxWeight;
    }

    const app_domain::Item* TradeUIViewModel::GetPlayerItemByIndex(size_t index) const
    {
        return GetInventoryItemByIndex(*m_playerInventoryId, index);
    }

    const app_domain::Item* TradeUIViewModel::GetTraderItemByIndex(size_t index) const
    {
        return GetInventoryItemByIndex(*m_traderInventoryId, index);
    }

    std::vector<const app_domain::Item*> TradeUIViewModel::GetPlayerItems() const
    {
        return GetInventoryItems(*m_playerInventoryId);
    }

    std::vector<const app_domain::Item*> TradeUIViewModel::GetTraderItems() const
    {
        return GetInventoryItems(*m_traderInventoryId);
    }

    void TradeUIViewModel::InvokeOnInventoryUpdates()
    {
        if (m_onPlayerInventoryUpdate)
            m_onPlayerInventoryUpdate();

        if (m_onTraderInventoryUpdate)
            m_onTraderInventoryUpdate();
    }

    uint32_t TradeUIViewModel::GetInventoryMoney(const std::string& inventoryId) const
    {
        auto inventory = m_inventoryService.GetInventoryById(inventoryId);
        return inventory ? inventory.value().get().CurrentMoney : 0;
    }

    const app_domain::Item* TradeUIViewModel::GetInventoryItemByIndex(const std::string& inventoryId, size_t index) const
    {
        auto inventoryItemResult = m_inventoryService.GetItemByIndex(inventoryId, index);
        if (!inventoryItemResult)
            return nullptr;

        const auto& inventoryItem = inventoryItemResult.value().get();

        auto itemResult = m_itemService.GetItemById(inventoryItem.ItemId);
        if (!itemResult)
            return nullptr;

        return &itemResult.value().get();
    }

    std::vector<const app_domain::Item*> TradeUIViewModel::GetInventoryItems(const std::string& inventoryId) const
    {
        std::vector<const app_domain::Item*> items;
        auto inventoryResult = m_inventoryService.GetInventoryById(inventoryId);
        if (!inventoryResult)
            return items;

        for (const auto& inventoryItem : inventoryResult.value().get().Items)
        {
            auto itemResult = m_itemService.GetItemById(inventoryItem.ItemId);
            if (itemResult)
                items.push_back(&itemResult.value().get());
        }

        return items;
    }
}