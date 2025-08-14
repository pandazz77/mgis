#pragma once

#include <map>
#include <string>
#include <variant>

class PropertiesNode;
using PropertyValue = std::variant<int,double,bool,std::string,PropertiesNode>;

class PropertiesNode: public std::map<std::string,PropertyValue>{
    public:
        using std::map<std::string,PropertyValue>::map;

        bool has(const std::string &key){
            auto it = this->find(key);
            return it != this->end();
        }

        template<typename T>
        bool holds(const std::string &key) const{
            auto variant = this->at(key);
            return std::holds_alternative<T>(variant);
        }

        template<typename T>
        T get(const std::string &key) const{
            auto variant = this->at(key);
            return std::get<T>(variant);
        }
};

using FeatureProperties = PropertiesNode;