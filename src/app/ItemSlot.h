#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
}

namespace app_domain
{
    class InventoryItemDetails;
}

namespace app
{
    class ItemSlot final : public core::CanvasPanel
    {
    public:
        enum class SelectMode
        {
            None,
            Hovered,
            Selected,
        };

        ItemSlot(core::AssetManager& assetManager, float cellSize);

        void SetItem(const app_domain::InventoryItemDetails& item);
        void ClearItem();

        bool HasItem() const;
        size_t GetItemIndex() const;

        void SetBackgroundImageActive(bool active);
        void SetCountTextActive(bool active);

        void SetSelectMode(SelectMode selectMode);

    private:
        core::AssetManager& m_assetManager;

        core::WidgetId m_backgroundImageId;
        core::WidgetId m_rarityGlowImageId;
        core::WidgetId m_iconImageId;
        core::WidgetId m_frameImageId;
        core::WidgetId m_countTextLabelId;
        core::WidgetId m_selectImageId;

        bool m_hasItem = false;
        size_t m_itemIndex = 0u;

        bool m_countTextActive = true;

        SelectMode m_selectMode = SelectMode::None;
    };
}
