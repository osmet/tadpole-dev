#pragma once

#include "../core/GridPanel.h"
#include "ItemSlot.h"
#include "ItemDragSystem.h"

namespace app_domain
{
    class InventoryItemDetails;
}

namespace app
{
    class ItemGridPanel : public core::GridPanel, public IItemDragTarget
    {
    public:
        using OnItemClick = std::function<void(std::size_t itemIndex)>;
        using OnItemHoverIn = std::function<void(std::size_t itemIndex, const sf::Vector2f& position)>;
        using OnItemHoverOut = std::function<void()>;
        using OnItemDragBegin = std::function<void(std::size_t itemIndex)>;

        ItemGridPanel(core::AssetManager& assetManager, sf::RenderWindow& renderWindow,
            size_t columnCount, size_t rowCount, float cellSize, float spacing);

        void SetItems(const std::vector<app_domain::InventoryItemDetails>& items);

        void SetOnItemClick(OnItemClick callback);
        void SetOnItemHoverIn(OnItemHoverIn callback);
        void SetOnItemHoverOut(OnItemHoverOut callback);
        void SetOnItemDragBegin(OnItemDragBegin callback);

        bool CanItemDragEnd(sf::Vector2f dragEndPosition, std::int32_t& out_signedToItemIndex) const override;

    protected:
        bool OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;

    private:
        void HandleClick(ItemSlot* itemSlot);
        void HandleHoverIn(ItemSlot* itemSlot, size_t slotIndex);
        void HandleHoverOut();
        void HandleDragBegin();
        void ClearLastHoveredSlotSelect();

        sf::RenderWindow& m_renderWindow;

        std::int32_t m_lastHoveredSlotIndex = -1;

        OnItemClick m_onItemClick;
        OnItemHoverIn m_onItemHoverIn;
        OnItemHoverOut m_onItemHoverOut;
        OnItemDragBegin m_onItemDragBegin;

        static constexpr float DragThreshold = 5.f;

        bool m_mouseDown = false;
        std::int32_t m_dragItemIndex = 0u;
        sf::Vector2f m_mouseDownPosition;
    };
}


