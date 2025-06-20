#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
    class Image;
    class TextLabel;
}

namespace app_domain
{
    struct Item;
}

namespace app
{
    class ItemPanel : public core::CanvasPanel
    {
    public:
        ItemPanel(core::AssetManager& assetManager);

        void Show(const app_domain::Item& item, const sf::Vector2f& position, const sf::Vector2f& offset, float minPositionY);
        void Hide();

    private:
        core::AssetManager& m_assetManager;

        core::Image* m_backgroundImage = nullptr;
        core::Image* m_backgroundCenterOverlayImage = nullptr;
        core::Image* m_backgroundBottomOverlayImage = nullptr;
        core::Image* m_rarityOverlayImage = nullptr;
        core::Image* m_iconImage = nullptr;
        core::TextLabel* m_nameTextLabel = nullptr;
        core::TextLabel* m_rarityTextLabel = nullptr;
        core::Image* m_descriptionIconImage = nullptr;
        core::TextLabel* m_descriptionTextLabel = nullptr;
        core::Image* m_typeIconImage = nullptr;
        core::TextLabel* m_typeTextLabel = nullptr;
        core::CanvasPanel* m_footerPanel = nullptr;
        core::TextLabel* m_moneyTextLabel = nullptr;
        core::Image* m_moneyIconImage = nullptr;
        core::TextLabel* m_weightTextLabel = nullptr;
        core::Image* m_weightIconImage = nullptr;
    };
}
