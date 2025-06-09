#pragma once

#include "CharacterData.h"

namespace app
{
    enum class CharacterError : std::int8_t
    {
        NotFound = 0,
    };

    class CharacterService
    {
    public:
        CharacterService(CharacterDataMap& characters);

        tl::expected<std::reference_wrapper<const CharacterData>, CharacterError>
            GetCharacterById(const std::string& id) const;

    private:
        CharacterDataMap& m_characters;
    };
}
