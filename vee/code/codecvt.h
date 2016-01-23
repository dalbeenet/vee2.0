#ifndef _VEE_CODECVT_H_
#define _VEE_CODECVT_H_

#include <string>
#include <vector>
#include <cinttypes>
#include <vee/base.h>
#include <vee/enumeration.h>

namespace vee {

Enumeration(charset, 0,
            null,
            ascii,
            utf8,
            utf16_big_endian,
            utf16_little_endian,
            utf32_big_endian,
            utf32_little_endian,
            utf7,
            utf1,
            utf_ebcdic,
            scsu,
            bocu_1,
            gb_18030);

std::pair<charset, uint32_t> detect_charset(const char* filepath) throw(...);

namespace conv {

class sha1 abstract
{
public:
    static ::std::vector<unsigned char> hashing(const ::std::string& dst);
    static void print(::std::vector<unsigned char>& dst);
};

class base64 abstract
{
public:
    static ::std::string decode(const ::std::string &dst);
    static ::std::string decode(const ::std::vector<unsigned char> &dst);
    static ::std::string encode(const ::std::string& dst);
    static ::std::string encode(const ::std::vector<unsigned char>& data);
};

//class ascii_converter abstract
//{
//public:
//    using string_container = ::std::string;
//    static ::std::string from_ascii(const char* src, size_t len);
//    static ::std::string from_utf8(const char* src, size_t len);
//    static ::std::string from_utf16le(const char* src, size_t len);
//    static ::std::string from_utf16be(const char* src, size_t len);
//    static void from_ascii(char* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static void from_utf8(char* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static void from_utf16le(char* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static void from_utf16be(char* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static ::std::string to_ascii(const char* src, size_t len);
//    static ::std::string to_utf8(const char* src, size_t len);
//    static ::std::u16string to_utf16le(const char* src, size_t len);
//    static ::std::u16string to_utf16be(const char* src, size_t len);
//    static void to_ascii(void* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static void to_utf8(void* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static void to_utf16le(void* const out, size_t out_buffer_size, const char* src, size_t src_len);
//    static void to_utf16be(void* const out, size_t out_buffer_size, const char* src, size_t src_len);
//};

} // !namespace converter

} // !namespace vee

#endif // !_VEE_CODECVT_H_
