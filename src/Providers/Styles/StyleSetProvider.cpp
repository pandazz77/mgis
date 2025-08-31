#include "StyleSetProvider.h"

StyleSetProvider::StyleSetProvider(std::initializer_list<StyleUnit> units){
    for(const StyleUnit &unit: units) add(unit);
}

void StyleSetProvider::add(const StyleUnit& unit){
    styles.push_back(unit);
}

void StyleSetProvider::remove(const std::string &id){
    int index = -1;
    for(int i = 0; i < styles.size(); i ++){
        if(styles[i].id()==id){
            index = i; break;
        }
    }
    if(index<0) abort(); // not found
    styles.erase(styles.begin() + index);
}

std::vector<StyleUnit> StyleSetProvider::getStyles(){
    return styles;
}

StyleUnit StyleSetProvider::get(std::string id){
    for(const StyleUnit &unit: styles){
        if(unit.id()==id) return unit;
    }
    qDebug() << "no style with id:" << id;
    abort();
}

void StyleSetProvider::onNewFeature(const Feature *feature, IStyler *& styler){
    for(const StyleUnit &style: styles){
        bool ok = true;
        for(const StyleCondition &condition: style.conditions()){
            ok = condition(StyleScope(feature));
            if(!ok) break;
        }
        if(!ok) continue; // do not apply style, because condition was not approved
        for(const StyleInstruction &instruction: style.instructions())
            instruction(styler,StyleScope(feature)); // apply style instruction
    }
}