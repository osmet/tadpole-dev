#pragma once

namespace app_domain
{
    struct Character
    {
        std::string Id;
        std::string Name;
        std::string PortraitTextureId;
        float MaxWeight = 100.f;
        std::string InventoryId;
    };

    using CharacterMap = std::unordered_map<std::string, Character>;
}