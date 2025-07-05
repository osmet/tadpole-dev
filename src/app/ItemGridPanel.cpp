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
            auto* itemSlot = CreateWidget<ItemSlot>(assetManager, cellSize);
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

    bool ItemGridPanel::OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(renderWindow));

        size_t columnCount = GetColumnCount();
        float cellSize = GetCellSize().x;
        float spacing = GetSpacing().x;
        size_t slotCount = GetWidgetCount();
        size_t rowCount = (columnCount == 0u) ? 0u : (slotCount + columnCount - 1u) / columnCount;

        sf::Vector2f panelPosition = GetPosition();
        sf::FloatRect panelBounds(panelPosition.x, panelPosition.y,
            columnCount * (cellSize + spacing) - spacing,
            rowCount * (cellSize + spacing) - spacing);

        if (!panelBounds.contains(mousePosition))
        {
            HandleHoverOut();
            return false;
        }

        sf::Vector2f relativePosition = mousePosition - panelPosition;
        size_t columnIndex = static_cast<size_t>(relativePosition.x / (cellSize + spacing));
        size_t rowIndex = static_cast<size_t>(relativePosition.y / (cellSize + spacing));
        size_t slotIndex = rowIndex * columnCount + columnIndex;

        if (slotIndex >= slotCount)
            return false;

        auto* widget = GetWidget(slotIndex);
        if (!widget)
            return false;

        auto* itemSlot = static_cast<ItemSlot*>(widget);
        if (!itemSlot)
            return false;

        HandleHoverIn(itemSlot, slotIndex);

        if (itemSlot->HasItem() && event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            HandleClick(itemSlot);
            return true;
        }

        return false;
    }

    void ItemGridPanel::HandleClick(ItemSlot* itemSlot)
    {
        if (!itemSlot)
            return;

        if (m_onItemSlotClick)
            m_onItemSlotClick(itemSlot->GetItemIndex());
    }

    void ItemGridPanel::HandleHoverIn(ItemSlot* itemSlot, size_t slotIndex)
    {
        if (!itemSlot)
            return;

        std::int32_t signedSlotIndex = static_cast<std::int32_t>(slotIndex);

        if (signedSlotIndex == m_lastHoveredSlotIndex)
            return;

        if (!itemSlot->HasItem())
        {
            HandleHoverOut();
            return;
        }

        ClearLastHoveredSlotSelect();

        itemSlot->SetSelectMode(ItemSlot::SelectMode::Hovered);

        if (m_onItemSlotHoverIn)
            m_onItemSlotHoverIn(itemSlot->GetItemIndex(), itemSlot->GetPosition());

        m_lastHoveredSlotIndex = signedSlotIndex;
    }

    void ItemGridPanel::HandleHoverOut()
    {
        if (m_lastHoveredSlotIndex != -1)
        {
            if (m_onItemSlotHoverOut)
                m_onItemSlotHoverOut();

            ClearLastHoveredSlotSelect();

            m_lastHoveredSlotIndex = -1;
        }
    }

    void ItemGridPanel::ClearLastHoveredSlotSelect()
    {
        size_t lastHoveredSlotIndex = static_cast<size_t>(m_lastHoveredSlotIndex);

        auto* widget = GetWidget(lastHoveredSlotIndex);
        if (!widget)
            return;

        auto* lastHoveredSlot = static_cast<ItemSlot*>(widget);
        if (!lastHoveredSlot)
            return;

        lastHoveredSlot->SetSelectMode(ItemSlot::SelectMode::None);
    }
}