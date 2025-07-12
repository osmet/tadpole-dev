#include "Precompiled.h"
#include "ItemGridPanel.h"

#include "../app_domain/InventoryItemDetails.h"
#include "ItemSlot.h"

namespace app
{
    ItemGridPanel::ItemGridPanel(core::AssetManager& assetManager, sf::RenderWindow& renderWindow,
        size_t columnCount, size_t rowCount, float cellSize, float spacing)
        : m_renderWindow(renderWindow)
    {
        size_t cellCount = columnCount * rowCount;

        SetColumnCount(columnCount);
        SetCellSize(cellSize);
        SetSpacing(spacing);
        
        for (size_t index = 0; index < cellCount; ++index)
            CreateWidget<ItemSlot>(assetManager, cellSize);
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

        HandleEvent(sf::Event(sf::Event::MouseMoved), m_renderWindow); // Refresh SelectMode
    }
    
    void ItemGridPanel::SetOnItemClick(OnItemClick callback)
    {
        m_onItemClick = std::move(callback);
    }

    void ItemGridPanel::SetOnItemHoverIn(OnItemHoverIn callback)
    {
        m_onItemHoverIn = std::move(callback);
    }

    void ItemGridPanel::SetOnItemHoverOut(OnItemHoverOut callback)
    {
        m_onItemHoverOut = std::move(callback);
    }

    void ItemGridPanel::SetOnItemDragBegin(OnItemDragBegin callback)
    {
        m_onItemDragBegin = std::move(callback);
    }

    bool ItemGridPanel::CanItemDragEnd(sf::Vector2f dragEndPosition, std::int32_t& out_signedToItemIndex) const
    {
        sf::Vector2f panelPosition = GetPosition();
        size_t columnCount = GetColumnCount();
        float cellSize = GetCellSize().x;
        float spacing = GetSpacing().x;
        size_t slotCount = GetWidgetCount();
        size_t rowCount = (columnCount == 0) ? 0 : (slotCount + columnCount - 1) / columnCount;

        sf::FloatRect panelBounds(
            panelPosition.x,
            panelPosition.y,
            columnCount * (cellSize + spacing) - spacing,
            rowCount * (cellSize + spacing) - spacing
        );

        out_signedToItemIndex = -1;

        if (!panelBounds.contains(dragEndPosition))
            return false;

        sf::Vector2f relativePosition = dragEndPosition - panelPosition;
        size_t columnIndex = static_cast<size_t>(relativePosition.x / (cellSize + spacing));
        size_t rowIndex = static_cast<size_t>(relativePosition.y / (cellSize + spacing));
        size_t slotIndex = rowIndex * columnCount + columnIndex;

        if (slotIndex >= slotCount)
            return true;

        auto* widget = GetWidget(slotIndex);
        if (!widget)
            return true;

        auto* itemSlot = static_cast<ItemSlot*>(widget);
        if (!itemSlot)
            return true;

        if (!itemSlot->HasItem())
            return true;

        out_signedToItemIndex = static_cast<std::int32_t>(itemSlot->GetItemIndex());

        return true;
    }

    bool ItemGridPanel::OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(renderWindow));

        sf::Vector2f panelPosition = GetPosition();
        size_t columnCount = GetColumnCount();
        float cellSize = GetCellSize().x;
        float spacing = GetSpacing().x;
        size_t slotCount = GetWidgetCount();
        size_t rowCount = (columnCount == 0u) ? 0u : (slotCount + columnCount - 1u) / columnCount;

        sf::FloatRect panelBounds(
            panelPosition.x,
            panelPosition.y,
            columnCount * (cellSize + spacing) - spacing,
            rowCount * (cellSize + spacing) - spacing
        );

        if (!panelBounds.contains(mousePosition))
        {
            m_mouseDown = false;
            m_dragItemIndex = -1;

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

        if (event.type != sf::Event::MouseButtonPressed && !m_mouseDown)
        {
            HandleHoverIn(itemSlot, slotIndex);
        }
        else if (event.type == sf::Event::MouseButtonPressed 
            && event.mouseButton.button == sf::Mouse::Left)
        {
            m_mouseDown = true;
            m_dragItemIndex = static_cast<std::int32_t>(itemSlot->GetItemIndex());
            m_mouseDownPosition = mousePosition;
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            m_mouseDown = false;
            m_dragItemIndex = -1;

            HandleHoverOut();
            HandleClick(itemSlot);

            return true;
        }
        else if (event.type == sf::Event::MouseMoved && m_mouseDown)
        {
            float distance = std::hypot(
                mousePosition.x - m_mouseDownPosition.x,
                mousePosition.y - m_mouseDownPosition.y);

            if (distance >= DragThreshold)
            {
                HandleHoverOut();
                HandleDragBegin();

                m_mouseDown = false;
                m_dragItemIndex = -1;

                return true;
            }
        }

        return false;
    }

    void ItemGridPanel::HandleClick(ItemSlot* itemSlot)
    {
        if (!itemSlot || !itemSlot->HasItem())
            return;

        if (m_onItemClick)
            m_onItemClick(itemSlot->GetItemIndex());
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

        if (m_onItemHoverIn)
            m_onItemHoverIn(itemSlot->GetItemIndex(), itemSlot->GetPosition());

        m_lastHoveredSlotIndex = signedSlotIndex;
    }

    void ItemGridPanel::HandleHoverOut()
    {
        if (m_lastHoveredSlotIndex != -1)
        {
            if (m_onItemHoverOut)
                m_onItemHoverOut();

            ClearLastHoveredSlotSelect();

            m_lastHoveredSlotIndex = -1;
        }
    }

    void ItemGridPanel::HandleDragBegin()
    {
        if (m_onItemDragBegin && m_dragItemIndex >= 0)
            m_onItemDragBegin(static_cast<size_t>(m_dragItemIndex));
    }

    void ItemGridPanel::ClearLastHoveredSlotSelect()
    {
        size_t slotIndex = static_cast<size_t>(m_lastHoveredSlotIndex);

        auto* widget = GetWidget(slotIndex);
        if (!widget)
            return;

        auto* lastHoverSlot = static_cast<ItemSlot*>(widget);
        if (!lastHoverSlot)
            return;

        lastHoverSlot->SetSelectMode(ItemSlot::SelectMode::None);
    }
}