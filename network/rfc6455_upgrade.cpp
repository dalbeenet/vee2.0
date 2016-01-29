#include <vee/network/net.h>
#include <string>
#include <map>

namespace vee {

namespace net {

namespace rfc6455 {

struct http_upgrade_request
{
    /* The Request-URI of the GET method is used to 
     * identify the endpoint of the Websocket connection */
    ::std::string request_uri;
    ::std::map<::std::string, ::std::string> container;
    ::std::string& host() const;
    ::std::string& upgrade() const;
    ::std::string& connection() const;
    ::std::string& sec_websocket_key() const;
    ::std::string& origin() const;
    ::std::string& sec_websocket_protocol() const;
    ::std::string& sec_websocket_version() const;
};

} // !namespace rfc6455

} // !namespace net

} // !namesapce vee