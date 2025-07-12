#include "Precompiled.h"
#include "ItemDragSystem.h"

#include "../app_domain/Item.h"
#include "../core/AssetManager.h"

namespace app
{
    IItemDragTarget::~IItemDragTarget() = default;

    ItemDragSystem::ItemDragSystem(core::AssetManager& assetManager, sf::RenderWindow& renderWindow, float iconSize)
        : m_assetManager(assetManager)
        , m_renderWindow(renderWindow)
    {
        m_itemIconImage.SetSize(iconSize);
    }

    void ItemDragSystem::BeginDrag(std::size_t itemDrag, const std::string& itemIconTextureId, OnItemDragEnd onItemDragEnd)
    {
        m_dragItemIndex = itemDrag;

        m_active = true;

        m_onItemDragEnd = std::move(onItemDragEnd);

        m_itemIconImage.SetTexture(m_assetManager.GetTexture(itemIconTextureId));

        UpdateRenderPosition();
    }

    void ItemDragSystem::EndDrag()
    {
        if (!IsActive())
            return;

        auto dragEndPosition = m_itemIconImage.GetPosition();

        for (auto* target : m_dragTargets)
        {
            if (!target)
                continue;

            std::int32_t signedToItemIndex;

            if (target->CanItemDragEnd(dragEndPosition, signedToItemIndex))
            {
                if (m_onItemDragEnd)
                    m_onItemDragEnd(target, m_dragItemIndex, signedToItemIndex);

                break;
            }
        }

        m_active = false;
    }

    void ItemDragSystem::AddDragTarget(IItemDragTarget* target)
    {
        if (!target)
            return;

        m_dragTargets.push_back(target);
    }

    bool ItemDragSystem::HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow)
    {
        if (!IsActive())
            return false;

        UpdateRenderPosition();

        if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            EndDrag();
            return false;
        }

        return true;
    }

    void ItemDragSystem::Render(sf::RenderWindow& renderWindow)
    {
        if (!IsActive())
            return;

        m_itemIconImage.Render(renderWindow);
    }

    bool ItemDragSystem::IsActive() const
    {
        return m_active;
    }

    void ItemDragSystem::UpdateRenderPosition()
    {
        auto mousePosition = sf::Mouse::getPosition(m_renderWindow);
        m_itemIconImage.SetLocalPosition(static_cast<sf::Vector2f>(mousePosition));
    }
}