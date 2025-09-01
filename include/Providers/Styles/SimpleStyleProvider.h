#pragma once

#include "StyleProvider.h"
#include <functional>

class SimpleStyleProvider: public StyleProvider{
    using sig = void(const Feature *feature, IStyler *& styler);

    public:
        SimpleStyleProvider();
        SimpleStyleProvider(std::function<sig> callback);

        void setCallback(std::function<sig> callback);
        void removeCallback();

    protected:
        void onNewFeature(const Feature *feature, IStyler *& styler) override;

    private:
        std::function<sig> onNewFeatureCallback;
};