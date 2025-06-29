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

    private:
        ItemSlot::OnItemSlotClick m_onItemSlotClick;
        ItemSlot::OnItemSlotHoverIn m_onItemSlotHoverIn;
        ItemSlot::OnItemSlotHoverOut m_onItemSlotHoverOut;
    };
}


