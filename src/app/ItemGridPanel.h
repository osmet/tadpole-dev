#pragma once

#include "../core/GridPanel.h"
#include "../core/Button.h"

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
    class ItemGridPanel : public core::GridPanel
    {
    public:
        ItemGridPanel(core::AssetManager& assetManager,
            size_t columnCount, size_t rowCount, float cellSize, float spacing);

        void SetItems(const std::vector<const app_domain::Item*>& items);

    private:
        core::AssetManager& m_assetManager;

    private:
        class ItemSlot final : public core::Button
        {
        public:
            ItemSlot(core::AssetManager& assetManager, float cellSize);

            void SetItem(const app_domain::Item& item);
            void ClearItem();

        private:
            core::AssetManager& m_assetManager;

            core::Image* m_rarityGlowImage = nullptr;
            core::Image* m_iconImage = nullptr;
            core::Image* m_frameImage = nullptr;
            core::TextLabel* m_countTextLabel = nullptr;
            core::Image* m_selectImage = nullptr;
        };
    };
}


