#pragma once

#include "../core/AssetManager.h"
#include "../app_domain/InventoryItemDetails.h"
#include "../core/Image.h"

namespace app
{
    class IItemDragTarget
    {
    public:
        virtual ~IItemDragTarget() = 0;

        virtual bool CanItemDragEnd(sf::Vector2f dragEndPosition, std::int32_t& out_signedToItemIndex) const = 0;
    };

    class ItemDragSystem
    {
    public:
        using OnItemDragEnd = std::function<void(IItemDragTarget* target, std::size_t fromItemIndex, std::int32_t signedToItemIndex)>;

        ItemDragSystem(core::AssetManager& assetManager, sf::RenderWindow& renderWindow, float iconSize);

        void BeginDrag(std::size_t dragItemIndex, const std::string& itemIconTextureId, OnItemDragEnd onItemDragEnd);
        void EndDrag();
        
        void AddDragTarget(IItemDragTarget* target);

        void HandleEvent(const sf::Event& event, sf::RenderWindow& renderWindow);
        void Render(sf::RenderWindow& renderWindow);

        bool IsActive() const;

    private:
        void UpdateRenderPosition();

        core::AssetManager& m_assetManager;
        sf::RenderWindow& m_renderWindow;

        core::Image m_itemIconImage;

        std::vector<IItemDragTarget*> m_dragTargets;
        std::size_t m_dragItemIndex = 0u;
        bool m_active = false;
        OnItemDragEnd m_onItemDragEnd;
    };
}
