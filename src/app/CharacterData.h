#pragma once

namespace app
{
    struct CharacterData
    {
        std::string Id;
        std::string Name;
        std::string PortraitTextureId;
        float MaxWeight = 100.f;
        std::string InventoryId;
    };

    using CharacterDataMap = std::unordered_map<std::string, CharacterData>;
}