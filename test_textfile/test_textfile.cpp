#include <vee/code/textfile.h>
#include <vee/filesystem/path.h>
using namespace vee;
using namespace std;
namespace fs = ::vee::filesystem;

#pragma warning(disable:4996)
int main()
{
    auto list = fs::get_file_list("testcases", true);
    for (auto& s : list)
    {
        printf("%s\n", s.c_str());
    }
    return 0;
}