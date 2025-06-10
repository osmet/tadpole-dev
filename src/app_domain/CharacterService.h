#pragma once

#include "Character.h"

namespace app_domain
{
    enum class CharacterError : std::int8_t
    {
        NotFound = 0,
    };

    class CharacterService
    {
    public:
        CharacterService(CharacterMap& characters);

        tl::expected<std::reference_wrapper<const Character>, CharacterError>
            GetCharacterById(const std::string& id) const;

    private:
        CharacterMap& m_characters;
    };
}
