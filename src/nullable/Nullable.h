#ifndef NULLABLE_H_INCLUDED
#define NULLABLE_H_INCLUDED

#include <type_traits>
#include <ostream>
#include <istream>

#include <nlohmann/json.hpp>


namespace nullable{


/**
 * @class Nullable
 *
 * Class allows the type encapsulated to have a NULL value.
*/
template<typename T>
class Nullable{

// binary operator equality
#ifndef _BINARY_OP_EQ

#define _BINARY_OP_EQ(op) \
    bool operator op (const Nullable &other) const{ \
        return (this->_value op other._value) && this->_isNull == other._isNull; \
    } \
\
    bool operator op (const T &value) const{ \
        return !(this->_value op value); \
    } \
\
  /*  template<typename Type> \
//    friend bool operator op (const Type &value, const Nullable<Type> &nullable){ return nullable op value; } */

#endif // _BINARY_OP_EQ


// binary operator arithmetic
#ifndef _BINARY_OP_ARITH

#define _BINARY_OP_ARITH(op) \
    /*template<typename = std::enable_if<std::is_arithmetic<T>::value>>*/ \
    T operator op (const Nullable<T> &other) const{ \
        return this->_value op other._value; \
    }

#endif // _BINARY_OP_ARITH


private:

    T _value;
    bool _isNull;

public:

    Nullable() : _value{}, _isNull{true}{}

    explicit Nullable(std::nullptr_t) : _value{}, _isNull{true} {}

    Nullable(const Nullable<T> &other) : _value{other._value}, _isNull{other._isNull}{
    }

    Nullable(const T &value) : _value{value}, _isNull{false} {
    }

    Nullable(const nlohmann::json &json) : _value{json.get<T>()}, _isNull{json.is_null()}{
    }

    Nullable &operator=(const Nullable<T> &other){
        this->_value = other._value;
        this->_isNull = other._isNull;
        return *this;
    }

    Nullable &operator=(const T &value){
        this->_value = value;
        this->_isNull = false;
        return *this;
    }


    virtual ~Nullable(void) {}
    virtual bool is_null() const noexcept { return this->_isNull; }

    /**
     * operator bool
     * @note T has to have operator bool overloaded (or be a primitive type)
     * @return bool
    */
    operator bool(void) const { return !this->_isNull && this->_value; }


    _BINARY_OP_EQ(==)
    _BINARY_OP_EQ(!=)
    _BINARY_OP_EQ(<)
    _BINARY_OP_EQ(>)
    _BINARY_OP_EQ(>=)
    _BINARY_OP_EQ(<=)

    _BINARY_OP_ARITH(^)
    _BINARY_OP_ARITH(&)
    _BINARY_OP_ARITH(|)
    _BINARY_OP_ARITH(*)
    _BINARY_OP_ARITH(/)
    _BINARY_OP_ARITH(-)
    _BINARY_OP_ARITH(+)

    operator T(void) const{
        return this->_value;
    }

    //template<typename = std::enable_if<std::is_arithmetic<T>::value>>
    T operator++(int){
        return this->_value++;
    }

    //template<typename = std::enable_if<std::is_arithmetic<T>::value>>
    T operator++(void){
        return ++this->_value;
    }

    //template<typename = std::enable_if<std::is_arithmetic<T>::value>>
    T operator--(int){
        return this->_value--;
    }

    //template<typename = std::enable_if<std::is_arithmetic<T>::value>>
    T operator--(void){
        return --this->_value;
    }

    //template<typename = std::enable_if<std::is_arithmetic<T>::value>>
    T operator ~ (void){
        return ~(this->_value);
    }


    friend std::ostream &operator << (std::ostream &os, const Nullable &nullable){
        if (nullable._isNull) os << '\0';
        else os << nullable._value;
        return os;
    }

    friend std::istream &operator >> (std::istream &is, Nullable &nullable){
        return is >> nullable._value;
    }
};


}


#endif // NULLABLE_H_INCLUDED
