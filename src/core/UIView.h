#pragma once

#include "CanvasPanel.h"

namespace core
{
    class UIView : public CanvasPanel
    {
    public:
        virtual void Initialize() = 0;
    };
}
