#pragma once

#include <unordered_map>
#include <string>
#include <variant>

class PropertiesNode;
class PropertiesNode: public std::unordered_map<std::string,std::variant<int,double,bool,std::string,PropertiesNode>>{
    public:
        bool has(const std::string &key){
            auto it = find(key);
            return it != end();
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