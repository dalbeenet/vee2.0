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

// KNOWN ISSUE: ���ڿ��� �����ϰ� �ִ� map�� �����Ҵ�ǰ� �������� �ʴ´�.
// �� ������ ���μ����� ����� ���� to_string()�� ��ȿ�ؾ��ϱ� ���� -> Data ���ǿ� �����ϸ� �ȵ�
// ���μ����� ����Ǹ� OS�� heap�� ������ ���̹Ƿ� ���� ������ ���� �������� �����
// MSVC, Win10�� �ƴ� ȯ�濡���� ���ڿ� �ʱ�ȭ�� ����� �� �����ϴ��� Ȯ���ؾ��� �ʿ䰡 ����
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
