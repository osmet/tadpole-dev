#include "Precompiled.h"
#include "AssetLoader.h"
#include "AssetConfig.h"
#include "../core/AssetManager.h"

namespace app
{
    void AssetLoader::LoadAssets(core::AssetManager& assetManager, const std::string& rootPath, const AssetConfig& assetConfig)
    {
        assetManager.SetRootPath(rootPath);

        const auto& fontPaths = assetConfig.GetFontPaths();
        for (auto it = fontPaths.begin(); it != fontPaths.end(); ++it)
        {
            assetManager.LoadFont(it->first, it->second);
        }

        const auto& texturePaths = assetConfig.GetTexturePaths();
        for (auto it = texturePaths.begin(); it != texturePaths.end(); ++it)
        {
            assetManager.LoadTexture(it->first, it->second);
        }
    }
}