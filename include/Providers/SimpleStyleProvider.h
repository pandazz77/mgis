#pragma once

#include "StyleProvider.h"
#include <functional>

class SimpleStyleProvider: public StyleProvider{
    public:
        SimpleStyleProvider();
        SimpleStyleProvider(std::function<void(FeatureLayer*)> callback);

        void setCallback(std::function<void(FeatureLayer*)> callback);
        void removeCallback();

    protected:
        void onNewFeature(FeatureLayer *feature) override;

    private:
        std::function<void(FeatureLayer*)> onNewFeatureCallback;
};