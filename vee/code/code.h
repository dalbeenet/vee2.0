#ifndef _VEE_CODE_H_
#define _VEE_CODE_H_

#include <string>
#include <vector>
#include <vee/base.h>

namespace vee {

class sha1: static_class
{
public:
    static ::std::vector<unsigned char> hashing(const ::std::string& dst);
    static void print(::std::vector<unsigned char>& dst);
};

class base64: static_class
{
public:
    static ::std::string decode(const ::std::string &dst);
    static ::std::string decode(const ::std::vector<unsigned char> &dst);
    static ::std::string encode(const ::std::string& dst);
    static ::std::string encode(const ::std::vector<unsigned char>& data);
};

} // !namespace vee

#endif // !_VEE_CODE_H_
