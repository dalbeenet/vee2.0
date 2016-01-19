#ifndef VEE_FILESYSTEM_PATH_H_
#define VEE_FILESYSTEM_PATH_H_

#include <string>
#include <vector>
#include <vee/filesystem/exception.h>

namespace vee {

namespace filesystem {

::std::vector<::std::string> get_file_list(const char* path, bool include_directories) throw(...);

bool exists(const char* path);

bool is_directory(const char* path) throw(...);

bool is_regular_file(const char* path) throw(...);

} // !namespace filesystem

} // !namespace vee

#endif // !VEE_FILESYSTEM_PATH_H_
