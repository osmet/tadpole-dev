#include "Precompiled.h"
#include "ScreenOverlayPanel.h"

#include "../core/Image.h"

namespace app
{
    ScreenOverlayPanel::ScreenOverlayPanel(const sf::Vector2f& renderWindowSize)
    {
        SetAnchor(0.f, 0.f);
        SetPivot(0.f, 0.f);

        auto* overlayImage = CreateWidget<core::Image>();
        overlayImage->SetAnchor(.5f, .5f);
        overlayImage->SetPivot(.5f, .5f);
        overlayImage->SetSize(renderWindowSize);
        overlayImage->SetColor(sf::Color(0u, 0u, 0u, 150u));
    }

    void ScreenOverlayPanel::Show()
    {
        SetActive(true);
    }

    void ScreenOverlayPanel::Hide()
    {
        SetActive(false);
    }
}