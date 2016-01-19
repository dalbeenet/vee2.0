#include <vee/filesystem/path.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
//#include <boost/progress.hpp>

namespace vee {

namespace filesystem {

::std::vector<::std::string> get_file_list(const char* path, bool include_directories) throw(...)
{
    ::std::vector<::std::string> ret;

    namespace fs = boost::filesystem;
    fs::path full_path(fs::initial_path<fs::path>());

    if (strlen(path) > 1)
        full_path = fs::system_complete(fs::path(path));
    else
        throw exceptions::invalid_path();

    if (!fs::exists(full_path))
    {
        throw exceptions::invalid_path();
    }

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if (fs::is_directory(full_path))
    {
        /*std::cout << "\nIn directory: "
            << full_path.string() << "\n\n";*/

        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(full_path);
             dir_itr != end_iter;
             ++dir_itr)
        {
            try
            {
                if (fs::is_directory(dir_itr->status()))
                {
                    ++dir_count;
                    if (include_directories)
                        ret.push_back(dir_itr->path().filename().string());
                    //std::cout << dir_itr->path().filename() << " [directory]\n";
                }
                else if (fs::is_regular_file(dir_itr->status()))
                {
                    ++file_count;
                    ret.push_back(dir_itr->path().filename().string());
                    //std::cout << dir_itr->path().filename() << "\n";
                }
                else
                {
                    ++other_count;
                    //std::cout << dir_itr->path().filename() << " [other]\n";
                }

            }
            catch (const std::exception&)
            {
                ++err_count;
                //std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
        /*std::cout << "\n" << file_count << " files\n"
            << dir_count << " directories\n"
            << other_count << " others\n"
            << err_count << " errors\n";*/
    }
    else // must be a file
    {
        //std::cout << "\nFound: " << full_path.file_string() << "\n";
        ret.push_back(full_path.string());
        return ret;
    }

    return ret;
}

bool exists(const char* path)
{
    namespace fs = boost::filesystem;
    return fs::exists(path);
}

bool is_directory(const char* path)
{
    namespace fs = boost::filesystem;
    if (!fs::exists(path))
    {
        throw exceptions::invalid_path();
    }
    return fs::is_directory(path);
}

bool is_regular_file(const char* path)
{
    namespace fs = boost::filesystem;
    if (!fs::exists(path))
    {
        throw exceptions::invalid_path();
    }
    return fs::is_regular_file(path);
}

} // !namespace filesystem

} // !namespace vee