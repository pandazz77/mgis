#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

class PropertiesNode;
class PropertiesList;

namespace {

template<typename T,typename... Types>
T cast_helper(std::variant<Types...> var){
    if constexpr(std::is_same_v<T,double>)
        if(std::holds_alternative<int>(var)) return static_cast<double>(std::get<int>(var));
}

template<typename... Types>
class variant: public std::variant<Types...>{
    public:
        using std::variant<Types...>::variant;

        template<typename T>
        T to() const {
            if(is<T>()) return std::get<T>(*this);

            return cast_helper<T>(*this);
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

        const PropertyValue operator[](const std::string &key) const{
            return this->at(key);
        }

        PropertyValue &operator[](const std::string &key){
            return std::map<std::string,PropertyValue>::map::operator[](key);
        }
};

using FeatureProperties = PropertiesNode;