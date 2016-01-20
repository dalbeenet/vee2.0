#include <vee/code/textfile.h>

void foo()
{
    for (auto iter = ::vee::charset::begin(); iter != ::vee::charset::end(); ++iter)
    {
        puts(iter.to_string());
    }
}