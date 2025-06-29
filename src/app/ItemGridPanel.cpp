#include "Precompiled.h"
#include "ItemGridPanel.h"

#include "../app_domain/InventoryItemDetails.h"
#include "ItemSlot.h"

namespace app
{
    ItemGridPanel::ItemGridPanel(core::AssetManager& assetManager,
        size_t columnCount, size_t rowCount, float cellSize, float spacing)
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

    void ItemGridPanel::SetOnItemSlotClick(ItemSlot::OnItemSlotClick callback)
    {
        m_onItemSlotClick = std::move(callback);
    }

    void ItemGridPanel::SetOnItemSlotHoverIn(ItemSlot::OnItemSlotHoverIn callback)
    {
        m_onItemSlotHoverIn = std::move(callback);
    }

    void ItemGridPanel::SetOnItemSlotHoverOut(ItemSlot::OnItemSlotHoverOut callback)
    {
        m_onItemSlotHoverOut = std::move(callback);
    }
}