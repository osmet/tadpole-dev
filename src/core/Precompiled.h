#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <streambuf>
#include <cassert>
#include <functional>

#include <SFML/Graphics.hpp>

#define RAPIDJSON_NOMEMBERITERATORCLASS
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

