#pragma once

#include <QString>
#include <QHash>

#include "Feature.h"

class Feature;

class MagicValue{ // Base
    public:
        virtual QString str() = 0;
};

class MagicConstant: public MagicValue{ // Ex: $POLY
    public:
        MagicConstant();
        MagicConstant(QString value);
        
        QString str() override;

    protected:
        QString value;
};

class MagicExp: public MagicValue{ // Magic expression
    public:
        MagicExp(QString mprop);

        QString str() override;

        static QString nextKey(const QString &currentKey);
        static QString currentKey(const QString &fullPath, bool &hasNext);

        virtual std::unique_ptr<MagicValue>dotFunc(QString mprop) = 0;
    private:
        const QString mprop; 
};

class MagicGeometry: public MagicExp{
    public:
        inline static const char *ID = "$GEOMETRY";
        MagicGeometry(Geometry *geometry, QString mprop);

        std::unique_ptr<MagicValue> dotFunc(QString mprop) override;

    private:
        Geometry *geometry;
};

class MagicProperty: public MagicExp{
    public:
        inline static const char *ID = "$PROPERTIES";

        MagicProperty(const Feature::Properties &properties, QString mprop);
        std::unique_ptr<MagicValue> dotFunc(QString mprop) override;

    private:
        const Feature::Properties properties;
};

using MagicConstantSet = QHash<QString,MagicConstant>;

namespace MagicFactory{

/**
 * @brief MagicValueFactory: Parse magic string and return magic value
 * 
 * @param magic magic string to be parsed
 * @param feature captured
 * @param constants set of constants
 * @return std::unique_ptr<MagicValue> produced value 
 */
std::unique_ptr<MagicValue> value(QString magic, const Feature *feature , const MagicConstantSet &constants={});

/**
 * @brief MagicExpression evaluator
 * 
 * @param strWithMagic QString, that contains magic substring and need to be processed (all magic strings will be replaced by magic values)
 * @param feature captured
 * @param constants set of constants
 */
void eval(QString &strWithMagic, const Feature *feature, const MagicConstantSet &constants={});

};