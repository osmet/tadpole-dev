#pragma once

#include "../core/UIView.h"

namespace app
{
    class AppContext;

    class TradeUIView : public core::UIView
    {
    public:
        TradeUIView(AppContext& appContext);

        void Initialize() override;

    private:
        AppContext& m_appContext;
    };
}
