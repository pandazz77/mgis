#include "JsonStyleProvider.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include "MagicValues.h"

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
    currentMap = doc.toVariant().toMap();

    if(currentMap.contains("metadata")) parseMetadata(currentMap["metadata"].toMap());
    else qWarning() << "JsonStyle doesnt contains metadata";

    if(currentMap.contains("constants")) parseConstants(currentMap["constants"].toMap());

    QVariantList styles = currentMap["styles"].toList();
    for(QVariant styleVar: styles){
        parseStyle(styleVar.toMap());
    }
}

void JsonStyleProvider::parseMetadata(const QVariantMap &map){
    /// TODO: implement
}

void JsonStyleProvider::parseConstants(const QVariantMap &constans){
    currentMap.remove("constants"); // we dont need this anymore
    QByteArray raw = QJsonDocument::fromVariant(currentMap).toJson();
    for(auto kv: constans.asKeyValueRange()){
        QByteArray valueRaw = kv.second.toJsonValue().toJson();
        QByteArray keyRaw = '"' + kv.first.toUtf8() + '"'; // we need to enquote key for right replacement
        raw.replace(keyRaw,valueRaw);
    }
    currentMap = QJsonDocument::fromJson(raw).toVariant().toMap();
}

void JsonStyleProvider::parseStyle(const QVariantMap &style){
    QString id;
    if(style.contains("id")) id = style["id"].toString();
    
    QStringList conditionsExpr;
    if(style["condition"].typeId() == QMetaType::QString) 
        conditionsExpr << style["condition"].toString();
    else if(style["condition"].typeId() == QMetaType::QVariantList)
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