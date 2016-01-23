#include <vee/filesystem/path.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <sys/stat.h>
//#include <boost/progress.hpp>

namespace vee {

namespace filesystem {

::std::vector<::std::string> get_file_list(const char* path, bool include_directories, bool opt_get_full_path) throw(...)
{
    ::std::vector<::std::string> ret;

    namespace fs = boost::filesystem;
    fs::path directory(fs::initial_path<fs::path>());

    if (strlen(path) > 1)
        directory = fs::system_complete(fs::path(path));
    else
        throw exceptions::invalid_path();

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if (fs::is_directory(directory))
    {
        /*std::cout << "\nIn directory: "
            << full_path.string() << "\n\n";*/

        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_itr(directory);
             dir_itr != end_iter;
             ++dir_itr)
        {
            try
            {
                if (fs::is_directory(dir_itr->status()))
                {
                    ++dir_count;
                    if (include_directories)
                    {
                        if (opt_get_full_path)
                        {
                            ret.push_back(directory.string() + "\\" + dir_itr->path().filename().string());
                        }
                        else
                        {
                            ret.push_back(dir_itr->path().filename().string());
                        }
                    }
                    //std::cout << dir_itr->path().filename() << " [directory]\n";
                }
                else if (fs::is_regular_file(dir_itr->status()))
                {
                    ++file_count;
                    if (opt_get_full_path)
                    {
                        ret.push_back(directory.string() + "\\" + dir_itr->path().filename().string());
                    }
                    else
                    {
                        ret.push_back(dir_itr->path().filename().string());
                    }
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
        throw exceptions::invalid_path();
    }

    return ret;
}

bool exists(const char* path) __noexcept
{
    namespace fs = boost::filesystem;
    fs::path full_path(fs::initial_path<fs::path>());

    if (strlen(path) > 1)
        full_path = fs::system_complete(fs::path(path));

    return fs::exists(full_path);
}

bool is_directory(const char* path)
{
    if (!exists(path))
        throw exceptions::file_does_not_exist();

    namespace fs = boost::filesystem;
    fs::path full_path(fs::initial_path<fs::path>());

    if (strlen(path) > 1)
        full_path = fs::system_complete(fs::path(path));

    return fs::is_directory(full_path);
}

bool is_regular_file(const char* path)
{
    if (!exists(path))
        throw exceptions::file_does_not_exist();

    namespace fs = boost::filesystem;
    fs::path full_path(fs::initial_path<fs::path>());

    if (strlen(path) > 1)
        full_path = fs::system_complete(fs::path(path));
    return fs::is_regular_file(full_path);
}

long get_file_size(const char* filepath) throw(...)
{
    if (!filesystem::exists(filepath))
        throw exceptions::file_does_not_exist();

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

    return size;
}

} // !namespace filesystem

} // !namespace vee