#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
    class Image;
    class TextLabel;
    class Button;
}

namespace app_domain
{
    class InventoryItemDetails;
}

namespace app
{
    class ScreenOverlayPanel;
    class ItemSlot;

    class ItemTransferPanel final : public core::CanvasPanel
    {
    public:
        using OnConfirm = std::function<void(std::uint32_t itemCount)>;
        using OnCancel = std::function<void()>;

        ItemTransferPanel(core::AssetManager& assetManager, const sf::Vector2f& renderWindowSize);

        void Show(const app_domain::InventoryItemDetails& item);

        void SetOnConfirm(OnConfirm callback);
        void SetOnCancel(OnCancel callback);

    private:
        core::WidgetId m_itemSlotId;
        core::WidgetId m_itemNameTextLabelId;
        core::WidgetId m_itemCountTextLabelId;
        core::WidgetId m_incrementCountButtonId;
        core::WidgetId m_decrementCountButtonId;

        OnConfirm m_onConfirm;
        OnCancel m_onCancel;

        std::size_t m_itemIndex = 0;
        std::uint32_t m_minItemCount = 1;
        std::uint32_t m_maxItemCount = 1;
        lang::ReactiveProperty<std::uint32_t> m_currentItemCount = 1;
    };
}
