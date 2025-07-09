#include "Precompiled.h"
#include "ItemPanel.h"
#include "../core/AssetManager.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"
#include "../app_domain/Item.h"
#include "../app_domain/ItemType.h"
#include "ItemConfig.h"

namespace app
{
    ItemPanel::ItemPanel(core::AssetManager& assetManager)
        : m_assetManager(assetManager)
    {
        SetAnchor(0.f, 0.f);
        SetPivot(0.f, 0.f);
        SetActive(false);

        auto& regularFont = assetManager.GetFont("Mignon_Regular");
        auto& italicFont = assetManager.GetFont("Mignon_Italic");

        auto& moneyIconTexture = assetManager.GetTexture("UI_Icon_Coin");
        auto& weightIconTexture = assetManager.GetTexture("UI_Icon_Weight");
        auto& descriptionIconTexture = assetManager.GetTexture("UI_Icon_ItemDescription");
        auto& typeIconTexture = assetManager.GetTexture("UI_Icon_ItemType");
        auto& linearGradientTexture = assetManager.GetTexture("UI_Overlay_LinearGradient_Vertical");
        auto& linearGradientReverseTexture = assetManager.GetTexture("UI_Overlay_LinearGradient_Vertical_Reverse");
        auto& linearGradientMirrorTexture = assetManager.GetTexture("UI_Overlay_LinearGradient_Horizontal_Mirror");

        auto* backgroundImage = CreateWidget<core::Image>();
        backgroundImage->SetAnchor(0.f, 0.f);
        backgroundImage->SetPivot(0.f, 0.f);
        backgroundImage->SetSize(430.f, 100.f);
        backgroundImage->SetColor(sf::Color(25u, 25u, 25u, 255u));
        backgroundImage->SetOutlineThickness(2.f);
        backgroundImage->SetOutlineColor(sf::Color(153u, 117u, 92u, 255u));
        m_backgroundImageId = backgroundImage->GetId();

        auto* backgroundCenterOverlayImage = backgroundImage->CreateWidget<core::Image>();
        backgroundCenterOverlayImage->SetAnchor(0.f, 0.f);
        backgroundCenterOverlayImage->SetPivot(0.f, 0.f);
        backgroundCenterOverlayImage->SetColor(sf::Color(102u, 61u, 20u, 25u));
        backgroundCenterOverlayImage->SetTexture(linearGradientMirrorTexture);
        m_backgroundCenterOverlayImageId = backgroundCenterOverlayImage->GetId();

        auto* backgroundBottomOverlayImage = backgroundImage->CreateWidget<core::Image>();
        backgroundBottomOverlayImage->SetAnchor(0.f, 0.f);
        backgroundBottomOverlayImage->SetPivot(0.f, 0.f);
        backgroundBottomOverlayImage->SetColor(sf::Color(255u, 255u, 255u, 5u));
        backgroundBottomOverlayImage->SetTexture(linearGradientReverseTexture);
        m_backgroundBottomOverlayImageId = backgroundBottomOverlayImage->GetId();

        auto* rarityOverlayImage = backgroundImage->CreateWidget<core::Image>();
        rarityOverlayImage->SetAnchor(0.f, 0.f);
        rarityOverlayImage->SetPivot(0.f, 0.f);
        rarityOverlayImage->SetSize(backgroundImage->GetSize().x, 128.f);
        rarityOverlayImage->SetTexture(linearGradientTexture);
        m_rarityOverlayImageId = rarityOverlayImage->GetId();

        auto* iconImage = backgroundImage->CreateWidget<core::Image>();
        iconImage->SetSize(168.f);
        iconImage->SetAnchor(1.f, 0.f);
        iconImage->SetPivot(1.f, 0.f);
        m_iconImageId = iconImage->GetId();

        auto* nameTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        nameTextLabel->SetAnchor(0.f, 0.f);
        nameTextLabel->SetPivot(0.f, 0.f);
        nameTextLabel->SetFont(regularFont);
        nameTextLabel->SetFontSize(22u);
        m_nameTextLabelId = nameTextLabel->GetId();

        auto* rarityTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        rarityTextLabel->SetAnchor(0.f, 0.f);
        rarityTextLabel->SetPivot(0.f, 0.f);
        rarityTextLabel->SetFont(regularFont);
        rarityTextLabel->SetFontSize(17u);
        rarityTextLabel->SetColor(sf::Color(191u, 163u, 143u, 255u));
        m_rarityTextLabelId = rarityTextLabel->GetId();

        sf::Color descriptionColor(255u, 255u, 255u, 128u);

        auto* descriptionIconImage = backgroundImage->CreateWidget<core::Image>();
        descriptionIconImage->SetSize(20.f);
        descriptionIconImage->SetAnchor(0.f, 0.f);
        descriptionIconImage->SetPivot(0.f, 0.f);
        descriptionIconImage->SetColor(descriptionColor);
        descriptionIconImage->SetTexture(descriptionIconTexture);
        m_descriptionIconImageId = descriptionIconImage->GetId();

        auto* descriptionTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        descriptionTextLabel->SetAnchor(0.f, 0.f);
        descriptionTextLabel->SetPivot(0.f, 0.f);
        descriptionTextLabel->SetFont(italicFont);
        descriptionTextLabel->SetFontSize(17u);
        descriptionTextLabel->SetColor(descriptionColor);
        m_descriptionTextLabelId = descriptionTextLabel->GetId();

        auto* typeIconImage = backgroundImage->CreateWidget<core::Image>();
        typeIconImage->SetSize(22.f);
        typeIconImage->SetAnchor(0.f, 0.f);
        typeIconImage->SetPivot(0.f, 0.f);
        typeIconImage->SetColor(sf::Color(255u, 255u, 255u, 200u));
        typeIconImage->SetTexture(typeIconTexture);
        m_typeIconImageId = typeIconImage->GetId();

        auto* typeTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        typeTextLabel->SetAnchor(0.f, 0.f);
        typeTextLabel->SetPivot(0.f, 0.f);
        typeTextLabel->SetFont(regularFont);
        typeTextLabel->SetFontSize(17u);
        typeTextLabel->SetColor(sf::Color(191u, 163u, 143u, 255u));
        m_typeTextLabelId = typeTextLabel->GetId();

        auto* footerPanel = backgroundImage->CreateWidget<core::CanvasPanel>();
        footerPanel->SetAnchor(0.f, 0.f);
        footerPanel->SetPivot(0.f, 0.f);
        footerPanel->SetSize(backgroundImage->GetSize().x, 45.f);
        m_footerPanelId = footerPanel->GetId();

        auto* footerPanelOverlayImage = footerPanel->CreateWidget<core::Image>();
        footerPanelOverlayImage->SetAnchor(0.f, 1.f);
        footerPanelOverlayImage->SetPivot(0.f, 1.f);
        footerPanelOverlayImage->SetSize(footerPanel->GetSize().x, footerPanel->GetSize().y);
        footerPanelOverlayImage->SetColor(sf::Color(94u, 51u, 128u, 80u));
        footerPanelOverlayImage->SetTexture(linearGradientReverseTexture);
        
        auto* moneyTextLabel = footerPanel->CreateWidget<core::TextLabel>();
        moneyTextLabel->SetAnchor(1.f, 1.f);
        moneyTextLabel->SetPivot(1.f, 1.f);
        moneyTextLabel->SetFont(regularFont);
        moneyTextLabel->SetFontSize(20u);
        m_moneyTextLabelId = moneyTextLabel->GetId();

        auto* moneyIconImage = footerPanel->CreateWidget<core::Image>();
        moneyIconImage->SetAnchor(1.f, 1.f);
        moneyIconImage->SetPivot(1.f, 1.f);
        moneyIconImage->SetSize(12.f, 14.f);
        moneyIconImage->SetColor(sf::Color(191u, 163u, 143u, 255u));
        moneyIconImage->SetTexture(moneyIconTexture);
        m_moneyIconImageId = moneyIconImage->GetId();

        auto* weightTextLabel = footerPanel->CreateWidget<core::TextLabel>();
        weightTextLabel->SetAnchor(1.f, 1.f);
        weightTextLabel->SetPivot(1.f, 1.f);
        weightTextLabel->SetFont(regularFont);
        weightTextLabel->SetFontSize(20u);
        m_weightTextLabelId = weightTextLabel->GetId();

        auto* weightIconImage = footerPanel->CreateWidget<core::Image>();
        weightIconImage->SetAnchor(1.f, 1.f);
        weightIconImage->SetPivot(1.f, 1.f);
        weightIconImage->SetSize(15.f);
        weightIconImage->SetTexture(weightIconTexture);
        m_weightIconImageId = weightIconImage->GetId();
    }

