#include "Precompiled.h"
#include "IJsonData.h"

namespace core
{
	IJsonData::~IJsonData() = default;

	void IJsonData::ToJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const
	{
	}
}