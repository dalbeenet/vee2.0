#ifndef _VEE_ENUMERATION_H_
#define _VEE_ENUMERATION_H_

#include <vee/strutils.h>
#include <cinttypes>
#include <map>

#define Enumeration2(EnumType, IntergralType, Offset, First, ...)\
enum class EnumType { First = Offset, __VA_ARGS__ };\
inline const char* to_string(EnumType value)\
{\
    auto gen = []() -> ::std::map<EnumType, ::std::string>\
                {\
        ::std::string raw(#First);\
        raw.append(", ");\
        raw.append(#__VA_ARGS__);\
        ::std::vector<::std::string> strings = ::vee::strutils::split(raw, ",");\
        ::std::map<EnumType, ::std::string> result;\
        IntergralType key = Offset;\
        for (unsigned int i = 0; i < strings.size(); ++i)\
                        {\
            result.insert(make_pair(static_cast<EnumType>(key++), ::vee::strutils::trim(strings[i])));\
                        }\
        return result;\
                };\
    static ::std::map<EnumType, ::std::string> map = gen();\
    return map[value].c_str();\
}

// KNOWN ISSUE: 문자열을 저장하고 있는 map은 동적할당되고 해제되지 않는다.
// 그 이유는 프로세스가 종료될 때도 to_string()이 유효해야하기 때문 -> Data 섹션에 저장하면 안됨
// 프로세스가 종료되면 OS가 heap을 수거할 것이므로 누수 문제는 없을 것이지만 편법임
// MSVC, Win10이 아닌 환경에서도 문자열 초기화와 사용이 잘 동작하는지 확인해야할 필요가 있음
#define Enumeration(__ENUM_TYPE__, Offset, __ENUMERATION_START__, ...)\
class __ENUM_TYPE__\
{\
public:\
    typedef enum\
    {\
        __ENUMERATION_START__ = Offset,\
        __VA_ARGS__,\
        __ENUMERATION_END__\
    } value_t;\
    inline static const char* to_string(__ENUM_TYPE__::value_t value)\
    {\
        auto gen = []() -> ::std::map<int, ::std::string>*\
        {\
            ::std::string raw(#__ENUMERATION_START__);\
            raw.append(", ");\
            raw.append(#__VA_ARGS__);\
            ::std::vector<::std::string> strings = ::vee::strutils::split(raw, ",");\
            ::std::map<int, ::std::string>* result = new ::std::map<int, ::std::string>();\
            int key = Offset;\
            for (unsigned int i = 0; i < strings.size(); ++i)\
            {\
                result->insert(make_pair(key++, ::vee::strutils::trim(strings[i])));\
            }\
            return result;\
        };\
        try\
        {\
            static ::std::map<int, ::std::string>* map = gen();\
            return map->operator[](static_cast<int>(value)).c_str();\
        }\
        catch(...)\
        {\
            return "error";\
        }\
    }\
    class iterator\
    {\
    public:\
        iterator(int value):\
            _value(value)\
        {\
        }\
        __ENUM_TYPE__::value_t operator*() const\
                {\
            return static_cast<__ENUM_TYPE__::value_t>(_value);\
                }\
        void operator++()\
        {\
            ++_value;\
        }\
        bool operator!=(const iterator& rhs) const\
        {\
            return _value != rhs._value;\
        }\
        const char* to_string()\
        {\
            return __ENUM_TYPE__::to_string(static_cast<__ENUM_TYPE__::value_t>(_value));\
        }\
    private:\
        int _value;\
    };\
    inline static iterator begin()\
    {\
        return iterator(static_cast<int>(__ENUM_TYPE__::__ENUMERATION_START__));\
    }\
    inline static iterator end()\
    {\
        return iterator(static_cast<int>(__ENUM_TYPE__::__ENUMERATION_END__));\
    }\
    __ENUM_TYPE__() = default;\
    __ENUM_TYPE__(const __ENUM_TYPE__& other) = default;\
    explicit __ENUM_TYPE__(int value):\
            _value(static_cast<value_t>(value))\
        {\
        }\
    __ENUM_TYPE__(value_t value):\
            _value(value)\
        {\
        }\
    inline int32_t to_int32()\
    {\
        return static_cast<int32_t>(_value);\
    }\
    inline uint32_t to_uint32()\
    {\
        return static_cast<int32_t>(_value);\
    }\
    inline int64_t to_int64()\
    {\
        return static_cast<int32_t>(_value);\
    }\
    inline uint64_t to_uint64()\
    {\
        return static_cast<int32_t>(_value);\
    }\
    bool operator==(value_t value)\
    {\
        return _value == value;\
    }\
    bool operator!=(value_t value)\
    {\
        return _value != value;\
    }\
    bool operator>(value_t value)\
    {\
        return _value > value;\
    }\
    bool operator<(value_t value)\
    {\
        return _value < value;\
    }\
    bool operator>=(value_t value)\
    {\
        return _value >= value;\
    }\
    bool operator<=(value_t value)\
    {\
        return _value <= value;\
    }\
    value_t& operator=(value_t value)\
    {\
        _value = value;\
        return _value;\
    }\
    value_t enum_form() const\
    {\
        return _value;\
    }\
    inline const char* to_string() const\
    {\
        return to_string(_value);\
    }\
private:\
    value_t _value;\
};

namespace vee {

} // !namespace vee

#endif // !_VEE_ENUMERATION_H_
