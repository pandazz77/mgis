#pragma once

#include <QString>
#include <QHash>
#include <QRegularExpression>

#include "Feature.h"

class MagicValue{ // Base
    public:
        virtual QString str() = 0;
};

class MagicConstant: public MagicValue{ // Ex: $POLY
    public:
        MagicConstant() : value("") {}
        MagicConstant(QString value) : value(value) {}
        
        QString str() override{
            return value;
        }

    protected:
        QString value;
};

class MagicExp: public MagicValue{ // Magic expression
    public:
        MagicExp(QString mprop) : mprop(mprop) {}

        QString str() override{
            return dotFunc(mprop)->str();
        }

        static QString nextKey(const QString &currentKey){
            return currentKey.sliced('.');
        }

        static QString currentKey(const QString &fullPath, bool &hasNext){
            int dotPos = fullPath.indexOf('.');
            hasNext = false;
            
            if(dotPos<0) return fullPath;
            hasNext = true;
            return fullPath.sliced(0,dotPos);
        }

        virtual std::unique_ptr<MagicValue>dotFunc(QString mprop) = 0;
    private:
        const QString mprop; 
};

class MagicGeometry: public MagicExp{
    public:
        inline static const char *ID = "$GEOMETRY";
        MagicGeometry(Geometry *geometry, QString mprop): MagicExp(mprop), geometry(geometry) {}

        std::unique_ptr<MagicValue> dotFunc(QString mprop){
            if(mprop=="type") 
                return std::make_unique<MagicConstant>(QString::number((int)geometry->type()));
        }

    private:
        Geometry *geometry;
};

class MagicProperty: public MagicExp{
    public:
        inline static const char *ID = "$PROPERTIES";
        MagicProperty(const Feature::Properties &properties, QString mprop): MagicExp(mprop), propeties(properties) {}

        std::unique_ptr<MagicValue> dotFunc(QString mprop){
            bool hasNext;
            const QString currentKey = MagicExp::currentKey(mprop,hasNext);
            if(!hasNext) { // end
                return std::make_unique<MagicConstant>(
                    QString::fromStdString(propeties[currentKey.toStdString()].to<std::string>())
                );
            }
            
            const QString nextKey = MagicExp::nextKey(currentKey);
            return std::make_unique<MagicProperty>(
                propeties[currentKey.toStdString()].to<Feature::Properties>(),
                nextKey
            );
        }

    private:
        const Feature::Properties &propeties;
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
inline std::unique_ptr<MagicValue> value(QString magic, Feature *feature , const MagicConstantSet &constants={}){
    if(constants.contains(magic)) 
        return std::make_unique<MagicConstant>(constants.value(magic));
    
    if(magic.startsWith(MagicGeometry::ID))
        return std::make_unique<MagicGeometry>(feature->geometry,MagicExp::nextKey(magic));
    if(magic.startsWith(MagicProperty::ID))
        return std::make_unique<MagicProperty>(feature->properties,MagicExp::nextKey(magic));
    qWarning() << "UNREGISTRED MAGIC" << magic;
}

/**
 * @brief MagicExpression evaluator
 * 
 * @param strWithMagic QString, that contains magic substring and need to be processed (all magic strings will be replaced by magic values)
 * @param feature captured
 * @param constants set of constants
 */
inline void eval(QString &strWithMagic, Feature *feature, const MagicConstantSet &constants={}){
    QRegularExpression re(R"(\$[\w\.]+)");
    QRegularExpressionMatchIterator i = re.globalMatch(strWithMagic);

    while(i.hasNext()){
        QRegularExpressionMatch match = i.next();
        QString captured = match.captured();

        strWithMagic.replace(
            captured,
            value(captured,feature,constants)->str()
        );
    }
}

};