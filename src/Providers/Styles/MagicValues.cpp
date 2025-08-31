#include "MagicValues.h"

#include <QRegularExpression>

// MARK: MagicConstant

MagicConstant::MagicConstant() : value("") {

}

MagicConstant::MagicConstant(QString value) : value(value) {

}

QString MagicConstant::str(){
    return value;
}

// =======================
// MARK: MagicExp

MagicExp::MagicExp(QString mprop) : mprop(mprop) {

}

QString MagicExp::str(){
    return dotFunc(mprop)->str();
}

QString MagicExp::nextKey(const QString &currentKey){
    int dotPos = currentKey.indexOf('.');
    return currentKey.sliced(dotPos+1);
}

QString MagicExp::currentKey(const QString &fullPath, bool &hasNext){
    int dotPos = fullPath.indexOf('.');
    hasNext = false;
    
    if(dotPos<0) return fullPath;
    hasNext = true;
    return fullPath.sliced(0,dotPos);
}

// =======================
// MARK: MagicGeometry


MagicGeometry::MagicGeometry(Geometry *geometry, QString mprop): MagicExp(mprop), geometry(geometry) {

}

std::unique_ptr<MagicValue> MagicGeometry::dotFunc(QString mprop){
    if(mprop=="type") 
        return std::make_unique<MagicConstant>(QString::number((int)geometry->type()));
}


// =======================
// MARK: MagicProperty

MagicProperty::MagicProperty(const Feature::Properties &properties, QString mprop): MagicExp(mprop), propeties(properties) {

}

std::unique_ptr<MagicValue> MagicProperty::dotFunc(QString mprop){
    bool hasNext;
    const QString currentKey = MagicExp::currentKey(mprop,hasNext);
    if(!hasNext) { // end
        if(!propeties.has(currentKey.toStdString())) // does not exist
            return std::make_unique<MagicConstant>("false");
        PropertyValue val = propeties[currentKey.toStdString()];
        QString result = QString::fromStdString(val.to<std::string>());
        if(val.is<std::string>()) result = '\'' + result + '\'';

        return std::make_unique<MagicConstant>(result);
    }
    
    const QString nextKey = MagicExp::nextKey(currentKey);
    return std::make_unique<MagicProperty>(
        propeties[currentKey.toStdString()].to<Feature::Properties>(),
        nextKey
    );
}

// =======================
// MARK: MagicFactory

std::unique_ptr<MagicValue> MagicFactory::value(QString magic,const Feature *feature , const MagicConstantSet &constants){
    if(constants.contains(magic)) 
        return std::make_unique<MagicConstant>(constants.value(magic));
    
    if(magic.startsWith(MagicGeometry::ID))
        return std::make_unique<MagicGeometry>(feature->geometry,MagicExp::nextKey(magic));
    if(magic.startsWith(MagicProperty::ID))
        return std::make_unique<MagicProperty>(feature->properties,MagicExp::nextKey(magic));
    qWarning() << "UNREGISTRED MAGIC" << magic;
}

void MagicFactory::eval(QString &strWithMagic,const Feature *feature, const MagicConstantSet &constants){
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