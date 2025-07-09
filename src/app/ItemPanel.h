#pragma once

#include "../core/CanvasPanel.h"

namespace core
{
    class AssetManager;
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

        core::WidgetId m_backgroundImageId;
        core::WidgetId m_backgroundCenterOverlayImageId;
        core::WidgetId m_backgroundBottomOverlayImageId;
        core::WidgetId m_rarityOverlayImageId;
        core::WidgetId m_iconImageId;
        core::WidgetId m_nameTextLabelId;
        core::WidgetId m_rarityTextLabelId;
        core::WidgetId m_descriptionIconImageId;
        core::WidgetId m_descriptionTextLabelId;
        core::WidgetId m_typeIconImageId;
        core::WidgetId m_typeTextLabelId;
        core::WidgetId m_footerPanelId;
        core::WidgetId m_moneyTextLabelId;
        core::WidgetId m_moneyIconImageId;
        core::WidgetId m_weightTextLabelId;
        core::WidgetId m_weightIconImageId;
    };
}
