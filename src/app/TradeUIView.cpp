#include "Precompiled.h"
#include "TradeUIView.h"

#include "AppContext.h"
#include "../core/CanvasPanel.h"
#include "../core/StackPanel.h"
#include "../core/Image.h"
#include "../core/Button.h"
#include "../core/TextLabel.h"
#include "../core/GridPanel.h"
#include "ItemGridPanel.h"
#include "ItemPanel.h"
#include "TradeUIViewModel.h"
#include "TooltipPanel.h"
#include "ErrorPanel.h"

namespace app
{
    TradeUIView::TradeUIView(AppContext& appContext, TradeUIViewModel& viewModel)
        : m_appContext(appContext)
        , m_viewModel(viewModel)
        , m_itemDragSystem(appContext.GetAssetManager(), appContext.GetRenderWindow(), 50.f)
    {
    }

    void TradeUIView::Initialize()
    {
        auto& assetManager = m_appContext.GetAssetManager();

        auto renderWindowSize = m_appContext.GetRenderWindowSize();

        auto& regularFont = assetManager.GetFont("Mignon_Regular");

        SetAnchor(0.f, 0.f);
        SetPivot(0.f, 0.f);
        SetSize(renderWindowSize);

        {
            {
                auto* tradeTextLabel = CreateWidget<core::TextLabel>();
                tradeTextLabel->SetAnchor(.5f, 0.f);
                tradeTextLabel->SetPivot(.5f, 0.f);
                tradeTextLabel->SetLocalPosition(0.f, 24.f);
                tradeTextLabel->SetFont(regularFont);
                tradeTextLabel->SetFontSize(28u);
                tradeTextLabel->SetText("Trade");
                tradeTextLabel->SetColor(sf::Color(191u, 163u, 143u, 255u));
            }

            {
                std::vector<ItemFilterPanel::ItemFilterDescriptor> itemFilterDescriptors = {
                    { app_domain::ItemCategory::All, "All", "UI_Icon_Filter_All" },
                    { app_domain::ItemCategory::Equipment, "Equipment", "UI_Icon_Filter_Equipment" },
                    { app_domain::ItemCategory::BooksAndKeys, "Books and Keys", "UI_Icon_Filter_BooksAndKeys" },
                    { app_domain::ItemCategory::Consumables, "Consumables", "UI_Icon_Filter_Consumables" },
                    { app_domain::ItemCategory::ScrollsAndTools, "Scrolls and Tools", "UI_Icon_Filter_ScrollsAndTools" },
                    { app_domain::ItemCategory::Misc, "Miscellaneous", "UI_Icon_Filter_Misc" },
                };

                auto* itemFilterPanel = CreateWidget<ItemFilterPanel>(assetManager, itemFilterDescriptors);
                itemFilterPanel->SetAnchor(.5f, 0.f);
                itemFilterPanel->SetPivot(.5f, 0.f);
                itemFilterPanel->SetLocalPosition(0.f, 135.f);
                m_itemFilterPanelId = itemFilterPanel->GetId();
            }

            {
                auto* playerCharacterInfoPanel = CreateWidget<CharacterInfoPanel>(assetManager, false);
                m_playerCharacterInfoPanelId = playerCharacterInfoPanel->GetId();

                auto* traderCharacterInfoPanel = CreateWidget<CharacterInfoPanel>(assetManager, true);
                m_traderCharacterInfoPanelId = traderCharacterInfoPanel->GetId();
            }

            {
                size_t columnCount = 7;
                size_t rowCount = 12;

                float cellSize = 54.f;
                float spacing = 4.f;

                auto* playerItemGrid = CreateWidget<ItemGridPanel>(assetManager, columnCount, rowCount, cellSize, spacing);
                playerItemGrid->SetAnchor(0.5f, 0.f);
                playerItemGrid->SetPivot(1.f, 0.f);
                playerItemGrid->SetLocalPosition(-40.f, 215.f);
                m_playerItemGridId = playerItemGrid->GetId();

                auto* traderItemGrid = CreateWidget<ItemGridPanel>(assetManager, columnCount, rowCount, cellSize, spacing);
                traderItemGrid->SetAnchor(0.5f, 0.f);
                traderItemGrid->SetPivot(0.f, 0.f);
                traderItemGrid->SetLocalPosition(40.f, 215.f);
                m_traderItemGridId = traderItemGrid->GetId();
            }

            {
                std::vector<ItemSortPanel::ItemSortDescriptor> itemSortDescriptors = {
                  { app_domain::ItemSortMode::Latest, "Latest" },
                  { app_domain::ItemSortMode::Type, "Type" },
                  { app_domain::ItemSortMode::Value, "Value" },
                  { app_domain::ItemSortMode::Weight, "Weight" },
                };

                auto* itemSortPanel = CreateWidget<ItemSortPanel>(assetManager, itemSortDescriptors);
                itemSortPanel->SetAnchor(.5f, 1.f);
                itemSortPanel->SetPivot(0.f, 1.f);
                itemSortPanel->SetLocalPosition(-492.f, -95.f);
                m_itemSortPanelId = itemSortPanel->GetId();
            }

            {
                auto* tradeButton = CreateWidget<core::Button>();
                tradeButton->SetAnchor(0.5f, 1.f);
                tradeButton->SetPivot(0.5f, 1.f);
                tradeButton->SetLocalPosition(0.f, -35.f);
                tradeButton->SetSize(250.f, 40.f);
                tradeButton->SetColor(sf::Color(48, 58, 64));
                tradeButton->SetOnClick([onTradeButtonClick = m_onTradeButtonClick]() { if (onTradeButtonClick) onTradeButtonClick(); });
                {
                    auto* textLabel = tradeButton->CreateWidget<core::TextLabel>();
                    textLabel->SetLocalPosition(0.f, 0.f);
                    textLabel->SetFont(regularFont);
                    textLabel->SetFontSize(18);
                    textLabel->SetText("TRADE");
                    textLabel->SetColor(sf::Color::White);
                }
                tradeButton->SetActive(false);
            }

            {
                auto* itemPanel = CreateWidget<ItemPanel>(assetManager);
                m_itemPanelId = itemPanel->GetId();
            }

            {
                auto* tooltipPanel = CreateWidget<TooltipPanel>(assetManager);

                {
                    auto* tooltipPanel = CreateWidget<TooltipPanel>(assetManager);

                    if (auto* itemFilterPanel = FindWidgetById<ItemFilterPanel>(m_itemFilterPanelId))
                        itemFilterPanel->SetTooltipPanel(tooltipPanel);

                    if (auto* itemSortPanel = FindWidgetById<ItemSortPanel>(m_itemSortPanelId))
                        itemSortPanel->SetTooltipPanel(tooltipPanel);
                }
            }

            {
                auto* itemTransferPanel = CreateWidget<ItemTransferPanel>(assetManager, renderWindowSize);
                m_itemTransferPanelId = itemTransferPanel->GetId();
            }

            {
                auto* errorPanel = CreateWidget<ErrorPanel>(assetManager, renderWindowSize);
                m_errorPanelId = errorPanel->GetId();
            }
        }

        BindViewModel();
    }

