#pragma once

#include "../core/IJsonData.h"

namespace app
{
	class AppConfig : public core::IJsonData
	{
	public:
		void FromJson(const rapidjson::Value& json) override;

		const std::string& GetAppName() const;
		std::uint32_t GetAppWindowWidth() const;
		std::uint32_t GetAppWindowHeight() const;
		std::uint32_t GetAppWindowStyle() const;

	private:
		std::string m_appName;
		std::uint32_t m_windowWidth = 0u;
		std::uint32_t m_windowHeight = 0u;
		std::uint32_t m_windowStyle = sf::Style::Default;
	};
}