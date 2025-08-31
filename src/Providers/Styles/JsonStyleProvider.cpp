#include "JsonStyleProvider.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QHash>
#include "MagicValues.h"
#include "ExpressionParser.h"

inline MagicConstant gTypeOf(const Geometry::Type &t){
    return MagicConstant(QString::number((int)t));
}

const MagicConstantSet GEOMETRY_CONSTANTS {
    {"$POINT",    gTypeOf(Geometry::Type::POINT) },
    {"$LINE",     gTypeOf(Geometry::Type::LINESTRING)},
    {"$POLY",     gTypeOf(Geometry::Type::POLYGON) },
    {"$MPOINT",   gTypeOf(Geometry::Type::MULTIPOINT)},
    {"$MLINE",    gTypeOf(Geometry::Type::MULTILINESTRING)},
    {"$MPOLY",    gTypeOf(Geometry::Type::MULTIPOLYGON)}
};

const QHash<QString,Qt::PenCapStyle> PEN_CAPS {
    {"flat",Qt::FlatCap},
    {"square",Qt::SquareCap},
    {"round",Qt::RoundCap}
};

const QHash<QString,Qt::PenJoinStyle> PEN_JOIN {
    {"miter",Qt::MiterJoin},
    {"bevel",Qt::BevelJoin},
    {"round",Qt::RoundJoin},
    {"svgmiter",Qt::SvgMiterJoin}
};

const QHash<QString,Qt::PenStyle> PEN_STYLE {
    // {"nopen",Qt::NoPen},
    {"solid",Qt::SolidLine},
    {"dash",Qt::DashLine},
    {"dot",Qt::DotLine},
    {"dash-dot",Qt::DashDotLine},
    {"dash-dot-dot",Qt::DashDotDotLine}
};

const QHash<QString,Qt::BrushStyle> BRUSH_STYLE{
    // {"nobrush",Qt::NoBrush},
    {"solid",Qt::SolidPattern},
    {"dense1",Qt::Dense1Pattern},
    {"dense2",Qt::Dense2Pattern},
    {"dense3",Qt::Dense3Pattern},
    {"dense4",Qt::Dense4Pattern},
    {"dense5",Qt::Dense5Pattern},
    {"dense6",Qt::Dense6Pattern},
    {"dense7",Qt::Dense7Pattern},
    {"hor",Qt::HorPattern},
    {"ver",Qt::VerPattern},
    {"cross",Qt::CrossPattern},
    {"bdiag",Qt::BDiagPattern},
    {"fdiag",Qt::FDiagPattern},
    {"diag-cross",Qt::DiagCrossPattern},
    /// TODO: implement gradients ?
    // {"linear-gradient",Qt::LinearGradientPattern},
    // {"conical-gradient",Qt::ConicalGradientPattern},
    // {"radial-gradient",Qt::RadialGradientPattern},
};

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

    currentMap.clear();
}

const QVariantMap &JsonStyleProvider::metaData(){
    return meta;
}

void JsonStyleProvider::parseMetadata(const QVariantMap &map){
    meta = map;

    // ================
    if(meta.contains("mgis-jsonstyle-spec")){
        double ver = meta["mgis-jsonstyle-spec"].toDouble();
        if(ver>VERSION) qWarning() << "Loaded JsonStyle version" << ver << "higher than supported" << VERSION;
    } else {
        qWarning() << "JsonStyle version not specified!";
    }
    // ================
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

    StyleSetProvider::add(StyleUnit{
        conditions,
        instructions,
        id.toStdString()
    });
}

StyleCondition JsonStyleProvider::parseCondition(QString conditionExp){
    return StyleCondition([conditionExp](const StyleScope &scope){
        QString cond = conditionExp;
        MagicFactory::eval(cond,scope.feature,GEOMETRY_CONSTANTS);
        ExpressionParser::Value res = ExpressionParser::evaluateExpression(cond.toStdString());
        // qDebug() << conditionExp << "--->" << cond << "--->" << std::get<bool>(res);
        return std::get<bool>(res);
        
    });
}

// STYLE CAST HELPERS =================
template <typename TStyle>
inline TStyle *sCast(IStyler *styler){
    return dynamic_cast<TStyle*>(styler);
}

#define spoint(styler) sCast<PointStyler>(styler)
#define sline(styler) sCast<LineStyler>(styler)
#define spoly(styler) sCast<PolyStyler>(styler)
// =====================================

StyleInstruction JsonStyleProvider::parseInstruction(QString key, QVariant _val){
    return StyleInstruction([key,_val](IStyler *& styler, const StyleScope &scope){
        QVariant val = _val;
        if(val.typeId()==QMetaType::QString){
            QString sVal = val.toString();
            MagicFactory::eval(sVal,scope.feature);
            val = QVariant(sVal);
        }

        if(key=="fill")
            spoly(styler)->setFill(val.toString());
        else if(key=="fill-opacity")
            spoly(styler)->setFillOpacity(val.toDouble());
        else if(key=="fill-texture")
            spoly(styler)->setTexture(QPixmap(val.toString()));
        else if(key=="texture-size"){
            QVariantList lst = val.toList();
            spoly(styler)->setTextureSize(QSize(lst[0].toInt(),lst[1].toInt()));
        }
        else if(key=="fill-style")
            spoly(styler)->setFillStyle(BRUSH_STYLE[val.toString()]);


        else if(key=="stroke")
            sline(styler)->setStroke(val.toString());
        else if(key=="stroke-width")
            sline(styler)->setWidth(val.toInt());
        else if(key=="stroke-opacity")
            sline(styler)->setStrokeOpacity(val.toDouble());
        else if(key=="dash-pattern"){
            QList<qreal> pattern; 
            for(auto var: val.toList()) pattern.append(var.toDouble());
            sline(styler)->setDashPattern(pattern);
        }   
        else if(key=="dash-offset")
            sline(styler)->setDashOffset(val.toDouble());
        else if(key=="stroke-cap")
            sline(styler)->setCapStyle(PEN_CAPS[val.toString()]);
        else if(key=="stroke-cosmetic")
            sline(styler)->setCosmetic(val.toDouble());
        else if(key=="stroke-join")
            sline(styler)->setJoinStyle(PEN_JOIN[val.toString()]);
        else if(key=="stroke-style")
            sline(styler)->setStrokeStyle(PEN_STYLE[val.toString()]);

        else if(key=="icon")
            spoint(styler)->setPixmap(QPixmap(val.toString()));
        else if(key=="icon-anchor"){
            QVariantList lst = val.toList();
            spoint(styler)->setAnchor(QPointF(lst[0].toDouble(),lst[1].toDouble()));
        }
        else if(key=="text")
            spoint(styler)->setPixmap(PointStyler::TextPixmap(val.toString()));


        else
            qWarning() << "UNSUPPORTED STYLE INSTRUCTION:" << key << "WITH VALUE" << val;
    });
}