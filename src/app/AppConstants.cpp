#include "Precompiled.h"
#include "AppConstants.h"

namespace app
{
	namespace
	{
		const std::string ConfigsRootPath = "../../configs/";
		const std::string AssetsRootPath = "../../assets/";
		const std::string DataRootPath = "../../data/";
	}

	const std::string& AppConstants::GetConfigsRootPath()
	{
		return ConfigsRootPath;
	}
	const std::string& AppConstants::GetAssetsRootPath()
	{
		return AssetsRootPath;
	}

	const std::string& AppConstants::GetDataRootPath()
	{
		return DataRootPath;
	}
}