#include <vee/strutils.h>
#include <boost/tokenizer.hpp>

namespace vee {

namespace strutils {

::std::vector<::std::string> split(const ::std::string& string, const char* token)
{
    using char_separator = ::boost::char_separator<char>;
    using tokenizer = boost::tokenizer<char_separator>;

    // Parse raw data per lines (token: \n)
    ::std::vector<::std::string> ret;
    {
        char_separator sep(token);
        tokenizer tok(string, sep);
        for (auto& it : tok)
        {
            ret.push_back(it);
        }
    }

    return ret;
}

char* stristr(const char* s1, const char* s2)
{
    do
    {
        const char* h = s1;
        const char* n = s2;
        while (tolower((unsigned char)*h) == tolower((unsigned char)*n) && *n)
        {
            h++;
            n++;
        }
        if (*n == 0)
        {
            return (char *)s1;
        }
    }
    while (*s1++);
    return nullptr;
};

} // !namespace strutils

} // !namespace strutils