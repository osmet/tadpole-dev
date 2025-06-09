#pragma once

#include "../core/IJsonData.h"

namespace app
{
	class AssetConfig : public core::IJsonData
	{
	public:
		void FromJson(const rapidjson::Value& json) override;

		const std::unordered_map<std::string, std::string>& GetFontPaths() const;
		const std::unordered_map<std::string, std::string>& GetTexturePaths() const;

	private:
		std::unordered_map<std::string, std::string> m_fontPaths;
		std::unordered_map<std::string, std::string> m_texturePaths;
	};
}