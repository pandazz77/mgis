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
    feature.properties["name"] = "water";
    feature.properties["level"] = 5;
    feature.properties["isEarth"] = false;

    bool hasNext; 
    QString key;

    key = MagicExp::currentKey("$TEST1",hasNext);
    assert(key == "$TEST1" && !hasNext);
    
    key = MagicExp::currentKey("$TEST1.test2",hasNext);
    assert(key == "$TEST1" && hasNext);

    key = MagicExp::nextKey("$TEST1.test2");
    assert(key=="test2");

    ExpressionParser::Value result;

    QString s1 = "$SOMECONSTANT == 1337";
    MagicFactory::eval(s1, nullptr,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s1.toStdString());
    assert(std::get<bool>(result));

    /// @warning magic constants $SOMECONSTANT_1 are not passed
    QString s2 = "$SOMECONSTANT==$1_SOMECONSTANT";
    MagicFactory::eval(s2, nullptr,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s2.toStdString());
    assert(std::get<bool>(result));

    QString s3 = "$GEOMETRY.type == $POLY";
    MagicFactory::eval(s3,&feature,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s3.toStdString());
    assert(std::get<bool>(result));

    QString s4 = "$PROPERTIES.name == 'water' ";
    MagicFactory::eval(s4,&feature,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s4.toStdString());
    assert(std::get<bool>(result));

    QString s5 = "$PROPERTIES.level > 3 and $PROPERTIES.level < 7";
    MagicFactory::eval(s5,&feature,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s5.toStdString());
    assert(std::get<bool>(result));

    QString s6 = "$PROPERTIES.isEarth != true";
    MagicFactory::eval(s6,&feature,MCONSTANTS);
    result = ExpressionParser::evaluateExpression(s6.toStdString());
    assert(std::get<bool>(result));
}

int main() {
    testMagic();

    // JsonStyleProvider styler("style.json");
    return 0;
}