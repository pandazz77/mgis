#pragma once

#include "StyleProvider.h"
#include <functional>

class SimpleStyleProvider: public StyleProvider{
    public:
        SimpleStyleProvider();
        SimpleStyleProvider(std::function<IStyler*(Feature*)> callback);

        void setCallback(std::function<IStyler*(Feature*)> callback);
        void removeCallback();

    protected:
        IStyler *onNewFeature(Feature *feature) override;

    private:
        std::function<IStyler*(Feature*)> onNewFeatureCallback;
};