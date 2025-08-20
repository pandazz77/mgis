#include "JsonStyleProvider.h"

#include <QFile>
#include <QJsonDocument>

JsonStyleProvider::JsonStyleProvider(const QString &jsonPath){
    fromFile(jsonPath);
}

JsonStyleProvider::JsonStyleProvider(const QJsonDocument &doc){
    load(doc);
}

void JsonStyleProvider::fromFile(const QString &filePath){
    QFile file(filePath);
    bool opened = file.open(QIODevice::ReadOnly);
    assert(opened);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    load(doc);
}

void JsonStyleProvider::load(const QJsonDocument &doc){
    QVariantMap map = doc.toVariant().toMap();

    if(map.contains("metadata")) parseMetadata(map["metadata"].toMap());
    else qWarning() << "JsonStyle doesnt contains metadata";

    if(map.contains("constants")) parseConstants(map["constants"].toMap());

    QVariantList styles = map["styles"].toList();
    for(QVariant styleVar: styles){
        parseStyle(styleVar.toMap());
    }
}

void JsonStyleProvider::parseMetadata(const QVariantMap &map){
    /// TODO: implement
}

void JsonStyleProvider::parseConstants(const QVariantMap &constans){
    /// TODO: preprocessor
}

void JsonStyleProvider::parseStyle(const QVariantMap &style){
    QString id;
    if(style.contains("id")) id = style["id"].toString();
    
    QStringList conditionsExpr;
    if(style["condition"].typeId() == QMetaType::QString) 
        conditionsExpr << style["condition"].toString();
    else if(style["condition"].typeId() == QMetaType::QStringList)
        conditionsExpr = style["condition"].toStringList();
    else
        abort();

    StyleConditionSet conditions;
    for(QString conditionExp: conditionsExpr){
        conditions.push_back(parseCondition(conditionExp));
    }

    StyleInstructionSet instructions;
    for(auto kv: style["instruction"].toMap().asKeyValueRange()){
        instructions.push_back(parseInstruction(kv.first,kv.second));
    }

    add(StyleUnit{
        conditions,
        instructions,
        id.toStdString()
    });
}

StyleCondition JsonStyleProvider::parseCondition(QString conditionExp){
    /// TODO: implement
}

StyleInstruction JsonStyleProvider::parseInstruction(QString key, QVariant val){
    /// TODO: implement
}