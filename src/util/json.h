#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include <type_traits>
#include <sstream>

namespace util{

namespace json{

template<typename T>
std::string jsonEntry(std::string key, const T &data) {

   // constexpr bool i = std::is_arithmetic<T>::value;

    std::stringstream s;

    s << "\"" << key << "\""
      << ':'
      << "\"" << data << "\"";

    return s.str();
}

}

}


#endif // JSON_H_INCLUDED