    bool TradeUIView::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        if (auto* errorPanel = FindWidgetById<ErrorPanel>(m_errorPanelId); 
            errorPanel && errorPanel->IsActiveSelf())
        {
            return errorPanel->HandleEvent(event, renderWindow);
        }
        else if (auto* itemTransferPanel = FindWidgetById<ItemTransferPanel>(m_itemTransferPanelId);
            itemTransferPanel && itemTransferPanel->IsActiveSelf())
        {
            return itemTransferPanel->HandleEvent(event, renderWindow);
        }
        else if (m_itemDragSystem.IsActive())
        {
            return m_itemDragSystem.HandleEvent(event, renderWindow);
        }

        return UIView::HandleEvent(event, renderWindow);
    }


    void TradeUIView::Render(sf::RenderWindow& renderWindow)
    {
        UIView::Render(renderWindow);

        m_itemDragSystem.Render(renderWindow);
    }
    
    void TradeUIView::BindViewModel()
    {
        auto& viewModelContext = m_viewModel.GetContext();

        viewModelContext.PlayerCharacterName.Subscribe([this](const auto& value)
        {
            SetPlayerName(value);
        });

        viewModelContext.TraderCharacterName.Subscribe([this](const auto& value)
        {
            SetTraderName(value);
        });

        viewModelContext.PlayerPortraitTextureId.Subscribe([this](const auto& value)
        {
            SetPlayerPortraitTexture(value);
        });

        viewModelContext.TraderPortraitTextureId.Subscribe([this](const auto& value)
        {
            SetTraderPortraitTexture(value);
        });

        viewModelContext.PlayerCurrentMoney.Subscribe([this](const auto& value)
        {
            SetPlayerMoney(value);
        });

        viewModelContext.TraderCurrentMoney.Subscribe([this](const auto& value)
        {
            SetTraderMoney(value);
        });

        viewModelContext.PlayerCurrentWeight.Subscribe([this](const auto& value)
        {
            SetPlayerWeight(value, m_viewModel.GetContext().PlayerMaxWeight.GetValue());
        });

        viewModelContext.PlayerItems.Subscribe([this](const auto& value)
        {
            SetPlayerItems(value);
        });

        viewModelContext.TraderItems.Subscribe([this](const auto& value)
        {
            SetTraderItems(value);
        });

        m_viewModel.SetOnShowTransferPanel([this](const app_domain::InventoryItemDetails& item, TradeUIViewModel::OnTransferPanelConfirm onConfirm)
        {
            if (auto* itemTransferPanel = FindWidgetById<ItemTransferPanel>(m_itemTransferPanelId))
            {
                itemTransferPanel->SetOnConfirm(std::move(onConfirm));
                itemTransferPanel->Show(item);
            }
        });

        m_viewModel.SetOnTradeError([this](const app_domain::TradeError& error)
        {
            ShowErrorPanel(error);
        });

        if (auto* playerItemGrid = FindWidgetById<ItemGridPanel>(m_playerItemGridId))
        {
            BindItemGridPanel(playerItemGrid, false);

            m_itemDragSystem.AddDragTarget(playerItemGrid);
        }

        if (auto* traderItemGrid = FindWidgetById<ItemGridPanel>(m_traderItemGridId))
        {
            BindItemGridPanel(traderItemGrid, true);

            m_itemDragSystem.AddDragTarget(traderItemGrid);
        }

        if (auto* itemFilterPanel = FindWidgetById<ItemFilterPanel>(m_itemFilterPanelId))
        {
            itemFilterPanel->SetOnFilterButtonClick([this](app_domain::ItemCategory itemCategory)
            {
                m_viewModel.GetContext().ItemFilterCategory.SetValue(itemCategory);
            });
        }

        if (auto* itemSortPanel = FindWidgetById<ItemSortPanel>(m_itemSortPanelId); itemSortPanel)
        {
            itemSortPanel->SetOnSortButtonClick([this](app_domain::ItemSortMode itemSortMode)
            {
                m_viewModel.GetContext().ItemSortMode.SetValue(itemSortMode);
            });
        }
    }

    void TradeUIView::BindItemGridPanel(ItemGridPanel* gridPanel, bool isBuying)
    {
        if (!gridPanel)
            return;
     
        gridPanel->SetOnItemClick([this, isBuying](std::size_t itemIndex)
        {
            HideItemPanel();

            m_viewModel.TradeItem(isBuying, itemIndex);
        });

        gridPanel->SetOnItemHoverIn([this, isBuying](std::size_t itemIndex, const sf::Vector2f& position)
        {
            auto itemOpt = isBuying
                ? m_viewModel.GetTraderItem(itemIndex)
                : m_viewModel.GetPlayerItem(itemIndex);
            if (!itemOpt.has_value())
                return;

            ShowItemPanel(itemOpt.value().GetItem(), position);
        });

        gridPanel->SetOnItemHoverOut([this]()
        {
            HideItemPanel();
        });

        gridPanel->SetOnItemDragBegin([this, isBuying](std::size_t itemIndex)
        {
            HideItemPanel();

            auto itemOpt = isBuying
                ? m_viewModel.GetTraderItem(itemIndex)
                : m_viewModel.GetPlayerItem(itemIndex);
            if (!itemOpt.has_value())
                return;

            const auto& item = itemOpt.value();

            m_itemDragSystem.BeginDrag(item.GetIndex(), item.GetItem().IconTextureId, [this, isBuying](IItemDragTarget* target, std::size_t fromItemIndex, std::int32_t signedToItemIndex)
            {
                if (!target)
                    return;

                if (target == FindWidgetById<ItemGridPanel>(m_playerItemGridId))
                {
                    if (isBuying)
                        m_viewModel.TradeItem(isBuying, fromItemIndex);
                    else
                        m_viewModel.StackItem(fromItemIndex, signedToItemIndex);
                }
                else if (target == FindWidgetById<ItemGridPanel>(m_traderItemGridId))
                {
                    if (!isBuying)
                        m_viewModel.TradeItem(isBuying, fromItemIndex);
                }
            });
        });
    }

    void TradeUIView::SetOnTradeButtonClick(std::function<void()> callback)
    {
        m_onTradeButtonClick = std::move(callback);
    }

    void TradeUIView::SetPlayerPortraitTexture(const std::string& textureId)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_playerCharacterInfoPanelId))
            characterInfoPanel->SetPortraitTexture(textureId);
    }

    void TradeUIView::SetPlayerName(const std::string& name)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_playerCharacterInfoPanelId))
            characterInfoPanel->SetCharacterName(name);
    }

    void TradeUIView::SetPlayerMoney(uint32_t money)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_playerCharacterInfoPanelId))
            characterInfoPanel->SetMoney(money);
    }

    void TradeUIView::SetPlayerWeight(float currentWeight, float maxWeight)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_playerCharacterInfoPanelId))
            characterInfoPanel->SetWeight(currentWeight, maxWeight);
    }

    void TradeUIView::SetTraderPortraitTexture(const std::string& textureId)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_traderCharacterInfoPanelId))
            characterInfoPanel->SetPortraitTexture(textureId);
    }

    void TradeUIView::SetTraderName(const std::string& name)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_traderCharacterInfoPanelId))
            characterInfoPanel->SetCharacterName(name);
    }

    void TradeUIView::SetTraderMoney(uint32_t money)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_traderCharacterInfoPanelId))
            characterInfoPanel->SetMoney(money);
    }

    void TradeUIView::SetTraderWeight(float currentWeight, float maxWeight)
    {
        if (auto* characterInfoPanel = FindWidgetById<CharacterInfoPanel>(m_traderCharacterInfoPanelId))
            characterInfoPanel->SetWeight(currentWeight, maxWeight);
    }

    void TradeUIView::SetPlayerItems(const std::vector<app_domain::InventoryItemDetails>& items)
    {
        if (auto* itemGridPanel = FindWidgetById<ItemGridPanel>(m_playerItemGridId))
            itemGridPanel->SetItems(items);
    }

    void TradeUIView::SetTraderItems(const std::vector<app_domain::InventoryItemDetails>& items)
    {
        if (auto* itemGridPanel = FindWidgetById<ItemGridPanel>(m_traderItemGridId))
            itemGridPanel->SetItems(items);
    }

    void TradeUIView::ShowItemPanel(const app_domain::Item& item, const sf::Vector2f& position) const
    {
        if (auto* itemPanel = FindWidgetById<ItemPanel>(m_itemPanelId))
            itemPanel->Show(item, position, sf::Vector2f(60.f, 60.f), m_appContext.GetRenderWindowSize().y);
    }

    void TradeUIView::HideItemPanel() const
    {
        if (auto* itemPanel = FindWidgetById<ItemPanel>(m_itemPanelId))
            itemPanel->Hide();
    }

    void TradeUIView::ShowErrorPanel(app_domain::TradeError error) const
    {
        auto* errorPanel = FindWidgetById<ErrorPanel>(m_errorPanelId);
        if (!errorPanel)
            return;

        using app_domain::TradeError;

        switch (error)
        {
        case TradeError::NotEnoughMoney:
            errorPanel->Show(
                "Insufficient Gold",
                "A character has insufficient gold to complete this transaction."
            );
            break;
        default:
            break;
        }
    }

    TradeUIView::CharacterInfoPanel::CharacterInfoPanel(core::AssetManager& assetManager, bool alignRight)
        : m_assetManager(assetManager)
    {
        auto& font = assetManager.GetFont("Mignon_Regular");
        auto& portraitFrameTexture = assetManager.GetTexture("UI_Panel_Portrait_Frame");
        auto& moneyIconTexture = assetManager.GetTexture("UI_Icon_Coin");
        auto& weightIconTexture = assetManager.GetTexture("UI_Icon_Weight");

        sf::Color portraitFrameColor(153u, 117u, 92u, 255u);
        float portraitFrameOutlineThickness = 3.f;
        sf::Vector2f portraitSize(72.f, 105.f);
        float portraitPositionX = alignRight ? 250.f : -250.f;
        float namePositionX = alignRight ? 40.f : -440.f;
        float moneyPositionX = alignRight ? portraitPositionX + portraitSize.x - 20.f : portraitPositionX - 20.f;
        float weightPositionX = alignRight ? 40.f : -40.f;
        float valuesPositionY = 193.f;

        SetAnchor(.5f, 0.f);
        SetPivot(.5f, 0.f);

        auto* portraitImage = CreateWidget<core::Image>();
        portraitImage->SetAnchor(.5f, 0.f);
        portraitImage->SetPivot(alignRight ? 0.f : 1.f, 0.f);
        portraitImage->SetLocalPosition(portraitPositionX, 75.f);
        portraitImage->SetSize(portraitSize);
        {
            auto* frameImage = portraitImage->CreateWidget<core::Image>();
            frameImage->SetSize(portraitSize);
            frameImage->SetTexture(portraitFrameTexture);
            frameImage->SetColor(portraitFrameColor);
        }
        m_portraitImage = portraitImage;

        auto* nameTextLabel = CreateWidget<core::TextLabel>();
        nameTextLabel->SetAnchor(.5f, 0.f);
        nameTextLabel->SetPivot(0.f, 0.f);
        nameTextLabel->SetLocalPosition(namePositionX, valuesPositionY);
        nameTextLabel->SetFont(font);
        nameTextLabel->SetFontSize(20u);
        m_nameTextLabel = nameTextLabel;

        auto* moneyTextLabel = CreateWidget<core::TextLabel>();
        moneyTextLabel->SetAnchor(.5f, 0.f);
        moneyTextLabel->SetPivot(1.f, 0.f);
        moneyTextLabel->SetLocalPosition(moneyPositionX, valuesPositionY);
        moneyTextLabel->SetFont(font);
        moneyTextLabel->SetFontSize(20u);
        m_moneyTextLabel = moneyTextLabel;

        auto* moneyIconImage = CreateWidget<core::Image>();
        moneyIconImage->SetAnchor(.5f, 0.f);
        moneyIconImage->SetPivot(1.f, 0.f);
        moneyIconImage->SetLocalPosition(moneyPositionX + 20.f, valuesPositionY);
        moneyIconImage->SetSize(12.f, 14.f);
        moneyIconImage->SetColor(sf::Color(191u, 163u, 143u, 255u));
        moneyIconImage->SetTexture(moneyIconTexture);

        if (!alignRight)
        {
            auto* weightTextLabel = CreateWidget<core::TextLabel>();
            weightTextLabel->SetAnchor(.5f, 0.f);
            weightTextLabel->SetPivot(1.f, 0.f);
            weightTextLabel->SetLocalPosition(weightPositionX - 22.f, valuesPositionY);
            weightTextLabel->SetFont(font);
            weightTextLabel->SetFontSize(20u);
            m_weightTextLabel = weightTextLabel;

            auto* weightIconImage = CreateWidget<core::Image>();
            weightIconImage->SetAnchor(.5f, 0.f);
            weightIconImage->SetPivot(1.f, 0.f);
            weightIconImage->SetLocalPosition(weightPositionX, valuesPositionY - 2.f);
            weightIconImage->SetSize(15.f);
            weightIconImage->SetTexture(weightIconTexture);
        }
    }

    void TradeUIView::CharacterInfoPanel::SetCharacterName(const std::string& name)
    {
        if (m_nameTextLabel)
            m_nameTextLabel->SetText(name);
    }

    void TradeUIView::CharacterInfoPanel::SetPortraitTexture(const std::string& textureId)
    {
        if (m_portraitImage)
            m_portraitImage->SetTexture(m_assetManager.GetTexture(textureId));
    }

    void TradeUIView::CharacterInfoPanel::SetMoney(uint32_t money)
    {
        if (m_moneyTextLabel)
            m_moneyTextLabel->SetText(std::to_string(money));
    }

    void TradeUIView::CharacterInfoPanel::SetWeight(float currentWeight, float maxWeight)
    {
        if (m_weightTextLabel)
        {
            m_weightTextLabel->SetColor(currentWeight > maxWeight ? sf::Color::Red : sf::Color::White);

            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%.1f/%d", currentWeight, (std::uint32_t)maxWeight);

            m_weightTextLabel->SetText(buffer);
        }
    }

    TradeUIView::ItemFilterPanel::ItemFilterDescriptor::ItemFilterDescriptor(app_domain::ItemCategory itemCategory, std::string name,
        std::string textureId)
        : ItemCategory(itemCategory), Name(std::move(name)), TextureId(std::move(textureId))
    {
    }

    TradeUIView::ItemFilterPanel::ItemFilterPanel(core::AssetManager& assetManager,
        const std::vector<ItemFilterDescriptor>& itemFilterDescriptors)
    {
        std::size_t buttonCount = itemFilterDescriptors.size();

        if (buttonCount == 0u)
        {
            SetActive(false);
            return;
        }

        float buttonSize = 35.f;
        float spacing = 10.f;

        SetSpacing(spacing);
        Reserve(buttonCount);

        auto& filterButtonTexture = assetManager.GetTexture("UI_Panel_Circle");
        sf::Color filterButtonNormalColor(0u, 0u, 0u, 50u);
        sf::Color filterButtonHoveredColor(128u, 128u, 128u, 50u);
        sf::Color filterButtonPressedColor(255u, 255u, 255u, 50u);
        sf::Color filterButtonDisabledColor(255u, 255u, 255u, 50u);
        sf::Color filterButtonFrameColor(128u, 107u, 95u, 255u);
        auto& filterButtonFrameTexture = assetManager.GetTexture("UI_Panel_Circle_Frame");

        for (const auto& itemFilterDescriptor : itemFilterDescriptors)
        {
            auto itemCategory = itemFilterDescriptor.ItemCategory;
            auto& name = itemFilterDescriptor.Name;
            auto& textureId = itemFilterDescriptor.TextureId;

            auto& texture = assetManager.GetTexture(textureId);

            auto* filterButton = CreateWidget<core::Button>();
            filterButton->SetAnchor(0.f, 0.f);
            filterButton->SetPivot(0.f, .5f);
            filterButton->SetSize(buttonSize);
            filterButton->SetTexture(filterButtonTexture);
            filterButton->SetNormalColor(filterButtonNormalColor);
            filterButton->SetHoveredColor(filterButtonHoveredColor);
            filterButton->SetPressedColor(filterButtonPressedColor);
            filterButton->SetDisabledColor(filterButtonDisabledColor);
            filterButton->SetOnClick([this, filterButton, itemCategory]()
            {
                for (auto* button : m_filterButtons)
                {
                    if (button)
                        button->SetInteractable(true);
                }

                if (filterButton)
                    filterButton->SetInteractable(false);

                if (m_onFilterButtonClick)
                    m_onFilterButtonClick(itemCategory);

            });
            filterButton->SetOnHoverIn([this, filterButton, name](const sf::Vector2f& mousePosition)
            {
                if (m_tooltipPanel && filterButton)
                    m_tooltipPanel->Show(name, filterButton->GetPosition(), sf::Vector2f(6.f, -12.f));
            });
            filterButton->SetOnHoverOut([this]
            {
                if (m_tooltipPanel)
                    m_tooltipPanel->Hide();
            });
            {
                auto* itemCategoryImage = filterButton->CreateWidget<core::Image>();
                itemCategoryImage->SetSize(25.f);
                itemCategoryImage->SetTexture(texture);

                auto* frameImage = filterButton->CreateWidget<core::Image>();
                frameImage->SetSize(buttonSize);
                frameImage->SetTexture(filterButtonFrameTexture);
                frameImage->SetColor(filterButtonFrameColor);
            }
            m_filterButtons.push_back(filterButton);
        }

        if (m_filterButtons[0])
            m_filterButtons[0]->SetInteractable(false);
    }

    void TradeUIView::ItemFilterPanel::SetTooltipPanel(TooltipPanel* tooltipPanel)
    {
        m_tooltipPanel = tooltipPanel;
    }

    void TradeUIView::ItemFilterPanel::SetOnFilterButtonClick(OnFilterButtonClick callback)
    {
        m_onFilterButtonClick = std::move(callback);
    }

    TradeUIView::ItemSortPanel::ItemSortPanel(core::AssetManager& assetManager,
        const std::vector<ItemSortDescriptor>& itemSortDescriptors)
    {
        std::size_t buttonCount = itemSortDescriptors.size();
        if (buttonCount == 0u)
        {
            SetActive(false);
            return;
        }

        float buttonSize = 35.f;
        float spacing = 10.f;
        auto& regularFont = assetManager.GetFont("Mignon_Regular");
        auto& sortIconTexture = assetManager.GetTexture("UI_Icon_Sort");
        float outlineThickness = 2.f;
        float panelWidth = 190.f;

        sf::Color frameColor(sf::Color(191u, 163u, 143u, 128u));

        core::TextLabel* sortByButtonTextLabel = nullptr;

        auto* sortByButton = CreateWidget<core::Button>();
        sortByButton->SetAnchor(.5f, 1.f);
        sortByButton->SetPivot(0.f, 1.f);
        sortByButton->SetSize(panelWidth, 32.f);
        sortByButton->SetColor(sf::Color::Transparent);
        sortByButton->SetOnClick([this] { ToggleItemSortButtonsPanel(); });
        sortByButton->SetOnHoverIn([this, sortByButton](const sf::Vector2f& mousePosition)
        {
            if (m_tooltipPanel && sortByButton)
                m_tooltipPanel->Show("Sort By", sf::Vector2f(mousePosition.x, sortByButton->GetPosition().y), sf::Vector2f(6.f, 24.f + sortByButton->GetSize().y));
        });
        sortByButton->SetOnHoverOut([this]
        {
            if (m_tooltipPanel)
                m_tooltipPanel->Hide();
        });
        {
            auto* iconImage = sortByButton->CreateWidget<core::Image>();
            iconImage->SetAnchor(0.f, .5f);
            iconImage->SetPivot(0.f, .5f);
            iconImage->SetLocalPosition(10.f, 0.f);
            iconImage->SetSize(16.f);
            iconImage->SetTexture(sortIconTexture);

            auto* textLabel = sortByButton->CreateWidget<core::TextLabel>();
            textLabel->SetAnchor(0.f, .5f);
            textLabel->SetPivot(0.f, .5f);
            textLabel->SetLocalPosition(33.f, -1.f);
            textLabel->SetFont(regularFont);
            textLabel->SetFontSize(18u);
            textLabel->SetText(itemSortDescriptors[0].second);
            sortByButtonTextLabel = textLabel;

            //auto& frameTexture = assetManager.GetTexture("UI_Panel_SortByButton_Frame");

            auto* frameImage = sortByButton->CreateWidget<core::Image>();
            frameImage->SetSize(panelWidth - outlineThickness * 2.f, sortByButton->GetSize().y - outlineThickness * 2.f);
            //frameImage->SetTexture(frameTexture);
            //frameImage->SetColor(frameColor);
            frameImage->SetColor(sf::Color::Transparent);
            frameImage->SetOutlineThickness(outlineThickness);
            frameImage->SetOutlineColor(frameColor);
        }

        sf::Vector2f itemSortPanelSize(panelWidth, 165.f);
        sf::Vector2f itemSortPanelPosition(0.f, -sortByButton->GetSize().y - 12.f);
        //auto& itemSortPanelTexture = assetManager.GetTexture("UI_Panel_ItemSort");
        //auto& itemSortPanelFrameTexture = assetManager.GetTexture("UI_Panel_ItemSort_Frame");

        auto* itemSortPanelImage = CreateWidget<core::Image>();
        itemSortPanelImage->SetAnchor(.5f, 1.f);
        itemSortPanelImage->SetPivot(0.f, 1.f);
        itemSortPanelImage->SetLocalPosition(itemSortPanelPosition);
        itemSortPanelImage->SetSize(itemSortPanelSize);
        //itemSortPanelImage->SetTexture(itemSortPanelTexture);
        itemSortPanelImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
        itemSortPanelImage->SetActive(false);
        {
            auto* frameImage = itemSortPanelImage->CreateWidget<core::Image>();
            frameImage->SetSize(itemSortPanelSize.x - outlineThickness * 2.f, itemSortPanelSize.y - outlineThickness * 2.f);
            //frameImage->SetTexture(itemSortPanelFrameTexture);
            //frameImage->SetColor(frameColor);
            frameImage->SetColor(sf::Color::Transparent);
            frameImage->SetOutlineThickness(outlineThickness);
            frameImage->SetOutlineColor(frameColor);

            auto* itemSortButtonsPanel = itemSortPanelImage->CreateWidget<core::StackPanel>();
            itemSortButtonsPanel->SetAnchor(0.f, 0.f);
            itemSortButtonsPanel->SetPivot(0.f, 0.f);
            itemSortButtonsPanel->SetLocalPosition(0.f, 10.f);
            itemSortButtonsPanel->Reserve(itemSortDescriptors.size());
            itemSortButtonsPanel->SetOrientation(core::StackPanel::Orientation::Vertical);
            itemSortButtonsPanel->SetSpacing(0.f);
            itemSortButtonsPanel->SetSizeToContent(false);

            for (const auto& itemSortDescriptor : itemSortDescriptors)
            {
                auto sortMode = itemSortDescriptor.first;
                const auto& sortModeName = itemSortDescriptor.second;

                auto* sortButton = itemSortButtonsPanel->CreateWidget<core::Button>();
                sortButton->SetAnchor(0.f, 0.f);
                sortButton->SetPivot(0.f, 0.f);
                sortButton->SetSize(panelWidth, 35.f);
                sortButton->SetColor(sf::Color::Transparent);
                sortButton->SetOnClick([this, sortMode, sortModeName, sortByButtonTextLabel]()
                {
                    if (sortByButtonTextLabel)
                        sortByButtonTextLabel->SetText(sortModeName);

                    ToggleItemSortButtonsPanel();

                    if (m_onSortButtonClick)
                        m_onSortButtonClick(sortMode);
                });
                {
                    auto* textLabel = sortButton->CreateWidget<core::TextLabel>();
                    textLabel->SetAnchor(0.f, .5f);
                    textLabel->SetPivot(0.f, .5f);
                    textLabel->SetLocalPosition(20.f, 0.f);
                    textLabel->SetFont(regularFont);
                    textLabel->SetFontSize(18u);
                    textLabel->SetText(sortModeName);
                }
            }
        }
        m_itemSortButtonsPanel = itemSortPanelImage;
    }

    void TradeUIView::ItemSortPanel::SetTooltipPanel(TooltipPanel* tooltipPanel)
    {
        m_tooltipPanel = tooltipPanel;
    }

    void TradeUIView::ItemSortPanel::SetOnSortButtonClick(OnSortButtonClick callback)
    {
        m_onSortButtonClick = std::move(callback);
    }

    void TradeUIView::ItemSortPanel::ToggleItemSortButtonsPanel()
    {
        if (m_itemSortButtonsPanel)
            m_itemSortButtonsPanel->SetActive(!m_itemSortButtonsPanel->IsActiveSelf());
    }
}