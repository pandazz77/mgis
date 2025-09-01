#pragma once

#include "StyleProvider.h"
#include "StyleUnit.hpp"

class StyleSetProvider: public StyleProvider{
    public:
        StyleSetProvider(std::initializer_list<StyleUnit> units = {});

        void onNewFeature(const Feature *feature, IStyler *& styler) override;

        void add(const StyleUnit& unit);
        void remove(const std::string &id);
        std::vector<StyleUnit> getStyles();
        StyleUnit get(std::string id);

    protected:
        std::vector<StyleUnit> styles;
};