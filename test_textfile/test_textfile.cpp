#include <vee/code/textfile.h>
#include <vee/filesystem/path.h>
#include <vee/enumeration.h>
#include <iostream>
namespace fs = ::vee::filesystem;

#pragma warning(disable:4996)
int main()
{
    auto list = fs::get_file_list("testcases", true);
    for (auto& s : list)
    {
        printf("%s\n", s.c_str());
    }
    
    puts(to_string(vee::charset::ascii));

    return 0;
}