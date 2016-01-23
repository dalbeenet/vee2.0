#ifndef _VEE_CODE_TEXTFILE_H_
#define _VEE_CODE_TEXTFILE_H_

#include <vee/code/codecvt.h>
#include <vee/code/exception.h>
#include <vee/filesystem/path.h>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <list>

namespace vee {

template <class StringConverter>
class textfile: public StringConverter
{
public:
    template< typename T >
    struct array_deleter
    {
        void operator ()(T const * p)
        {
            delete[] p;
        }
    };
    using base_t = typename StringConverter;
    using string_container = typename base_t::string_container;
    virtual ~textfile() = default;
    void read_from_file(const char* filepath) throw(...)
    {
        auto charset_info = detect_charset(filepath);
        _charset = charset_info.first;
        uint32_t padding = charset_info.second;
        long size = filesystem::get_file_size(filepath);
        
        using namespace std;
        ifstream file(filepath, ios::in | ios::binary);
        
        if (size - padding < 0)
            return; // Empty text file
        
        file.seekg(padding);
        shared_ptr<char> buffer(new char[size - padding], std::default_delete<char[]>());
        file.read(buffer.get(), size - padding);

        string_container sub_result;

        switch (_charset.enum_form())
        {
        case charset::ascii:
            sub_result = base_t::from_ascii(buffer.get(), size);
            break;
        case charset::utf8:
            sub_result = base_t::from_utf8(buffer.get(), size);
            break;
        case charset::utf16_big_endian:
            sub_result = base_t::from_utf16be(buffer.get(), size);
            break;
        case charset::utf16_little_endian:
            sub_result = base_t::from_utf16le(buffer.get(), size);
            break;
        }
    }
private:
    ::std::list<typename string_container> _container;
    charset _charset = charset::null;
};

} // !namespace vee

#endif // !_VEE_CODE_TEXTFILE_H_
