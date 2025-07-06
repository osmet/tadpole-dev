#include "Precompiled.h"
#include "ItemTransferPanel.h"
#include "../core/AssetManager.h"
#include "../core/Image.h"
#include "../core/TextLabel.h"
#include "../core/Button.h"
#include "../app_domain/Item.h"
#include "../app_domain/InventoryItemDetails.h"
#include "ItemSlot.h"
#include "ScreenOverlayPanel.h"

namespace app
{
    ItemTransferPanel::ItemTransferPanel(core::AssetManager& assetManager, const sf::Vector2f& renderWindowSize)
    {
        SetAnchor(0.5f, 0.5f);
        SetPivot(0.5f, 0.5f);
        SetActive(false);

        auto* screenOverlayImage = CreateWidget<ScreenOverlayPanel>(renderWindowSize);

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        auto* backgroundImage = CreateWidget<core::Image>();
        backgroundImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
        backgroundImage->SetOutlineThickness(2.f);
        backgroundImage->SetOutlineColor(sf::Color(153u, 117u, 92u, 255u));
        backgroundImage->SetSize(540.f, 155.f);

        auto* itemSlot = backgroundImage->CreateWidget<ItemSlot>(assetManager, 88.f);
        itemSlot->SetAnchor(0.5f, 0.f);
        itemSlot->SetPivot(0.5f, 0.5f);
        itemSlot->SetLocalPosition(0.f, 0.f);
        itemSlot->SetBackgroundImageActive(true);
        itemSlot->SetCountTextActive(false);
        m_itemSlot = itemSlot;

        auto* itemNameTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        itemNameTextLabel->SetAnchor(0.5f, 0.f);
        itemNameTextLabel->SetPivot(0.5f, 0.f);
        itemNameTextLabel->SetLocalPosition(0.f, 55.f);
        itemNameTextLabel->SetFont(regularFont);
        itemNameTextLabel->SetFontSize(22u);
        itemNameTextLabel->SetText("Name");
        m_itemNameTextLabel = itemNameTextLabel;

        auto* itemCountTextLabel = backgroundImage->CreateWidget<core::TextLabel>();
        itemCountTextLabel->SetAnchor(0.5f, 0.f);
        itemCountTextLabel->SetPivot(0.5f, 0.5f);
        itemCountTextLabel->SetLocalPosition(0.f, 86.f + 18.f - 1.f);
        itemCountTextLabel->SetFont(regularFont);
        itemCountTextLabel->SetFontSize(22u);
        itemCountTextLabel->SetColor(sf::Color(222u, 214u, 203u, 255u));
        itemCountTextLabel->SetText("1/5");
        m_itemCountTextLabel = itemCountTextLabel;

        auto* decrementCountButton = backgroundImage->CreateWidget<core::Button>();
        decrementCountButton->SetSize(34.f, 34.f);
        decrementCountButton->SetLocalPosition(-78.f, 86.f);
        decrementCountButton->SetAnchor(0.5f, 0.f);
        decrementCountButton->SetPivot(0.5f, 0.f);
        decrementCountButton->SetColor(sf::Color(64u, 55u, 48u, 255u));
        decrementCountButton->SetOnClick([this]()
        {
            if (m_currentItemCount > m_minItemCount)
            {
                --m_currentItemCount;
                UpdateItemCountWidgets();
            }
        });
        {
            auto* textLabel = decrementCountButton->CreateWidget<core::TextLabel>();
            textLabel->SetFont(regularFont);
            textLabel->SetFontSize(20u);
            textLabel->SetText("-");
        }
        m_decrementCountButton = decrementCountButton;

        auto* incrementCountButton = backgroundImage->CreateWidget<core::Button>();
        incrementCountButton->SetSize(34.f, 34.f);
        incrementCountButton->SetLocalPosition(78.f, 86.f);
        incrementCountButton->SetAnchor(0.5f, 0.f);
        incrementCountButton->SetPivot(0.5f, 0.f);
        incrementCountButton->SetColor(sf::Color(64u, 55u, 48u, 255u));
        incrementCountButton->SetOnClick([this]()
        {
            if (m_currentItemCount < m_maxItemCount)
            {
                ++m_currentItemCount;
                UpdateItemCountWidgets();
            }
        });
        {
            auto* textLabel = incrementCountButton->CreateWidget<core::TextLabel>();
            textLabel->SetFont(regularFont);
            textLabel->SetFontSize(20u);
            textLabel->SetText("+");
        }
        m_incrementCountButton = incrementCountButton;

        auto* confirmButton = backgroundImage->CreateWidget<core::Button>();
        confirmButton->SetAnchor(0.5f, 1.f);
        confirmButton->SetPivot(0.5f, 0.5f);
        confirmButton->SetLocalPosition(100.f, 0.f);
        confirmButton->SetSize(160.f, 34.f);
        confirmButton->SetColor(sf::Color(48u, 58u, 64u, 255u));
        confirmButton->SetOnClick([this]()
        {
            SetActive(false);

            if (m_onConfirm)
                m_onConfirm(m_currentItemCount);
        });

        auto confirmText = confirmButton->CreateWidget<core::TextLabel>();
        confirmText->SetFont(regularFont);
        confirmText->SetFontSize(18u);
        confirmText->SetText("Confirm");

        auto* cancelButton = backgroundImage->CreateWidget<core::Button>();
        cancelButton->SetAnchor(0.5f, 1.f);
        cancelButton->SetPivot(0.5f, 0.5f);
        cancelButton->SetLocalPosition(-100.f, 0.f);
        cancelButton->SetSize(160.f, 34.f);
        cancelButton->SetColor(sf::Color(64u, 55u, 48u, 255u));
        cancelButton->SetOnClick([this]()
        {
            SetActive(false);

            if (m_onCancel)
                m_onCancel();
        });

        auto cancelText = cancelButton->CreateWidget<core::TextLabel>();
        cancelText->SetFont(regularFont);
        cancelText->SetFontSize(18u);
        cancelText->SetText("Cancel");
    }

    void ItemTransferPanel::Show(const app_domain::InventoryItemDetails& item)
    {
        m_itemIndex = item.GetIndex();
        m_minItemCount = 1;
        m_maxItemCount = item.GetCount();

        m_currentItemCount = m_maxItemCount / 2u;
        UpdateItemCountWidgets();

        if (!m_itemSlot || !m_itemNameTextLabel)
            return;

        m_itemSlot->SetItem(item);
        m_itemNameTextLabel->SetText(item.GetItem().Name);

        SetActive(true);
    }

    void ItemTransferPanel::SetOnConfirm(OnConfirm callback)
    {
        m_onConfirm = std::move(callback);
    }

    void ItemTransferPanel::SetOnCancel(OnCancel callback)
    {
        m_onCancel = std::move(callback);
    }

    void ItemTransferPanel::UpdateItemCountWidgets()
    {
        if (m_itemCountTextLabel)
            m_itemCountTextLabel->SetText(std::to_string(m_currentItemCount) + " / " + std::to_string(m_maxItemCount));

        if (m_decrementCountButton)
            m_decrementCountButton->SetInteractable(m_currentItemCount > m_minItemCount);

        if (m_incrementCountButton)
            m_incrementCountButton->SetInteractable(m_currentItemCount < m_maxItemCount);
    }
}