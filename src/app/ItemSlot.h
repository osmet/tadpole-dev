#pragma once

#include "../core/Button.h"

namespace core
{
    class AssetManager;
    class Image;
    class TextLabel;
}

namespace app_domain
{
    class InventoryItemDetails;
}

namespace app
{
    class ItemSlot final : public core::Button
    {
    public:
        using OnItemSlotClick = std::function<void(std::size_t itemIndex)>;
        using OnItemSlotHoverIn = std::function<void(std::size_t itemIndex, const sf::Vector2f& position)>;
        using OnItemSlotHoverOut = std::function<void()>;

        ItemSlot(core::AssetManager& assetManager, float cellSize);

        void SetItem(const app_domain::InventoryItemDetails& item);
        void ClearItem();

        bool HasItem() const;
        size_t GetItemIndex() const;

        void SetBackgroundImageActive(bool active);
        void SetCountTextActive(bool active);

    private:
        core::AssetManager& m_assetManager;

        core::Image* m_backgroundImage = nullptr;
        core::Image* m_rarityGlowImage = nullptr;
        core::Image* m_iconImage = nullptr;
        core::Image* m_frameImage = nullptr;
        core::TextLabel* m_countTextLabel = nullptr;
        core::Image* m_selectImage = nullptr;

        bool m_hasItem = false;
        size_t m_itemIndex = 0u;

        bool m_countTextActive = true;
    };
}