    void ItemPanel::Show(const app_domain::Item& item, const sf::Vector2f& position, const sf::Vector2f& offset, float minPositionY)
    {
        auto* backgroundImage = FindWidgetById<core::Image>(m_backgroundImageId);
        auto* backgroundCenterOverlayImage = FindWidgetById<core::Image>(m_backgroundCenterOverlayImageId);
        auto* backgroundBottomOverlayImage = FindWidgetById<core::Image>(m_backgroundBottomOverlayImageId);
        auto* rarityOverlayImage = FindWidgetById<core::Image>(m_rarityOverlayImageId);
        auto* iconImage = FindWidgetById<core::Image>(m_iconImageId);
        auto* nameTextLabel = FindWidgetById<core::TextLabel>(m_nameTextLabelId);
        auto* rarityTextLabel = FindWidgetById<core::TextLabel>(m_rarityTextLabelId);
        auto* descriptionIconImage = FindWidgetById<core::Image>(m_descriptionIconImageId);
        auto* descriptionTextLabel = FindWidgetById<core::TextLabel>(m_descriptionTextLabelId);
        auto* typeIconImage = FindWidgetById<core::Image>(m_typeIconImageId);
        auto* typeTextLabel = FindWidgetById<core::TextLabel>(m_typeTextLabelId);
        auto* footerPanel = FindWidgetById<core::CanvasPanel>(m_footerPanelId);
        auto* moneyTextLabel = FindWidgetById<core::TextLabel>(m_moneyTextLabelId);
        auto* moneyIconImage = FindWidgetById<core::Image>(m_moneyIconImageId);
        auto* weightTextLabel = FindWidgetById<core::TextLabel>(m_weightTextLabelId);
        auto* weightIconImage = FindWidgetById<core::Image>(m_weightIconImageId);
        
        if (!backgroundImage || !backgroundCenterOverlayImage || !backgroundBottomOverlayImage
            || !rarityOverlayImage || !iconImage || !nameTextLabel || !rarityTextLabel 
            || !descriptionIconImage || !descriptionTextLabel || !typeIconImage || !typeTextLabel
            || !footerPanel || !moneyTextLabel || !moneyIconImage || !weightTextLabel || !weightIconImage)
            return;

        iconImage->SetTexture(m_assetManager.GetTexture(item.IconTextureId));
        iconImage->SetLocalPosition(30.f, -30.f);

        sf::Vector2f padding(20.f, 16.f);
        float textIconPaddingX = 28.f;
        float maxTextWidth = 320.f;
        float minBodyPanelSize = iconImage->GetSize().y + iconImage->GetLocalPosition().y + padding.y;
        float panelSizeY = padding.y;

        sf::Color rarityNameColor(ItemConfig::GetRarityColor(item.Rarity));

        bool isCommon = item.Rarity == app_domain::ItemRarity::Common;

        if (!isCommon)
        {
            sf::Color rarityOverlayColor(rarityNameColor);
            rarityOverlayColor.a = 50u;

            rarityOverlayImage->SetColor(rarityOverlayColor);
            rarityOverlayImage->SetActive(true);
        }
        else
        {
            rarityOverlayImage->SetActive(false);
        }

        nameTextLabel->SetText(item.Name);
        nameTextLabel->SetLocalPosition(padding);
        nameTextLabel->SetColor(rarityNameColor);
        panelSizeY += nameTextLabel->GetSize().y;

        if (!isCommon)
        {
            panelSizeY += 10.f;
            rarityTextLabel->SetLocalPosition(padding.x, panelSizeY);
            rarityTextLabel->SetText("[" + app_domain::ItemRarityHelper::ToString(item.Rarity) + "]");
            rarityTextLabel->SetActive(true);
            panelSizeY += rarityTextLabel->GetSize().y;
        }
        else
        {
            rarityTextLabel->SetActive(false);
        }

        panelSizeY += 20.f;
        descriptionIconImage->SetLocalPosition(padding.x, panelSizeY);

        panelSizeY += 6.f;
        descriptionTextLabel->SetLocalPosition(padding.x + textIconPaddingX, panelSizeY);
        std::string description = item.Description;
        if (!description.empty() && description.back() != '.')
            description += ".";
        descriptionTextLabel->SetText(description);
        panelSizeY += descriptionTextLabel->GetSize().y;

        panelSizeY += 26.f;
        typeIconImage->SetLocalPosition(padding.x, panelSizeY);

        panelSizeY += 6.f;
        typeTextLabel->SetLocalPosition(padding.x + textIconPaddingX, panelSizeY);
        typeTextLabel->SetText(app_domain::ItemTypeHelper::ToString(item.Type));
        panelSizeY += typeTextLabel->GetSize().y;

        panelSizeY += padding.y;

        if (panelSizeY < minBodyPanelSize)
        {
            panelSizeY = minBodyPanelSize;

            typeIconImage->SetLocalPosition(padding.x, panelSizeY - padding.y - typeTextLabel->GetSize().y - 6.f);
            typeTextLabel->SetLocalPosition(padding.x + textIconPaddingX, panelSizeY - padding.y - typeTextLabel->GetSize().y);
        }

        footerPanel->SetLocalPosition(0.f, panelSizeY);

        moneyIconImage->SetLocalPosition(-padding.x, -padding.y + 1.f);

        moneyTextLabel->SetText(std::to_string(item.Value));
        moneyTextLabel->SetLocalPosition(moneyIconImage->GetLocalPosition().x - 20.f, -padding.y);

        weightIconImage->SetLocalPosition(moneyTextLabel->GetLocalPosition().x - moneyTextLabel->GetSize().x - padding.x, -padding.y);

        weightTextLabel->SetLocalPosition(weightIconImage->GetLocalPosition().x - 22.f, -padding.y);

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.1f", item.Weight);
        weightTextLabel->SetText(buffer);

        float panelWidth = backgroundImage->GetSize().x;
        float panelHeight = panelSizeY + footerPanel->GetSize().y;

        backgroundCenterOverlayImage->SetSize(panelWidth, panelSizeY);
        backgroundBottomOverlayImage->SetSize(panelWidth, 45.f);
        backgroundBottomOverlayImage->SetLocalPosition(0.f, panelSizeY - backgroundBottomOverlayImage->GetSize().y);

        backgroundImage->SetSize(panelWidth, panelHeight);

        sf::Vector2f targetPosition(position + offset);

        if (targetPosition.y + panelHeight > minPositionY)
            targetPosition.y = minPositionY - panelHeight;

        SetLocalPosition(targetPosition);

        SetActive(true);
    }

    void ItemPanel::Hide()
    {
        SetActive(false);
    }
}