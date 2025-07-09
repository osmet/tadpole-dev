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
        m_context.ItemFilterCategory.Subscribe([this](const auto& value)
        {
            UpdateItems();
        });

        m_context.ItemSortMode.Subscribe([this](const auto& value)
        {
            UpdateItems();
        });
    }

    TradeUIViewModel::Context& TradeUIViewModel::GetContext()
    {
        return m_context;
    }

    lang::Expected<void, app_domain::TradeError> 
        TradeUIViewModel::BeginTrade(const std::string& playerCharacterId, const std::string& traderCharacterId)
    {
        auto tradeContextResult = m_tradeService.MakeContext(traderCharacterId, playerCharacterId);
        if (!tradeContextResult.has_value())
            return lang::Unexpected(tradeContextResult.error());

        const auto& tradeContext = tradeContextResult.value();
        const auto& playerCharacter = tradeContext.ToCharacter;
        const auto& traderCharacter = tradeContext.FromCharacter;

        m_playerCharacterId = playerCharacter.Id;
        m_traderCharacterId = traderCharacter.Id;

        m_playerInventoryId = playerCharacter.InventoryId;
        m_traderInventoryId = traderCharacter.InventoryId;

        m_context.PlayerCharacterName.SetValue(playerCharacter.Name);
        m_context.TraderCharacterName.SetValue(traderCharacter.Name);

        m_context.PlayerPortraitTextureId.SetValue(playerCharacter.PortraitTextureId);
        m_context.TraderPortraitTextureId.SetValue(traderCharacter.PortraitTextureId);

        m_context.PlayerMaxWeight.SetValue(playerCharacter.MaxWeight);

        UpdateItems();

        return {};
    }

    void TradeUIViewModel::TradeItem(bool isBuying, std::size_t itemIndex)
    {
        auto canTradeResult = CanTradeItem(isBuying, itemIndex, 1u);
        if (!canTradeResult.has_value())
        {
            EmitOnTradeError(canTradeResult.error());
            return;
        }

        auto itemOpt = isBuying ? GetTraderItem(itemIndex) : GetPlayerItem(itemIndex);
        if (!itemOpt.has_value())
            return;

        ShowTransferPanelOrApply(itemOpt.value(), [this, isBuying, itemIndex](std::uint32_t count)
        {
            TryTradeItem(isBuying, itemIndex, count);
        });
    }

    void TradeUIViewModel::StackItem(std::size_t fromItemIndex, std::int32_t signedToItemIndex)
    {
        if (signedToItemIndex < 0)
            return;

        std::size_t toItemIndex = static_cast<std::size_t>(signedToItemIndex);

        auto canStackResult = CanStackItem(fromItemIndex, toItemIndex, 1u);
        if (!canStackResult.has_value())
            return;

        auto fromItemOpt = GetPlayerItem(fromItemIndex);
        if (!fromItemOpt.has_value())
            return;

        ShowTransferPanelOrApply(fromItemOpt.value(), [this, fromItemIndex, toItemIndex](std::uint32_t count)
        {
            TryStackItem(fromItemIndex, toItemIndex, count);
        });
    }

    lang::Optional<app_domain::InventoryItemDetails> TradeUIViewModel::GetPlayerItem(size_t itemIndex) const
    {
        return GetInventoryItem(m_playerInventoryId, itemIndex);
    }

    lang::Optional<app_domain::InventoryItemDetails> TradeUIViewModel::GetTraderItem(size_t itemIndex) const
    {
        return GetInventoryItem(m_traderInventoryId, itemIndex);
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
        m_context.PlayerItems.SetValue(std::move(LoadInventoryItems(m_playerInventoryId)));
        m_context.TraderItems.SetValue(std::move(LoadInventoryItems(m_traderInventoryId)));

        m_context.PlayerCurrentMoney.SetValue(GetInventoryCurrentMoney(m_playerInventoryId));
        m_context.TraderCurrentMoney.SetValue(GetInventoryCurrentMoney(m_traderInventoryId));

        m_context.PlayerCurrentWeight.SetValue(GetInventoryCurrentWeight(m_playerInventoryId));
    }

    uint32_t TradeUIViewModel::GetInventoryCurrentMoney(const std::string& inventoryId) const
    {
        auto inventoryResult = m_inventoryService.GetInventoryById(inventoryId);
        if (!inventoryResult.has_value())
            return 0u;

        return inventoryResult.value().get().CurrentMoney;
    }

    float TradeUIViewModel::GetInventoryCurrentWeight(const std::string& inventoryId) const
    {
        auto result = m_inventoryService.CalculateCurrentWeight(inventoryId);
        if (!result.has_value())
            return 0.f;

        return result.value();
    }

    lang::Optional<app_domain::InventoryItemDetails> 
        TradeUIViewModel::GetInventoryItem(const std::string& inventoryId, size_t itemIndex) const
    {
        auto result = m_inventoryService.GetItemDetails(inventoryId, itemIndex);
        if (!result.has_value())
            return std::nullopt;

        return result.value();
    }

    std::vector<app_domain::InventoryItemDetails>  
        TradeUIViewModel::LoadInventoryItems(const std::string& inventoryId) const
    {
        auto result = m_inventoryService.GetFilterSortItemDetailsList(inventoryId,
            m_context.ItemFilterCategory.GetValue(), m_context.ItemSortMode.GetValue(),
            [this](const app_domain::InventoryItemDetails& item) 
            { 
                return m_tradeService.IsItemTradable(item.GetItem()); 
            });
        if (!result.has_value())
            return {};

        return result.value().Items;
    }

    void TradeUIViewModel::ShowTransferPanelOrApply(const app_domain::InventoryItemDetails& item,
        std::function<void(std::uint32_t)> onConfirm)
    {
        if (item.GetCount() > 1u && m_onShowTransferPanel)
            m_onShowTransferPanel(item, std::move(onConfirm));
        else
            onConfirm(1u);
    }

    void TradeUIViewModel::EmitOnTradeError(const app_domain::TradeError& error) const
    {
        if (m_onTradeError)
            m_onTradeError(error);
    }

    lang::Expected<void, app_domain::TradeError>
        TradeUIViewModel::CanTradeItem(bool isBuying, std::size_t itemIndex, std::uint32_t count) const
    {
        auto& fromCharacterId = isBuying ? m_traderCharacterId : m_playerCharacterId;
        auto& toCharacterId = isBuying ? m_playerCharacterId : m_traderCharacterId;

        auto result = m_tradeService.CanTradeItem(fromCharacterId, toCharacterId, itemIndex, count);
        if (!result.has_value())
            return lang::Unexpected(result.error());

        return {};
    }

    void TradeUIViewModel::TryTradeItem(bool isBuying, std::size_t itemIndex, std::uint32_t count)
    {
        auto& fromCharacterId = isBuying ? m_traderCharacterId : m_playerCharacterId;
        auto& toCharacterId = isBuying ? m_playerCharacterId : m_traderCharacterId;

        auto result = m_tradeService.TradeItem(fromCharacterId, toCharacterId, itemIndex, count);
        if (!result.has_value())
        {
            EmitOnTradeError(result.error());
            return;
        }

        UpdateItems();
    }

    lang::Expected<void, app_domain::TradeError>
        TradeUIViewModel::CanStackItem(std::size_t fromItemIndex, std::size_t toItemIndex, std::uint32_t count)
    {
        auto result = m_inventoryService.CanStackItem(m_playerInventoryId, fromItemIndex, toItemIndex, count);
        if (!result.has_value())
            return lang::Unexpected(app_domain::TradeService::ToTradeError(result.error()));

        return {};
    }

    void TradeUIViewModel::TryStackItem(std::size_t fromItemIndex, std::size_t toItemIndex, std::uint32_t count)
    {
        auto result = m_inventoryService.StackItem(m_playerInventoryId, fromItemIndex, toItemIndex, count);
        if (!result.has_value())
        {
            EmitOnTradeError(app_domain::TradeService::ToTradeError(result.error()));
            return;
        }

        UpdateItems();
    }
}