#include "Precompiled.h"
#include "ItemGridPanel.h"
#include "../core/AssetManager.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"
#include "../app_domain/Item.h"
#include "../app_domain/ItemRarity.h"
#include "../app_domain/InventoryItemDetails.h"
#include "ItemConfig.h"

namespace app
{
    ItemGridPanel::ItemGridPanel(core::AssetManager& assetManager,
        size_t columnCount, size_t rowCount, float cellSize, float spacing)
        : m_assetManager(assetManager)
    {
        size_t cellCount = columnCount * rowCount;

        SetColumnCount(columnCount);
        SetCellSize(cellSize);
        SetSpacing(spacing);

        for (size_t index = 0; index < cellCount; ++index)
        {
            auto* itemSlot = CreateWidget<ItemSlot>(assetManager, cellSize);

            itemSlot->SetOnClick([this, itemSlot]() 
            { 
                if (m_onItemSlotClick && itemSlot && itemSlot->HasItem()) 
                    m_onItemSlotClick(itemSlot->GetItemIndex()); 
            });

            itemSlot->SetOnHoverIn([this, index, itemSlot](const sf::Vector2f& mousePosition) 
            { 
                if (!m_onItemSlotHoverIn || !itemSlot || !itemSlot->HasItem())
                    return;

                auto* widget = GetWidget(index);
                if (!widget)
                    return;

                 m_onItemSlotHoverIn(itemSlot->GetItemIndex(), widget->GetPosition());
            });

            itemSlot->SetOnHoverOut([this]() 
            { 
                if (m_onItemSlotHoverOut)
                    m_onItemSlotHoverOut(); 
            });
        }
    }

    void ItemGridPanel::SetItems(const std::vector<app_domain::InventoryItemDetails>& items)
    {
        size_t cellCount = GetWidgetCount();
        for (size_t index = 0; index < cellCount; ++index)
        {
            auto* widget = GetWidget(index);
            if (!widget)
                continue;

            auto* itemSlot = static_cast<ItemSlot*>(widget);

            if (index < items.size())
                itemSlot->SetItem(items[index]);
            else
                itemSlot->ClearItem();
        }
    }

    void ItemGridPanel::SetOnItemSlotClick(OnItemSlotClick callback)
    {
        m_onItemSlotClick = std::move(callback);
    }

    void ItemGridPanel::SetOnItemSlotHoverIn(OnItemSlotHoverIn callback)
    {
        m_onItemSlotHoverIn = std::move(callback);
    }

    void ItemGridPanel::SetOnItemSlotHoverOut(OnItemSlotHoverOut callback)
    {
        m_onItemSlotHoverOut = std::move(callback);
    }

    ItemGridPanel::ItemSlot::ItemSlot(core::AssetManager& assetManager, float cellSize)
        : m_assetManager(assetManager)
    {
        float outlineThickness = 2.f;
        float iconSize = cellSize - outlineThickness * 2.f;

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        SetAnchor(0.f, 0.f);
        SetPivot(0.f, 0.f);
        SetColor(sf::Color::Transparent);

        auto* rarityGlowImage = CreateWidget<core::Image>();
        rarityGlowImage->SetSize(iconSize);
        rarityGlowImage->SetTexture(assetManager.GetTexture("UI_Overlay_ItemSlot_RarityGlow"));
        rarityGlowImage->SetActive(false);
        m_rarityGlowImage = rarityGlowImage;

        auto* iconImage = CreateWidget<core::Image>();
        iconImage->SetSize(iconSize);
        iconImage->SetActive(false);
        m_iconImage = iconImage;

        auto* frameImage = CreateWidget<core::Image>();
        frameImage->SetSize(iconSize);
        frameImage->SetColor(sf::Color::Transparent);
        frameImage->SetOutlineThickness(outlineThickness);
        frameImage->SetOutlineColor(sf::Color(64u, 54u, 48u, 255u));
        m_frameImage = frameImage;

        auto* countTextLabel = CreateWidget<core::TextLabel>();
        countTextLabel->SetAnchor(1.f, 1.f);
        countTextLabel->SetPivot(1.f, 1.f);
        countTextLabel->SetLocalPosition(-4.f, -8.f);
        countTextLabel->SetFont(regularFont);
        countTextLabel->SetFontSize(20);
        countTextLabel->SetText("3");
        countTextLabel->SetOutlineThickness(1.f);
        countTextLabel->SetActive(false);
        m_countTextLabel = countTextLabel;

        auto* selectImage = CreateWidget<core::Image>();
        selectImage->SetSize(iconSize + 2.f);
        selectImage->SetColor(sf::Color::Transparent);
        selectImage->SetOutlineThickness(outlineThickness);
        selectImage->SetOutlineColor(sf::Color::White);
        selectImage->SetActive(false);
        m_selectImage = selectImage;
    }

    void ItemGridPanel::ItemSlot::SetItem(const app_domain::InventoryItemDetails& item)
    {
        m_hasItem = true;
        m_itemIndex = item.GetIndex();

        if (!m_iconImage || !m_rarityGlowImage || !m_countTextLabel)
            return;

        m_iconImage->SetTexture(m_assetManager.GetTexture(item.GetItem().IconTextureId));
        m_iconImage->SetActive(true);

        if (item.GetItem().Rarity != app_domain::ItemRarity::Common)
        {
            m_rarityGlowImage->SetColor(ItemConfig::GetRarityColor(item.GetItem().Rarity));
            m_rarityGlowImage->SetActive(true);
        }

        if (item.GetCount() > 1u)
        {
            m_countTextLabel->SetText(std::to_string(item.GetCount()));
            m_countTextLabel->SetActive(true);
        }
    }

    void ItemGridPanel::ItemSlot::ItemSlot::ClearItem()
    {
        m_hasItem = false;

        if (!m_iconImage || !m_rarityGlowImage || !m_countTextLabel)
            return;

        m_iconImage->SetActive(false);

        m_rarityGlowImage->SetActive(false);

        m_countTextLabel->SetActive(false);
    }

    bool ItemGridPanel::ItemSlot::ItemSlot::HasItem() const
    {
        return m_hasItem;
    }

    size_t ItemGridPanel::ItemSlot::ItemSlot::GetItemIndex() const
    {
        return m_itemIndex;
    }
}