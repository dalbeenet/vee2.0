#include <vee/network/exception.h>
#include <cstdarg>
#include <array>

namespace vee {

namespace exceptions {

namespace rfc6455 {

header_field_not_found::header_field_not_found(const char* header_field_name, ...)
{
    std::array<char, description_capacity> buffer;
    buffer.fill(0);
    va_list ap;
    va_start(ap, header_field_name);
    vsprintf_s(buffer.data(), buffer.size(), header_field_name, ap);
    sprintf_s(_desc, "Header \"%s\" field not found in HTTP Upgrade request", buffer.data());
    va_end(ap);
}

} // !namespace rfc6455

} // !namespace exception

} // !namespace vee