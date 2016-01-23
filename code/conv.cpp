#include <vee/code/codecvt.h>
#include <vee/io/exception.h>
#include <sys/stat.h>
#include <array>
#include <fstream>
//#pragma warning(disable:4512)
//#include <boost/locale.hpp>
//#pragma warning(default:4512)

namespace vee {

std::pair<charset, uint32_t> detect_charset(const char* filepath) throw(...)
{
    using namespace std;

    long size = 0;

    // Getting the size of a file
    {
        struct stat results;
        if (stat(filepath, &results) == NULL)
        {
            // The size of the file in bytes is in results.st_size
            size = results.st_size;
        }
        else
        {
            // An error occurred
            throw exceptions::file_open_failed();
        }
    }

    if (size < 2)
        return make_pair(charset::ascii, 0);

    // Opening a file
    array<unsigned char, 5> bom;
    try
    {
        ifstream file(filepath, ios::in | ios::binary);

        if (size > 4)
        {
            //file.seekg(ios::beg);
            bom.fill(0);
            file.read((char*)bom.data(), 4);
            if (bom[0] == 0x00 &&
                bom[1] == 0x00 &&
                bom[2] == 0xFE &&
                bom[3] == 0xFF)
                return make_pair(charset::utf32_big_endian, 4);
            else if (bom[0] == 0xFF &&
                     bom[1] == 0xFE &&
                     bom[2] == 0x00 &&
                     bom[3] == 0x00)
                     return make_pair(charset::utf32_little_endian, 4);
            else if (bom[0] == 0xDD &&
                     bom[1] == 0x73 &&
                     bom[2] == 0x66 &&
                     bom[3] == 0x73)
                     return make_pair(charset::utf_ebcdic, 4);
            else if (bom[0] == 0x84 &&
                     bom[1] == 0x31 &&
                     bom[2] == 0x95 &&
                     bom[3] == 0x33)
                     return make_pair(charset::gb_18030, 4);
            else if (bom[0] == 0x2B &&
                     bom[1] == 0x2F &&
                     bom[2] == 0x76)
            {
                switch (bom[4])
                {
                case 0x39:
                case 0x2B:
                case 0x2F:
                    return make_pair(charset::utf7, 4);
                case 0x38:
                    if (size > 5)
                    {
                        file.read((char*)bom.data() + 4, 1);
                        if (bom[4] == 0x2D)
                            return make_pair(charset::utf7, 5);
                    }
                    return make_pair(charset::utf7, 4);
                }
            }
        }

        if (size > 3)
        {
            file.seekg(ios::beg);
            bom.fill(0);
            file.read((char*)bom.data(), 3);
            if (bom[0] == 0xEF &&
                bom[1] == 0xBB &&
                bom[2] == 0xBF)
                return make_pair(charset::utf8, 3);
            else if (bom[0] == 0xF7 &&
                     bom[1] == 0x64 &&
                     bom[2] == 0x4C)
                     return make_pair(charset::utf1, 3);
            else if (bom[0] == 0x0E &&
                     bom[1] == 0xFE &&
                     bom[2] == 0xFF)
                     return make_pair(charset::scsu, 3);
            else if (bom[0] == 0xFB &&
                     bom[1] == 0xEE &&
                     bom[2] == 0x28)
                     return make_pair(charset::bocu_1, 3);
        }

        if (size > 2)
        {
            file.seekg(ios::beg);
            bom.fill(0);
            file.read((char*)bom.data(), 2);
            if (bom[0] == 0xFE &&
                bom[1] == 0xFF)
                return make_pair(charset::utf16_big_endian, 2);
            else if (bom[0] == 0xFF &&
                     bom[1] == 0xFE)
                     return make_pair(charset::utf16_little_endian, 2);
        }

        return make_pair(charset::ascii, 0);
    }
    catch (ifstream::failure&)
    {
        throw exceptions::file_open_failed();
    }
}

namespace conv {

//::std::string ascii_converter::from_ascii(const char* src, size_t len)
//{
//    ::std::string ret((char*)src, len);
//    return ret;
//}
//
//void ascii_converter::from_ascii(char* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//    memset(out, 0, out_buffer_size);
//    memmove(out, src, (out_buffer_size > src_len)?src_len:out_buffer_size - 1);
//}
//
//::std::string ascii_converter::from_utf8(const char* src, size_t len)
//{
//    return ::boost::locale::conv::from_utf(src, "UTF-8");
//}
//
//void ascii_converter::from_utf8(char* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//
//}
//
//::std::string ascii_converter::from_utf16le(const char* src, size_t len)
//{
//    /*const char16_t* str = (char16_t*)src;
//    std::mbstate_t state{};
//    std::string out(MB_CUR_MAX, '\0');
//    for (size_t n = 0; n < len; ++n)
//    {
//        int rc = std::c16rtomb(&out[0], str[n], &state);
//        std::cout << str[n] << " converted to [ ";
//        for (int x = 0; x < rc; ++x) std::cout << +(unsigned char)out[x] << ' ';
//        std::cout << "]\n";
//    }*/
//    return ::boost::locale::conv::from_utf(src, src + len, "UTF-16LE");
//}
//
//void ascii_converter::from_utf16le(char* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//
//}
//
//::std::string ascii_converter::from_utf16be(const char* src, size_t len)
//{
//    return ::boost::locale::conv::from_utf(src, src + len, "ASCII");
//}
//
//void ascii_converter::from_utf16be(char* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//
//}
//
//::std::string ascii_converter::to_ascii(const char* src, size_t len)
//{
//    ::std::string ret((char*)src, len);
//    return ret;
//}
//
//void ascii_converter::to_ascii(void* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//    memset(out, 0, out_buffer_size);
//    memmove(out, src, (out_buffer_size > src_len) ? src_len : out_buffer_size - 1);
//}
//
//::std::string ascii_converter::to_utf8(const char* src, size_t len)
//{
//    return ::boost::locale::conv::to_utf<char>(src, "ASCII");
//}
//
//void ascii_converter::to_utf8(void* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//
//}
//
//::std::u16string ascii_converter::to_utf16le(const char* src, size_t len)
//{
//    //return ::boost::locale::conv::to_utf<char16_t>(src, "UTF-16LE");
//    return ::std::u16string();
//}
//
//void ascii_converter::to_utf16le(void* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//
//}
//
//::std::u16string ascii_converter::to_utf16be(const char* src, size_t len)
//{
//    //return ::boost::locale::conv::to_utf<char16_t>(src, "UTF-16BE");
//    return ::std::u16string();
//}
//
//void ascii_converter::to_utf16be(void* const out, size_t out_buffer_size, const char* src, size_t src_len)
//{
//
//}

} // !namespace converter


} // !namespace vee