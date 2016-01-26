#include <vee/code/textfile.h>
#include <vee/filesystem/path.h>
#include <vee/enumeration.h>
#include <iostream>

Enumeration(numbers2, 1, first, second, third);

#pragma warning(disable:4996)
int main()
{
    try
    {
        auto list = ::vee::filesystem::get_file_list("testcases", true);
        for (auto& s : list)
        {
            ::std::string relative_path = "testcases\\";
            relative_path.append(s);
            printf("%s (%s)\n",
                   s.c_str(),
                   ::vee::detect_charset(relative_path.c_str()).first.to_string());
            /*::vee::textfile<::vee::conv::ascii_converter> txt;
            txt.read_from_file(relative_path.c_str());*/
        }
    }
    catch (::vee::exception& e)
    {
        puts(e.to_string());
    }

    numbers2 enum_instance = numbers2::second;
    puts("----------------------------------"); 
    puts("test case: Switch statement");
    switch (enum_instance.enum_form())
    {
    case numbers2::first:
        puts(enum_instance.to_string());
        break;
    case numbers2::second:
        puts(enum_instance.to_string());
        break;
    case numbers2::third:
        puts(enum_instance.to_string());
        break;
    default:    
        break;
    }
    puts("----------------------------------");
    puts("test case: STL Style for loop");
    for (auto iter = numbers2::begin(); iter != numbers2::end(); ++iter)
    {
        puts(iter.to_string());
    }
    puts("----------------------------------");
    puts("test case: C++11 Range for statement");
    for (auto value : numbers2())
    {
        printf("enumerated value: %d\n", value);
    }
    puts("----------------------------------");
    for (auto iter = ::vee::charset::begin(); iter != ::vee::charset::end(); ++iter)
    {
        //puts(iter.to_string());
    }

    return 0;
}