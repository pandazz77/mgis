#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

class PropertiesNode;
class PropertiesList;

namespace {

template<typename... Types>
class variant: public std::variant<Types...>{
    public:
        using std::variant<Types...>::variant;

        template<typename T>
        T to() const {
            return std::get<T>(*this);
        }

        template<typename T>
        bool is() const {
            return std::holds_alternative<T>(*this);
        }
        
        template<typename T>
        bool operator == (const T &other) const{
            return to<T>()==other;
        }

        template<typename T>
        bool operator != (const T &other) const{
            return to<T>()!=other;
        }
        
        template<typename T>
        bool operator > (const T &other) const{
            return to<T>()>other;
        }

        template<typename T>
        bool operator >= (const T &other) const{
            return to<T>()>=other;
        }

        template<typename T>
        bool operator < (const T &other) const{
            return to<T>()<other;
        }

        template<typename T>
        bool operator <= (const T &other) const{
            return to<T>()<=other;
        }
};

};

using PropertyValue = variant<
    int,
    double,
    bool,
    std::string,
    PropertiesNode,
    PropertiesList,
    std::nullptr_t
>;

class PropertiesList: public std::vector<PropertyValue>{
    public:
        using std::vector<PropertyValue>::vector;
};

class PropertiesNode: public std::map<std::string,PropertyValue>{
    public:
        using std::map<std::string,PropertyValue>::map;

        bool has(const std::string &key) const{
            auto it = find(key);
            return it != end();
        }
};

using FeatureProperties = PropertiesNode;