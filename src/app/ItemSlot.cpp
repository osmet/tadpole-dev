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
        SetColor(sf::Color::Transparent);

        auto* backgroundImage = CreateWidget<core::Image>();
        backgroundImage->SetSize(iconSize);
        backgroundImage->SetColor(sf::Color(13u, 11u, 9u, 255u));
        backgroundImage->SetActive(false);
        m_backgroundImage = backgroundImage;

        auto* rarityGlowImage = CreateWidget<core::Image>();
        rarityGlowImage->SetSize(iconSize);
        rarityGlowImage->SetTexture(assetManager.GetTexture("UI_Overlay_ItemSlot_RarityGlow"));
        rarityGlowImage->SetActive(false);
        m_rarityGlowImage = rarityGlowImage;

        auto* iconImage = CreateWidget<core::Image>();
        iconImage->SetSize(iconSize);
        iconImage->SetActive(false);
        m_iconImage = iconImage;

        auto* frameImage = CreateWidget<core::Image>();
        frameImage->SetSize(iconSize);
        frameImage->SetColor(sf::Color::Transparent);
        frameImage->SetOutlineThickness(outlineThickness);
        frameImage->SetOutlineColor(sf::Color(64u, 54u, 48u, 255u));
        m_frameImage = frameImage;

        auto* countTextLabel = CreateWidget<core::TextLabel>();
        countTextLabel->SetAnchor(1.f, 1.f);
        countTextLabel->SetPivot(1.f, 1.f);
        countTextLabel->SetLocalPosition(-4.f, -8.f);
        countTextLabel->SetFont(regularFont);
        countTextLabel->SetFontSize(20);
        countTextLabel->SetText("3");
        countTextLabel->SetOutlineThickness(1.f);
        countTextLabel->SetActive(false);
        m_countTextLabel = countTextLabel;

        auto* selectImage = CreateWidget<core::Image>();
        selectImage->SetSize(iconSize + 2.f);
        selectImage->SetColor(sf::Color::Transparent);
        selectImage->SetOutlineThickness(outlineThickness);
        selectImage->SetOutlineColor(sf::Color::White);
        selectImage->SetActive(false);
        m_selectImage = selectImage;
    }

    void ItemSlot::SetItem(const app_domain::InventoryItemDetails& item)
    {
        m_hasItem = true;
        m_itemIndex = item.GetIndex();

        if (!m_iconImage || !m_rarityGlowImage || !m_countTextLabel)
            return;

        m_iconImage->SetTexture(m_assetManager.GetTexture(item.GetItem().IconTextureId));
        m_iconImage->SetActive(true);

        if (item.GetItem().Rarity != app_domain::ItemRarity::Common)
        {
            m_rarityGlowImage->SetColor(ItemConfig::GetRarityColor(item.GetItem().Rarity));
            m_rarityGlowImage->SetActive(true);
        }
        else
        {
            m_rarityGlowImage->SetActive(false);
        }

        if (m_countTextActive && item.GetCount() > 1u)
        {
            m_countTextLabel->SetText(std::to_string(item.GetCount()));
            m_countTextLabel->SetActive(true);
        }
        else
        {
            m_countTextLabel->SetActive(false);
        }
    }

    void ItemSlot::ItemSlot::ClearItem()
    {
        m_hasItem = false;

        if (!m_iconImage || !m_rarityGlowImage || !m_countTextLabel)
            return;

        m_iconImage->SetActive(false);

        m_rarityGlowImage->SetActive(false);

        m_countTextLabel->SetActive(false);
    }

    bool ItemSlot::ItemSlot::HasItem() const
    {
        return m_hasItem;
    }

    size_t ItemSlot::ItemSlot::GetItemIndex() const
    {
        return m_itemIndex;
    }

    void ItemSlot::SetBackgroundImageActive(bool active)
    {
        if (m_backgroundImage)
            m_backgroundImage->SetActive(active);
    }

    void ItemSlot::SetCountTextActive(bool active)
    {
        m_countTextActive = active;

        if (m_countTextLabel)
            m_countTextLabel->SetActive(active);
    }
}