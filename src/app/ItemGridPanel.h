#pragma once

#include "../core/GridPanel.h"
#include "ItemSlot.h"

namespace app_domain 
{ 
    class InventoryItemDetails;
}

namespace app
{
    class ItemGridPanel : public core::GridPanel
    {
    public:
        ItemGridPanel(core::AssetManager& assetManager,
            size_t columnCount, size_t rowCount, float cellSize, float spacing);

        void SetItems(const std::vector<app_domain::InventoryItemDetails>& items);

        void SetOnItemSlotClick(ItemSlot::OnItemSlotClick callback);
        void SetOnItemSlotHoverIn(ItemSlot::OnItemSlotHoverIn callback);
        void SetOnItemSlotHoverOut(ItemSlot::OnItemSlotHoverOut callback);

    protected:
        bool OnHandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow) override;

    private:
        void HandleClick(ItemSlot* itemSlot);
        void HandleHoverIn(ItemSlot* itemSlot, size_t slotIndex);
        void HandleHoverOut();

        void ClearLastHoveredSlotSelect();

        std::int32_t m_lastHoveredSlotIndex = -1;

        ItemSlot::OnItemSlotClick m_onItemSlotClick;
        ItemSlot::OnItemSlotHoverIn m_onItemSlotHoverIn;
        ItemSlot::OnItemSlotHoverOut m_onItemSlotHoverOut;
    };
}


