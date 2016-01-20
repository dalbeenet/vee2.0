#ifndef _VEE_CODE_TEXTFILE_H_
#define _VEE_CODE_TEXTFILE_H_

#include <vee/enumeration.h>
#include <vee/code/exception.h>
#include <cinttypes>
#include <string>
#include <list>

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

class textfile
{
public:
    using string = ::std::string;
    virtual ~textfile() = default;
    void read_from_file(const char* filepath) throw(...);
private:
    ::std::list<string> _container;
    charset _charset = charset::null;
};

} // !namespace vee

#endif // !_VEE_CODE_TEXTFILE_H_
