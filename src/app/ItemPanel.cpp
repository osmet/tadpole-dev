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
        m_backgroundImage = backgroundImage;

        auto* backgroundCenterOverlayImage = m_backgroundImage->CreateWidget<core::Image>();
        backgroundCenterOverlayImage->SetAnchor(0.f, 0.f);
        backgroundCenterOverlayImage->SetPivot(0.f, 0.f);
        backgroundCenterOverlayImage->SetColor(sf::Color(102u, 61u, 20u, 25u));
        backgroundCenterOverlayImage->SetTexture(linearGradientMirrorTexture);
        m_backgroundCenterOverlayImage = backgroundCenterOverlayImage;

        auto* backgroundBottomOverlayImage = m_backgroundImage->CreateWidget<core::Image>();
        backgroundBottomOverlayImage->SetAnchor(0.f, 0.f);
        backgroundBottomOverlayImage->SetPivot(0.f, 0.f);
        backgroundBottomOverlayImage->SetColor(sf::Color(255u, 255u, 255u, 5u));
        backgroundBottomOverlayImage->SetTexture(linearGradientReverseTexture);
        m_backgroundBottomOverlayImage = backgroundBottomOverlayImage;

        auto* rarityOverlayImage = m_backgroundImage->CreateWidget<core::Image>();
        rarityOverlayImage->SetAnchor(0.f, 0.f);
        rarityOverlayImage->SetPivot(0.f, 0.f);
        rarityOverlayImage->SetSize(backgroundImage->GetSize().x, 128.f);
        rarityOverlayImage->SetTexture(linearGradientTexture);
        m_rarityOverlayImage = rarityOverlayImage;

        auto* iconImage = m_backgroundImage->CreateWidget<core::Image>();
        iconImage->SetSize(168.f);
        iconImage->SetAnchor(1.f, 0.f);
        iconImage->SetPivot(1.f, 0.f);
        m_iconImage = iconImage;

        auto* nameTextLabel = m_backgroundImage->CreateWidget<core::TextLabel>();
        nameTextLabel->SetAnchor(0.f, 0.f);
        nameTextLabel->SetPivot(0.f, 0.f);
        nameTextLabel->SetFont(regularFont);
        nameTextLabel->SetFontSize(22u);
        m_nameTextLabel = nameTextLabel;

        auto* rarityTextLabel = m_backgroundImage->CreateWidget<core::TextLabel>();
        rarityTextLabel->SetAnchor(0.f, 0.f);
        rarityTextLabel->SetPivot(0.f, 0.f);
        rarityTextLabel->SetFont(regularFont);
        rarityTextLabel->SetFontSize(17u);
        rarityTextLabel->SetColor(sf::Color(191u, 163u, 143u, 255u));
        m_rarityTextLabel = rarityTextLabel;

        sf::Color descriptionColor(255u, 255u, 255u, 128u);

        auto* descriptionIconImage = m_backgroundImage->CreateWidget<core::Image>();
        descriptionIconImage->SetSize(20.f);
        descriptionIconImage->SetAnchor(0.f, 0.f);
        descriptionIconImage->SetPivot(0.f, 0.f);
        descriptionIconImage->SetColor(descriptionColor);
        descriptionIconImage->SetTexture(descriptionIconTexture);
        m_descriptionIconImage = descriptionIconImage;

        auto* descriptionTextLabel = m_backgroundImage->CreateWidget<core::TextLabel>();
        descriptionTextLabel->SetAnchor(0.f, 0.f);
        descriptionTextLabel->SetPivot(0.f, 0.f);
        descriptionTextLabel->SetFont(italicFont);
        descriptionTextLabel->SetFontSize(17u);
        descriptionTextLabel->SetColor(descriptionColor);
        m_descriptionTextLabel = descriptionTextLabel;

        auto* typeIconImage = m_backgroundImage->CreateWidget<core::Image>();
        typeIconImage->SetSize(22.f);
        typeIconImage->SetAnchor(0.f, 0.f);
        typeIconImage->SetPivot(0.f, 0.f);
        typeIconImage->SetColor(sf::Color(255u, 255u, 255u, 200u));
        typeIconImage->SetTexture(typeIconTexture);
        m_typeIconImage = typeIconImage;

        auto* typeTextLabel = m_backgroundImage->CreateWidget<core::TextLabel>();
        typeTextLabel->SetAnchor(0.f, 0.f);
        typeTextLabel->SetPivot(0.f, 0.f);
        typeTextLabel->SetFont(regularFont);
        typeTextLabel->SetFontSize(17u);
        typeTextLabel->SetColor(sf::Color(191u, 163u, 143u, 255u));
        m_typeTextLabel = typeTextLabel;

        auto* footerPanel = m_backgroundImage->CreateWidget<core::CanvasPanel>();
        footerPanel->SetAnchor(0.f, 0.f);
        footerPanel->SetPivot(0.f, 0.f);
        footerPanel->SetSize(m_backgroundImage->GetSize().x, 45.f);
        m_footerPanel = footerPanel;

        auto* footerPanelOverlayImage = m_footerPanel->CreateWidget<core::Image>();
        footerPanelOverlayImage->SetAnchor(0.f, 1.f);
        footerPanelOverlayImage->SetPivot(0.f, 1.f);
        footerPanelOverlayImage->SetSize(m_footerPanel->GetSize().x, m_footerPanel->GetSize().y);
        footerPanelOverlayImage->SetColor(sf::Color(94u, 51u, 128u, 80u));
        footerPanelOverlayImage->SetTexture(linearGradientReverseTexture);
        
        auto* moneyTextLabel = footerPanel->CreateWidget<core::TextLabel>();
        moneyTextLabel->SetAnchor(1.f, 1.f);
        moneyTextLabel->SetPivot(1.f, 1.f);
        moneyTextLabel->SetFont(regularFont);
        moneyTextLabel->SetFontSize(20u);
        m_moneyTextLabel = moneyTextLabel;

        auto* moneyIconImage = footerPanel->CreateWidget<core::Image>();
        moneyIconImage->SetAnchor(1.f, 1.f);
        moneyIconImage->SetPivot(1.f, 1.f);
        moneyIconImage->SetSize(12.f, 14.f);
        moneyIconImage->SetColor(sf::Color(191u, 163u, 143u, 255u));
        moneyIconImage->SetTexture(moneyIconTexture);
        m_moneyIconImage = moneyIconImage;

        auto* weightTextLabel = footerPanel->CreateWidget<core::TextLabel>();
        weightTextLabel->SetAnchor(1.f, 1.f);
        weightTextLabel->SetPivot(1.f, 1.f);
        weightTextLabel->SetFont(regularFont);
        weightTextLabel->SetFontSize(20u);
        m_weightTextLabel = weightTextLabel;

        auto* weightIconImage = footerPanel->CreateWidget<core::Image>();
        weightIconImage->SetAnchor(1.f, 1.f);
        weightIconImage->SetPivot(1.f, 1.f);
        weightIconImage->SetSize(15.f);
        weightIconImage->SetTexture(weightIconTexture);
        m_weightIconImage = weightIconImage;
    }

    void ItemPanel::Show(const app_domain::Item& item, const sf::Vector2f& position, const sf::Vector2f& offset, float minPositionY)
    {
        if (!m_backgroundImage || !m_backgroundCenterOverlayImage || !m_backgroundBottomOverlayImage
            || !m_rarityOverlayImage || !m_iconImage || !m_nameTextLabel || !m_rarityTextLabel 
            || !m_descriptionIconImage || !m_descriptionTextLabel || !m_typeIconImage || !m_typeTextLabel
            || !m_footerPanel || !m_moneyTextLabel || !m_moneyIconImage || !m_weightTextLabel || !m_weightIconImage)
            return;

        m_iconImage->SetTexture(m_assetManager.GetTexture(item.IconTextureId));
        m_iconImage->SetLocalPosition(30.f, -30.f);

        sf::Vector2f padding(20.f, 16.f);
        float textIconPaddingX = 28.f;
        float maxTextWidth = 320.f;
        float minBodyPanelSize = m_iconImage->GetSize().y + m_iconImage->GetLocalPosition().y + padding.y;
        float panelSizeY = padding.y;

        sf::Color rarityNameColor(ItemConfig::GetRarityColor(item.Rarity));

        bool isCommon = item.Rarity == app_domain::ItemRarity::Common;

        if (!isCommon)
        {
            sf::Color rarityOverlayColor(rarityNameColor);
            rarityOverlayColor.a = 50u;

            m_rarityOverlayImage->SetColor(rarityOverlayColor);
            m_rarityOverlayImage->SetActive(true);
        }
        else
        {
            m_rarityOverlayImage->SetActive(false);
        }

        m_nameTextLabel->SetText(item.Name);
        m_nameTextLabel->SetLocalPosition(padding);
        m_nameTextLabel->SetColor(rarityNameColor);
        panelSizeY += m_nameTextLabel->GetSize().y;

        if (!isCommon)
        {
            panelSizeY += 10.f;
            m_rarityTextLabel->SetLocalPosition(padding.x, panelSizeY);
            m_rarityTextLabel->SetText("[" + app_domain::ItemRarityHelper::ToString(item.Rarity) + "]");
            m_rarityTextLabel->SetActive(true);
            panelSizeY += m_rarityTextLabel->GetSize().y;
        }
        else
        {
            m_rarityTextLabel->SetActive(false);
        }

        panelSizeY += 20.f;
        m_descriptionIconImage->SetLocalPosition(padding.x, panelSizeY);

        panelSizeY += 6.f;
        m_descriptionTextLabel->SetLocalPosition(padding.x + textIconPaddingX, panelSizeY);
        std::string description = item.Description;
        if (!description.empty() && description.back() != '.')
            description += ".";
        m_descriptionTextLabel->SetText(description);
        panelSizeY += m_descriptionTextLabel->GetSize().y;

        panelSizeY += 26.f;
        m_typeIconImage->SetLocalPosition(padding.x, panelSizeY);

        panelSizeY += 6.f;
        m_typeTextLabel->SetLocalPosition(padding.x + textIconPaddingX, panelSizeY);
        m_typeTextLabel->SetText(app_domain::ItemTypeHelper::ToString(item.Type));
        panelSizeY += m_typeTextLabel->GetSize().y;

        panelSizeY += padding.y;

        if (panelSizeY < minBodyPanelSize)
        {
            panelSizeY = minBodyPanelSize;

            m_typeIconImage->SetLocalPosition(padding.x, panelSizeY - padding.y - m_typeTextLabel->GetSize().y - 6.f);
            m_typeTextLabel->SetLocalPosition(padding.x + textIconPaddingX, panelSizeY - padding.y - m_typeTextLabel->GetSize().y);
        }

        m_footerPanel->SetLocalPosition(0.f, panelSizeY);

        m_moneyIconImage->SetLocalPosition(-padding.x, -padding.y + 1.f);

        m_moneyTextLabel->SetText(std::to_string(item.Value));
        m_moneyTextLabel->SetLocalPosition(m_moneyIconImage->GetLocalPosition().x - 20.f, -padding.y);

        m_weightIconImage->SetLocalPosition(m_moneyTextLabel->GetLocalPosition().x - m_moneyTextLabel->GetSize().x - padding.x, -padding.y);

        m_weightTextLabel->SetLocalPosition(m_weightIconImage->GetLocalPosition().x - 22.f, -padding.y);

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.1f", item.Weight);
        m_weightTextLabel->SetText(buffer);

        float panelWidth = m_backgroundImage->GetSize().x;
        float panelHeight = panelSizeY + m_footerPanel->GetSize().y;

        m_backgroundCenterOverlayImage->SetSize(panelWidth, panelSizeY);
        m_backgroundBottomOverlayImage->SetSize(panelWidth, 45.f);
        m_backgroundBottomOverlayImage->SetLocalPosition(0.f, panelSizeY - m_backgroundBottomOverlayImage->GetSize().y);

        m_backgroundImage->SetSize(panelWidth, panelHeight);

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