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

    tl::expected<void, app_domain::TradeError> 
        TradeUIViewModel::BeginTrade(const std::string& playerCharacterId, const std::string& traderCharacterId)
    {
        auto contextResult = m_tradeService.MakeContext(traderCharacterId, playerCharacterId);
        if (!contextResult)
            return tl::unexpected(contextResult.error());

        const auto& context = contextResult.value();
        const auto& playerCharacter = context.ToCharacter;
        const auto& traderCharacter = context.FromCharacter;

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

        UpdateItems();

        return {};
    }

    void TradeUIViewModel::TradeItem(bool isBuying, std::size_t itemIndex)
    {
        auto canTradeResult = CanTradeItem(isBuying, itemIndex, 1u);

        if (!canTradeResult)
        {
            EmitOnTradeError(canTradeResult.error());
            return;
        }

        auto itemOpt = isBuying
            ? GetTraderItem(itemIndex)
            : GetPlayerItem(itemIndex);

        if (!itemOpt)
            return;

        const auto& item = itemOpt.value();

        if (item.GetCount() > 1u && m_onShowTransferPanel)
        {
            m_onShowTransferPanel(item, [this, isBuying, itemIndex](std::uint32_t count)
            {
                TryTradeItem(isBuying, itemIndex, count);
            });
        }
        else
        {
            TryTradeItem(isBuying, itemIndex, 1u);
        }
    }

    void TradeUIViewModel::StackItem(std::size_t fromItemIndex, std::int32_t signedToItemIndex)
    {
        if (signedToItemIndex < 0)
            return;

        std::size_t toItemIndex = static_cast<std::size_t>(signedToItemIndex);

        auto canStackResult = CanStackItem(fromItemIndex, toItemIndex, 1u);
        if (!canStackResult)
            return;

        auto fromItemOpt = GetPlayerItem(fromItemIndex);
        if (!fromItemOpt)
            return;

        const auto& fromItem = fromItemOpt.value();

        if (fromItem.GetCount() > 1u && m_onShowTransferPanel)
        {
            m_onShowTransferPanel(fromItem, [this, fromItemIndex, toItemIndex](std::uint32_t count)
            {
                TryStackItem(fromItemIndex, toItemIndex, count);
            });
        }
        else
        {
            TryStackItem(fromItemIndex, toItemIndex, 1u);
        }
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
        return GetInventoryMoney(m_playerInventoryId);
    }

    uint32_t TradeUIViewModel::GetTraderMoney() const
    {
        return GetInventoryMoney(m_traderInventoryId);
    }

    float TradeUIViewModel::GetPlayerCurrentWeight() const
    {
        auto weight = m_inventoryService.CalculateCurrentWeight(m_playerInventoryId);
        return weight.value_or(0.0f);
    }

    float TradeUIViewModel::GetPlayerMaxWeight() const
    {
        return m_playerMaxWeight;
    }

    const std::optional<app_domain::InventoryItemDetails> TradeUIViewModel::GetPlayerItem(size_t itemIndex) const
    {
        return GetInventoryItem(m_playerInventoryId, itemIndex);
    }

    const std::optional<app_domain::InventoryItemDetails> TradeUIViewModel::GetTraderItem(size_t itemIndex) const
    {
        return GetInventoryItem(m_traderInventoryId, itemIndex);
    }

    const std::vector<app_domain::InventoryItemDetails>& TradeUIViewModel::GetPlayerItems() const
    {
        return m_playerItemsCache;
    }

    const std::vector<app_domain::InventoryItemDetails>& TradeUIViewModel::GetTraderItems() const
    {
        return m_traderItemsCache;
    }

    void TradeUIViewModel::SetItemFilterCategory(app_domain::ItemCategory itemFilterCategory)
    {
        if (m_itemFilterCategory == itemFilterCategory)
            return;

        m_itemFilterCategory = itemFilterCategory;

        UpdateItems();
    }

    void TradeUIViewModel::SetItemSortMode(app_domain::ItemSortMode itemSortMode)
    {
        if (m_itemSortMode == itemSortMode)
            return;

        m_itemSortMode = itemSortMode;

        UpdateItems();
    }

    void TradeUIViewModel::SetOnTradeBegin(OnTradeBegin callback)
    {
        m_onTradeBegin = std::move(callback);
    }

    void TradeUIViewModel::SetOnPlayerItemsUpdate(OnItemsUpdate callback)
    {
        m_onPlayerItemsUpdate = std::move(callback);
    }

    void TradeUIViewModel::SetOnTraderItemsUpdate(OnItemsUpdate callback)
    {
        m_onTraderItemsUpdate = std::move(callback);
    }

    void TradeUIViewModel::SetOnShowTransferPanel(OnShowTransferPanel callback)
    {
        m_onShowTransferPanel = std::move(callback);
    }

    void TradeUIViewModel::SetOnTradeError(OnTradeError callback)
    {
        m_onTradeError = std::move(callback);
    }


    void TradeUIViewModel::UpdateItems()
    {
        LoadInventoryItems(m_playerInventoryId, m_playerItemsCache);
        LoadInventoryItems(m_traderInventoryId, m_traderItemsCache);

        if (m_onPlayerItemsUpdate)
            m_onPlayerItemsUpdate();

        if (m_onTraderItemsUpdate)
            m_onTraderItemsUpdate();
    }

    void TradeUIViewModel::EmitOnTradeError(const app_domain::TradeError& error) const
    {
        if (m_onTradeError)
            m_onTradeError(error);
    }

    uint32_t TradeUIViewModel::GetInventoryMoney(const std::string& inventoryId) const
    {
        auto inventory = m_inventoryService.GetInventoryById(inventoryId);
        return inventory ? inventory.value().get().CurrentMoney : 0;
    }

    std::optional<app_domain::InventoryItemDetails> 
        TradeUIViewModel::GetInventoryItem(const std::string& inventoryId, size_t itemIndex) const
    {
        auto result = m_inventoryService.GetItemDetails(inventoryId, itemIndex);
        if (!result)
            return std::nullopt;

        return result.value();
    }

    void TradeUIViewModel::LoadInventoryItems(const std::string& inventoryId, 
        std::vector<app_domain::InventoryItemDetails>& out_items) const
    {
        out_items.clear();

        auto result = m_inventoryService.GetFilterSortItemDetailsList(inventoryId,
            m_itemFilterCategory, m_itemSortMode,
            [this](const app_domain::InventoryItemDetails& item) 
            { 
                return m_tradeService.IsItemTradable(item.GetItem()); 
            });

        if (result)
            out_items = std::move(result.value().Items);
    }

    tl::expected<void, app_domain::TradeError>
        TradeUIViewModel::CanTradeItem(bool isBuying, std::size_t itemIndex, std::uint32_t count) const
    {
        auto& fromCharacterId = isBuying ? m_traderCharacterId : m_playerCharacterId;
        auto& toCharacterId = isBuying ? m_playerCharacterId : m_traderCharacterId;

        auto result = m_tradeService.CanTradeItem(fromCharacterId, toCharacterId, itemIndex, count);
        if (!result)
            return tl::unexpected(result.error());

        return {};
    }

    void TradeUIViewModel::TryTradeItem(bool isBuying, std::size_t itemIndex, std::uint32_t count)
    {
        auto& fromCharacterId = isBuying ? m_traderCharacterId : m_playerCharacterId;
        auto& toCharacterId = isBuying ? m_playerCharacterId : m_traderCharacterId;

        auto result = m_tradeService.TradeItem(fromCharacterId, toCharacterId, itemIndex, count);
        if (!result)
        {
            EmitOnTradeError(result.error());
            return;
        }

        UpdateItems();
    }

    tl::expected<void, app_domain::TradeError>
        TradeUIViewModel::CanStackItem(std::size_t fromItemIndex, std::size_t toItemIndex, std::uint32_t count)
    {
        auto result = m_inventoryService.CanStackItem(m_playerInventoryId, fromItemIndex, toItemIndex, count);
        if (!result)
            return tl::unexpected(app_domain::TradeService::ToTradeError(result.error()));

        return {};
    }

    void TradeUIViewModel::TryStackItem(std::size_t fromItemIndex, std::size_t toItemIndex, std::uint32_t count)
    {
        auto result = m_inventoryService.StackItem(m_playerInventoryId, fromItemIndex, toItemIndex, count);
        if (!result)
        {
            EmitOnTradeError(app_domain::TradeService::ToTradeError(result.error()));
            return;
        }

        UpdateItems();
    }
}