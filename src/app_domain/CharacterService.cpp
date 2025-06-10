#include "Precompiled.h"
#include "CharacterService.h"

namespace app_domain
{
    CharacterService::CharacterService(CharacterMap& characters)
        : m_characters(characters)
    {
    }

    tl::expected<std::reference_wrapper<const Character>, CharacterError>
        CharacterService::GetCharacterById(const std::string& id) const
    {
        auto it = m_characters.find(id);
        if (it == m_characters.end())
            return tl::unexpected(CharacterError::NotFound);

        return std::cref(it->second);
    }
}
