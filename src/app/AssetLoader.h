#pragma once

namespace core
{
    class AssetManager;
}

namespace app
{
    class AssetConfig;

    class AssetLoader final
    {
    public:
        static void LoadAssets(core::AssetManager& assetManager, const std::string& rootFilePath, const AssetConfig& assetConfig);
    };
}