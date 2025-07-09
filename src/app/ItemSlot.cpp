#include "Precompiled.h"
#include "ItemSlot.h"

#include "../core/AssetManager.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"
#include "../app_domain/Item.h"
#include "../app_domain/ItemRarity.h"
#include "../app_domain/InventoryItemDetails.h"
#include "ItemConfig.h"

namespace app
{
    ItemSlot::ItemSlot(core::AssetManager& assetManager, float cellSize)
        : m_assetManager(assetManager)
    {
        float outlineThickness = 2.f;
        float iconSize = cellSize - outlineThickness * 2.f;

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        SetAnchor(0.f, 0.f);
        SetPivot(0.f, 0.f);

        auto* backgroundImage = CreateWidget<core::Image>();
        backgroundImage->SetSize(iconSize);
        backgroundImage->SetColor(sf::Color(13u, 11u, 9u, 255u));
        backgroundImage->SetActive(false);
        m_backgroundImageId = backgroundImage->GetId();

        auto* rarityGlowImage = CreateWidget<core::Image>();
        rarityGlowImage->SetSize(iconSize);
        rarityGlowImage->SetTexture(assetManager.GetTexture("UI_Overlay_ItemSlot_RarityGlow"));
        rarityGlowImage->SetActive(false);
        m_rarityGlowImageId = rarityGlowImage->GetId();

        auto* iconImage = CreateWidget<core::Image>();
        iconImage->SetSize(iconSize);
        iconImage->SetActive(false);
        m_iconImageId = iconImage->GetId();

        auto* frameImage = CreateWidget<core::Image>();
        frameImage->SetSize(iconSize);
        frameImage->SetColor(sf::Color::Transparent);
        frameImage->SetOutlineThickness(outlineThickness);
        frameImage->SetOutlineColor(sf::Color(64u, 54u, 48u, 255u));
        m_frameImageId = frameImage->GetId();

        auto* countTextLabel = CreateWidget<core::TextLabel>();
        countTextLabel->SetAnchor(1.f, 1.f);
        countTextLabel->SetPivot(1.f, 1.f);
        countTextLabel->SetLocalPosition(-4.f, -8.f);
        countTextLabel->SetFont(regularFont);
        countTextLabel->SetFontSize(20);
        countTextLabel->SetText("3");
        countTextLabel->SetOutlineThickness(1.f);
        countTextLabel->SetActive(false);
        m_countTextLabelId = countTextLabel->GetId();

        auto* selectImage = CreateWidget<core::Image>();
        selectImage->SetSize(iconSize + 2.f);
        selectImage->SetColor(sf::Color::Transparent);
        selectImage->SetOutlineThickness(outlineThickness);
        selectImage->SetOutlineColor(sf::Color::White);
        selectImage->SetActive(false);
        m_selectImageId = selectImage->GetId();
    }

    void ItemSlot::SetItem(const app_domain::InventoryItemDetails& item)
    {
        m_hasItem = true;
        m_itemIndex = item.GetIndex();

        auto* iconImage = FindWidgetById<core::Image>(m_iconImageId);
        auto* rarityGlowImage = FindWidgetById<core::Image>(m_rarityGlowImageId);
        auto* countTextLabel = FindWidgetById<core::TextLabel>(m_countTextLabelId);

        if (!iconImage || !rarityGlowImage || !countTextLabel)
            return;

        iconImage->SetTexture(m_assetManager.GetTexture(item.GetItem().IconTextureId));
        iconImage->SetActive(true);

        if (item.GetItem().Rarity != app_domain::ItemRarity::Common)
        {
            rarityGlowImage->SetColor(ItemConfig::GetRarityColor(item.GetItem().Rarity));
            rarityGlowImage->SetActive(true);
        }
        else
        {
            rarityGlowImage->SetActive(false);
        }

        if (m_countTextActive && item.GetCount() > 1u)
        {
            countTextLabel->SetText(std::to_string(item.GetCount()));
            countTextLabel->SetActive(true);
        }
        else
        {
            countTextLabel->SetActive(false);
        }
    }

    void ItemSlot::ClearItem()
    {
        m_hasItem = false;

        auto* iconImage = FindWidgetById<core::Image>(m_iconImageId);
        auto* rarityGlowImage = FindWidgetById<core::Image>(m_rarityGlowImageId);
        auto* countTextLabel = FindWidgetById<core::TextLabel>(m_countTextLabelId);

        if (!iconImage || !rarityGlowImage || !countTextLabel)
            return;

        iconImage->SetActive(false);
        rarityGlowImage->SetActive(false);
        countTextLabel->SetActive(false);
    }

    bool ItemSlot::HasItem() const
    {
        return m_hasItem;
    }

    size_t ItemSlot::GetItemIndex() const
    {
        return m_itemIndex;
    }

    void ItemSlot::SetBackgroundImageActive(bool active)
    {
        if (auto* backgroundImage = FindWidgetById<core::Image>(m_backgroundImageId))
            backgroundImage->SetActive(active);
    }

    void ItemSlot::SetCountTextActive(bool active)
    {
        m_countTextActive = active;

        if (auto* countTextLabel = FindWidgetById<core::TextLabel>(m_countTextLabelId))
            countTextLabel->SetActive(active);
    }

    void ItemSlot::SetSelectMode(SelectMode selectMode)
    {
        m_selectMode = selectMode;

        auto* selectImage = FindWidgetById<core::Image>(m_selectImageId);
        if (!selectImage)
            return;

        switch (m_selectMode)
        {
        case SelectMode::None:
            selectImage->SetActive(false);
            break;

        case SelectMode::Hovered:
            selectImage->SetOutlineColor(sf::Color::White);
            selectImage->SetActive(true);
            break;

        case SelectMode::Selected:
            selectImage->SetOutlineColor(sf::Color(255u, 255u, 255u, 150u));
            selectImage->SetActive(true);
            break;
        }
    }
}