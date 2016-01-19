#ifndef _VEE_ENUMERATION_H_
#define _VEE_ENUMERATION_H_

#include <cinttypes>
#include <vee/strutils.h>
#include <map>

namespace vee {

#define Enumeration(EnumType, integral_type, offset, First, ...)\
enum class EnumType { First = offset, __VA_ARGS__ };\
inline const char* to_string(EnumType value)\
{\
    auto gen = []() -> ::std::map<EnumType, ::std::string>\
    {\
        ::std::string raw(#First);\
        raw.append(", ");\
        raw.append(#__VA_ARGS__);\
        ::std::vector<::std::string> strings = ::vee::strutils::split(raw, ",");\
        ::std::map<EnumType, ::std::string> result;\
        integral_type key = offset;\
        for (int i = 0; i < strings.size(); ++i)\
        {\
            result.insert(make_pair(static_cast<EnumType>(key++), ::vee::strutils::trim(strings[i])));\
        }\
        return result;\
    };\
    static ::std::map<EnumType, ::std::string> map = gen();\
    return map[value].c_str();\
}

} // !namespace vee

#endif // !_VEE_ENUMERATION_H_
