#pragma once

#include "StyleSetProvider.h"
#include "MagicValues.h"

class JsonStyleProvider: public StyleSetProvider{
    public:
        static constexpr double VERSION = 0.1;

        JsonStyleProvider(const QString &jsonPath);
        JsonStyleProvider(const QJsonDocument &doc);

        const QVariantMap &metaData(); 

    protected:
        void parseMetaData();

    private:
        void load(const QJsonDocument &doc);
        void fromFile(const QString &filePath);

        void parseMetadata(const QVariantMap &map);
        void parseConstants(const QVariantMap &constans);
        void parseStyle(const QVariantMap &style);

        StyleCondition parseCondition(QString conditionExp);
        StyleInstruction parseInstruction(QString key, QVariant val);

    private:
        QVariantMap currentMap;
        QVariantMap meta; // metadata
        MagicConstantSet magicConstans;
};