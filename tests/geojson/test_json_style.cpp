#include "JsonStyleProvider.h"
#include "MagicValues.h"
#include "ExpressionParser.h"



void testMagic(){
    const MagicConstantSet MCONSTANTS{
        {"$POLY",MagicConstant(QString::number((int)Geometry::Type::POLYGON))},
        {"$LINE",MagicConstant(QString::number((int)Geometry::Type::LINESTRING))},
        {"$POINT",MagicConstant(QString::number((int)Geometry::Type::POINT))},
        {"$SOMECONSTANT",MagicConstant("1337")},
        {"$1_SOMECONSTANT",MagicConstant("1337")}
    };

    Feature feature(new Polygon(LinearRing({
        {90,180},
        {-90,180},
        {-90,-180},
        {90,-180},
        {90,180}
    })));
    // feature.properties["sss"]

    ExpressionParser::Value result;

    QString s1 = "$SOMECONSTANT == 1337";
    MagicFactory::eval(s1, nullptr,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s1.toStdString());
    assert(std::get<bool>(result));

    QString s2 = "$SOMECONSTANT==$1_SOMECONSTANT";
    MagicFactory::eval(s2, nullptr,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s2.toStdString());
    assert(std::get<bool>(result));
}

int main() {
    testMagic();

    // JsonStyleProvider styler("style.json");
    return 0;
}