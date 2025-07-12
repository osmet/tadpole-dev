#pragma once

namespace app
{
    class AppConstants
    {
    public:
        static const std::string& GetConfigsRootPath();
        static const std::string& GetAssetsRootPath();
        static const std::string& GetDataRootPath();
        static const std::string& GetDataDefaultsRootPath();
    };
}