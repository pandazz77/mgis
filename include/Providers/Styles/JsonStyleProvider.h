#pragma once

#include "StyleSetProvider.h"

class JsonStyleProvider: protected StyleSetProvider{
    public:
        JsonStyleProvider(const QString &jsonPath);
        JsonStyleProvider(const QJsonDocument &doc);

        

    protected:
        void parseMetaData();

    private:
        void load(const QJsonDocument &doc);
        void fromFile(const QString &filePath);

        void parseMetadata(const QVariantMap &map);
        void parseConstants(const QVariantMap &constans);
        void parseStyle(const QVariantMap &style);

        static StyleCondition parseCondition(QString conditionExp);
        static StyleInstruction parseInstruction(QString key, QVariant val);

    private:
        QVariantMap currentMap;
};