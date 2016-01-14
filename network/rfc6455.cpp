#include <vee/network/net.h>
#include <vee/strutils.h>
#include <vee/code/code.h>
#include <vee/network/tcp_stream.h>
#include <boost/tokenizer.hpp>
#include <boost/endian/conversion.hpp>
#include <random>

namespace vee {

namespace net {

namespace rfc6455 {

namespace handshake {

#pragma region client_request implementation
void client_request::print() const
{
    printf("%s\nHost: %s\nUpgrade: %s\nConnection: %s\nOrigin: %s\nSec-Websocket-Key: %s\nSec-Websocket-Protocol: %s\nSec-Websocket-Version: %s\n",
           request_uri.data(),
           host.data(),
           upgrade.data(),
           connection.data(),
           origin.data(),
           sec_websocket_key.data(),
           sec_websocket_protocol.data(),
           sec_web_socket_version.data()
           );
}

void client_request::parse(const char* data)
{
    return parse(::std::string(data));
}

void client_request::parse(::std::string& data)
{
    using char_separator = ::boost::char_separator<char>;
    using tokenizer = boost::tokenizer<char_separator>;

    // Parse raw data per lines (token: \n)
    ::std::vector<::std::string> data_by_lines;
    {
        char_separator sep("\r\n");
        tokenizer tok(data, sep);
        for (auto& it : tok)
        {
            data_by_lines.push_back(it);
        }
    }

    auto get_value = [](const ::std::string& dst)-> std::pair<bool, ::std::string>
    {
        auto pos = dst.find(':');
        if (pos == ::std::string::npos)
        {
            return std::make_pair(false, "");
        }
        else
        {
            ::std::string buffer(dst.substr(pos + 1));
            ::std::string result = strutils::trim(buffer);
            return std::make_pair(true, std::move(result));
        }
    };

    /*printf("DATA RECIEVED --------------\n");
    for (auto& it : data_by_lines)
    {
    printf("%s\n", it.data());
    }*/

    for (auto& it : data_by_lines)
    {
        auto set_value = [](::std::pair<bool, ::std::string>& result, ::std::string& dst) -> void
        {
            if (result.first == true)
            {
                dst = std::move(result.second);
            }
        };

        if (strstr(it.data(), "GET"))
        {
            request_uri = it;
        }
        else if (strstr(it.data(), "Host:"))
        {
            set_value(get_value(it), host);
        }
        else if (strstr(it.data(), "Upgrade:"))
        {
            set_value(get_value(it), upgrade);
        }
        else if (strstr(it.data(), "Connection:"))
        {
            set_value(get_value(it), connection);
        }
        else if (strstr(it.data(), "Sec-WebSocket-Key:"))
        {
            set_value(get_value(it), sec_websocket_key);
        }
        else if (strstr(it.data(), "Origin:"))
        {
            set_value(get_value(it), origin);
        }
        else if (strstr(it.data(), "Sec-WebSocket-Protocol:"))
        {
            set_value(get_value(it), sec_websocket_protocol);
        }
        else if (strstr(it.data(), "Sec-WebSocket-Version:"))
        {
            set_value(get_value(it), sec_web_socket_version);
        }
        else
        {
            continue;
        }
    }
}

bool client_request::is_valid() const
{
    //! 다음이 생략됨
    //! HOST 헤더 유효성 검사
    //! Sec-Websocket-Key 헤더 유효성 검사
    //! Origin 헤더 유효성 검사
    if (
        (strutils::stristr(request_uri.data(), "GET") == nullptr) ||
        (strutils::stristr(request_uri.data(), "HTTP/1.1") == nullptr) ||
        (strutils::stristr(upgrade.data(), "Websocket") == nullptr) ||
        (strutils::stristr(connection.data(), "Upgrade") == nullptr) ||
        (strutils::stristr(sec_web_socket_version.data(), "13") == nullptr)
        )
    {
        return false;
    }
    return true;
}

void client_request::clear()
{
    request_uri.clear();
    host.clear();
    upgrade.clear();
    connection.clear();
    origin.clear();
    sec_websocket_key.clear();
    sec_web_socket_version.clear();
    sec_websocket_protocol.clear();
}

::std::string client_request::binary_pack() const
{
    ::std::string pack;
    pack.append(request_uri);
    pack.append("\r\nHost: ");
    pack.append(host);
    pack.append("\r\nUpgrade: ");
    pack.append(upgrade);
    pack.append("\r\nConnection: ");
    pack.append(connection);
    pack.append("\r\nSec-WebSocket-Key: ");
    pack.append(sec_websocket_key);
    pack.append("\r\nOrigin: ");
    pack.append(origin);
    pack.append("\r\nSec-WebSocket-Protocol: ");
    pack.append(sec_websocket_protocol);
    pack.append("\r\nSec-WebSocket-Version: ");
    pack.append(sec_web_socket_version);
    pack.append("\r\n\r\n");
    return pack;
}
#pragma endregion

#pragma region server_response implementation
void server_response::parse(const char* data)
{
    return parse(::std::string(data));
}

void server_response::parse(::std::string& data)
{
    using char_separator = ::boost::char_separator<char>;
    using tokenizer = boost::tokenizer<char_separator>;

    // Parse raw data per lines (token: \n)
    ::std::vector<::std::string> data_by_lines;
    {
        char_separator sep("\r\n");
        tokenizer tok(data, sep);
        for (auto& it : tok)
        {
            data_by_lines.push_back(it);
        }
    }

    auto get_value = [](const ::std::string& dst)-> std::pair<bool, ::std::string>
    {
        auto pos = dst.find(':');
        if (pos == ::std::string::npos)
        {
            return std::make_pair(false, "");
        }
        else
        {
            ::std::string buffer(dst.substr(pos + 1));
            ::std::string result = strutils::trim(buffer);
            return std::make_pair(true, std::move(result));
        }
    };

    for (auto& it : data_by_lines)
    {
        auto set_value = [](::std::pair<bool, ::std::string>& result, ::std::string& dst) -> void
        {
            if (result.first == true)
            {
                dst = std::move(result.second);
            }
        };

        if (strstr(it.data(), "HTTP/1.1 101 Switching Protocols"))
        {
            http_status = it;
        }
        else if (strstr(it.data(), "Upgrade:"))
        {
            set_value(get_value(it), upgrade);
        }
        else if (strstr(it.data(), "Connection:"))
        {
            set_value(get_value(it), connection);
        }
        else if (strstr(it.data(), "Sec-WebSocket-Accept:"))
        {
            set_value(get_value(it), sec_websocket_accept);
        }
        else
        {
            continue;
        }
    }
}

void server_response::print() const
{
    printf("%s\nUpgrade: %s\nConnection: %s\nSec-Websocket-Accept: %s\n", http_status.data(), upgrade.data(), connection.data(), sec_websocket_accept.data());
}

void server_response::clear()
{
    http_status = "HTTP/1.1 101 Switching Protocols";
    upgrade = "websocket";
    connection = "Upgrade";
    sec_websocket_accept.clear();
}

::std::string server_response::binary_pack() const
{
    ::std::string ret;
    //ret += "HTTP/1.1 101 Switching Protocols\r\nConnection: Upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Origin: http://jjoriping.ufree.kr\r\nSec-WebSocket-Location: ws://localhost:8001/\r\nSec-WebSocket-Accept: ";
    ret.append(http_status);
    ret.append("\r\nUpgrade: ");
    ret.append(upgrade);
    ret.append("\r\nConnection: ");
    ret.append(connection);
    ret.append("\r\nSec-WebSocket-Accept: ");
    ret.append(sec_websocket_accept);
    //ret.append("\r\nSec-WebSocket-Location: ws://localhost:1992/");
    ret.append("\r\n\r\n");
    return ret;
}
#pragma endregion

} // !namespace handshake

namespace dataframe {

void header::analyze(const unsigned char* raw_data, const size_t length)
{
    if (!raw_data) return;

    unsigned int shift = 0;
    // analyze a first byte
    memmove(&fin_opcode, raw_data + shift++, 1);
    fin = ((fin_opcode & 0x80) >> 7) ? true : false;
    switch ((fin_opcode & 0x0f))
    {
    case 0x0:
        opcode = opcode_id::continuation_frame;
        break;
    case 0x1:
        opcode = opcode_id::text_frame;
        break;
    case 0x2:
        opcode = opcode_id::binary_frame;
        break;
    case 0x8:
        opcode = opcode_id::connnection_close;
        break;
    case 0x9:
        opcode = opcode_id::ping;
        break;
    case 0xA:
        opcode = opcode_id::pong;
        break;
    default:
        opcode = opcode_id::reserved_for_further;
        break;
    }
    //TODO: RSV1~3 (Extension flag) processing 

    // analyze a second byte
    memmove(&mask_payload_len, raw_data + shift++, 1);
    use_mask = ((mask_payload_len & 0x80) >> 7) ? true : false;
    payload_len = (mask_payload_len & 0x7f);
    if (payload_len == 0x7e)
    {
        memmove(&extended_payload_len16, raw_data + shift, 2);
        payload_len = boost::endian::big_to_native(extended_payload_len16);
        shift += 2;
        //x use_extended16_payload = true;
    }
    else if (payload_len == 0x7f)
    {
        memmove(&extended_payload_len64, raw_data + shift, 8);
        payload_len = boost::endian::big_to_native(extended_payload_len64);
        shift += 8;
        //x use_extended64_payload = true;
    }
    else
    {
        //x use_extended16_payload = false;
        //x use_extended64_payload = false;
    }

    // Getting a mask key
    if (use_mask == true)
    {
        //x ::std::array<unsigned char, 4> mask_key_buffer;
        //memmove(mask_key_buffer.data(), raw_data + shift, 4);
        //x std::reverse_copy(mask_key_buffer.begin(), mask_key_buffer.end(), masking_key.begin());
        memmove(masking_key.data(), raw_data + shift, 4);
        shift += 4;
    }

    payload_pos = shift;

    //x Getting a payload data
    //x payload.clear();
    //x payload.resize(length - shift);
    //x memmove(&payload[0], raw_data + shift, length - shift);

    return;
}

uint32_t header::binary_pack(opcode_id opcode, unsigned char* out_buffer) const
{
    uint32_t shift = 0;

    int8_t fin_opcode_block = 0;
    switch (opcode)
    {
    case opcode_id::continuation_frame:
        fin_opcode_block = 0x0;
        break;
    case opcode_id::text_frame:
        fin_opcode_block = 0x1;
        break;
    case opcode_id::binary_frame:
        fin_opcode_block = 0x2;
        break;
    case opcode_id::connnection_close:
        fin_opcode_block = 0x8;
        break;
    case opcode_id::ping:
        fin_opcode_block = 0x9;
        break;
    case opcode_id::pong:
        fin_opcode_block = 0xA;
        break;
    case opcode_id::reserved_for_further:
        fin_opcode_block = 0x2; // Websocket stream does not support this operation. set to default value(binary frame).
        break;
    default:
        fin_opcode_block = 0x2; // defualt value is binary frame
        break;
    }

    (fin) ? (fin_opcode_block |= 0x80) : (fin_opcode_block |= 0x00);
    memmove(out_buffer + shift++, &fin_opcode_block, 1);

    int8_t mask_payload_len_block = 0;
    if (payload_len >= 0x7e)
    {
        if (payload_len <= INT16_MAX)
            mask_payload_len_block = 0x7e;
        else
            mask_payload_len_block = 0x7f;
    }
    else
        mask_payload_len_block = (int8_t)payload_len;
    (use_mask) ? (mask_payload_len_block |= 0x80) : (mask_payload_len_block |= 0x00);
    memmove(out_buffer + shift++, &mask_payload_len_block, 1);

    if ((mask_payload_len_block & (~0x80)) == 0x7e)
    {
        int16_t extended_payload_len_16 = boost::endian::native_to_big((int16_t)payload_len);
        memmove(out_buffer + shift, &extended_payload_len_16, 2);
        shift += 2;
    }
    else if ((mask_payload_len_block & (~0x80)) == 0x7f)
    {
        int64_t extended_payload_len_64 = boost::endian::native_to_big((int64_t)payload_len);
        memmove(out_buffer + shift, &extended_payload_len_64, 8);
        shift += 8;
    }

    if (use_mask)
    {
        memmove(out_buffer + shift, masking_key.data(), 4);
        shift += 4;
    }

    return shift; // same as length of header
}

uint32_t header::binary_pack_size() const
{
    // FOLLOWING PROCESS as same as binary_pack() method, but This function doesn't call memmove()
    uint32_t shift = 0;

    //x int8_t fin_opcode_block = (int8_t)0x1; // must be binary frame (net_stream interface requirement)
    //x (fin) ? (fin_opcode_block |= 0x80) : (fin_opcode_block |= 0x00);
    //x memmove(out_buffer + shift++, &fin_opcode_block, 1);
    shift += 1;

    int8_t mask_payload_len_block = 0;
    if (payload_len >= 0x7e)
    {
        if (payload_len <= INT16_MAX)
            mask_payload_len_block = 0x7e;
        else
            mask_payload_len_block = 0x7f;
    }
    else
        mask_payload_len_block = (int8_t)payload_len;
    (use_mask) ? (mask_payload_len_block |= 0x80) : (mask_payload_len_block |= 0x00);
    //x memmove(out_buffer + shift++, &mask_payload_len_block, 1);
    shift += 1;

    if ((mask_payload_len_block & (~0x80)) == 0x7e)
    {
        int16_t extended_payload_len_16 = boost::endian::native_to_big((int16_t)payload_len);
        //x memmove(out_buffer + shift, &extended_payload_len_16, 2);
        shift += 2;
    }
    else if ((mask_payload_len_block & (~0x80)) == 0x7f)
    {
        int64_t extended_payload_len_64 = boost::endian::native_to_big((int64_t)payload_len);
        //x memmove(out_buffer + shift, &extended_payload_len_64, 8);
        shift += 8;
    }

    if (use_mask)
    {
        //x memmove(out_buffer + shift, masking_key.data(), 4);
        shift += 4;
    }

    return shift; // same as length of header
}

void header::randomize_mask()
{
    static ::std::mt19937 engine((unsigned int)time(NULL));
    static ::std::uniform_int_distribution<int> distribution(0, 127);
    static auto generator = ::std::bind(distribution, engine);
    for (unsigned char& octet : masking_key)
        octet = generator();
    printf("DEBUG> Masking key: %d %d %d %d\n", masking_key[0], masking_key[1], masking_key[2], masking_key[3]);
}

} // !namespace dataframe

namespace impl {

class xwebsocket_server;
class xwebsocket_stream;

class xwebsocket_server: public net_server
{
    DISALLOW_COPY_AND_ASSIGN(xwebsocket_server);
public:
    using tcp_server = tcp::tcp_server;
    using tcp_stream = tcp::tcp_stream;
    using socket_t = ::boost::asio::ip::tcp::socket;
    using endpoint = ::boost::asio::ip::tcp::endpoint;
    using io_service_t = ::boost::asio::io_service;
    xwebsocket_server(unsigned short port, io_service_t& io_service = io_service_sigleton::get().io_service());
    xwebsocket_server(xwebsocket_server&& other);
    virtual ~xwebsocket_server();
    virtual void close() override;
    virtual net_stream::shared_ptr accept() throw(...) override;
    virtual void async_accept(std::function<VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE> e) override;
    inline io_service_t& get_io_service() const
    {
        return _host_io_service;
    }
protected:
    static bool _handshake(net_stream& raw_socket);
protected:
    ::boost::asio::io_service& _host_io_service;
    tcp_server _tcp_server;
};

xwebsocket_server::xwebsocket_server(unsigned short port, io_service_t& io_service /* = io_service_sigleton::get().io_service() */):
_host_io_service(io_service),
_tcp_server(port, io_service)
{

}

xwebsocket_server::xwebsocket_server(xwebsocket_server&& other):
_host_io_service(other._host_io_service),
_tcp_server(static_cast<xwebsocket_server::tcp_server&&>(other._tcp_server))
{

}

xwebsocket_server::~xwebsocket_server()
{

}

void xwebsocket_server::close()
{

}

net_stream::shared_ptr xwebsocket_server::accept() throw(...)
{
    ::std::shared_ptr<tcp_stream> raw_tcp_stream = ::std::static_pointer_cast<tcp_stream>(_tcp_server.accept());
    bool handshake_result = _handshake(*raw_tcp_stream);
    if (handshake_result == true)
    {
        ::std::shared_ptr<net_stream> stream = ::std::static_pointer_cast<net_stream>(::std::make_shared<xwebsocket_stream>(static_cast<tcp_stream&&>(*raw_tcp_stream), false));
        return stream;
    }
    else
    {
        throw exceptions::rfc6455_handshake_failure();
    }
}

void xwebsocket_server::async_accept(std::function<VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE> e)
{
    auto handle_accept = [e](net::op_result& result, ::std::shared_ptr<net_stream>& stream)
    {
        if (!result.is_success)
        {
            e(result, stream);
            return;
        }
        else
        {
            ::std::shared_ptr<tcp_stream> raw_tcp_stream = ::std::static_pointer_cast<tcp_stream>(stream);
            bool handshake_result = xwebsocket_server::_handshake(*raw_tcp_stream);
            if (!handshake_result)
            {
                result.is_success = false;
                result.log.append("\r\nrfc6455_client_handshake_failure!");
            }
            else
            {
                ::std::shared_ptr<net_stream> stream = ::std::static_pointer_cast<net_stream>(::std::make_shared<xwebsocket_stream>(static_cast<tcp_stream&&>(*raw_tcp_stream), false));
                e(result, stream);
            }
        }
    };
    _tcp_server.async_accept(handle_accept);
}

bool xwebsocket_server::_handshake(net_stream& raw_socket)
{
    try
    {
        ::std::array<unsigned char, 4096> buffer;
        raw_socket.read_some(buffer.data(), buffer.size());

        // Parsing HTTP header
        handshake::client_request header;
        header.parse(::std::string((char*)buffer.data()));

        // Validating the client header
        if (header.is_valid() == false)
        {
            return false;
        }

        printf("websocket_server> [RFC6455 Handshake] Websocket client request\n");
        PRINT_LINE;
        header.print();
        PRINT_LINE;

        // Append magic GUID
        ::std::string magic_string(header.sec_websocket_key);
        magic_string.append(RFC4122_GUID);
        // Hashing via SHA-1
        auto hash_code = sha1::hashing(magic_string);
        sha1::print(hash_code);
        // Encoding via Base64
        ::std::string hash_code_base64 = base64::encode(hash_code);
        printf("To base64: %s\n", hash_code_base64.data());
        // Make data for response
        handshake::server_response response;
        response.http_status = "HTTP/1.1 101 Switching Protocols";
        response.upgrade = "websocket";
        response.connection = "Upgrade";
        response.sec_websocket_accept = hash_code_base64;
        ::std::string response_data = response.binary_pack();

        printf("websocket_server> [RFC6455 Handshake] Websocket server response\n");
        PRINT_LINE;
        puts(response_data.data());
        PRINT_LINE;
        // Send response data to client
        raw_socket.write_some((unsigned char*)response_data.data(), response_data.size());
    }
    catch (exceptions::stream_corrupted& e)
    {
        printf("websocket_server> exception occured at _handshake() %s\n", e.what());
        return false;
    }
    catch (exceptions::stream_io_failed& e)
    {
        printf("websocket_server> exception occured at _handshake() %s\n", e.what());
        return false;
    }
    return true;
}

class xwebsocket_stream: public websocket_stream
{
    DISALLOW_COPY_AND_ASSIGN(xwebsocket_stream);
public:
    using unique_ptr = ::std::unique_ptr<xwebsocket_stream>;
    using shared_ptr = ::std::shared_ptr<xwebsocket_stream>;
    using tcp_stream = tcp::tcp_stream;
    using socket_t = ::boost::asio::ip::tcp::socket;
    using endpoint = ::boost::asio::ip::tcp::endpoint;
    using io_service_t = ::boost::asio::io_service;
    xwebsocket_stream(bool is_client);
    explicit xwebsocket_stream(tcp_stream&& stream, bool is_client);
    virtual ~xwebsocket_stream();
    xwebsocket_stream& operator=(xwebsocket_stream&& rhs);
    virtual void connect(const char* ip_addr, port_t port) throw(...) override;
    virtual void async_connect(const char* ip_addr, port_t port, std::function<VEE_NET_ASYNC_CONNECT_CALLBACK_SIGNATURE> e) override;
    virtual void disconnect() override;
    //! Read and Write operations require additional space to accommodate the Websocket data frame header in the data buffer.
    virtual websocket_io_result write_some(opcode_id opcode, const unsigned char* data, const size_t len) throw(...) override;
    virtual void          async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_callback e) throw(...) override;
    virtual websocket_io_result read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity) throw(...) override;
    virtual websocket_io_result read_some_all(unsigned char* const buffer, const size_t buf_capacity) throw(...) override;
    virtual void      async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) override;
    virtual void      async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) override;
    virtual int       socket_id() __noexcept;
    inline io_service_t& get_io_service() const
    {
        return *_host_io_service_ptr;
    }
    void conversion(tcp_stream&& stream);

protected:
    std::pair<websocket_io_result /*header and payload size*/, std::vector<unsigned char> /*data*/> _convert_to_websocket_form(opcode_id opcode, const unsigned char* data, const size_t len);
    std::pair<websocket_io_result /*header and payload size*/, dataframe::header /*header*/> _preprocess_received_data(unsigned char* const data, const size_t len) throw(...);
protected:
    io_service_t* _host_io_service_ptr;
    tcp_stream _tcp_stream;
    bool _is_client;
};

xwebsocket_stream::xwebsocket_stream(bool is_client):
_tcp_stream(io_service_sigleton::get().io_service()),
_host_io_service_ptr(&(_tcp_stream.get_io_service())),
_is_client(is_client)
{

}

xwebsocket_stream::xwebsocket_stream(tcp_stream&& stream, bool is_client):
_tcp_stream(static_cast<tcp_stream&&>(stream)),
_host_io_service_ptr(&(_tcp_stream.get_io_service())),
_is_client(is_client)
{

}

xwebsocket_stream::~xwebsocket_stream()
{

}

xwebsocket_stream& xwebsocket_stream::operator=(xwebsocket_stream&& rhs)
{
    _tcp_stream = static_cast<tcp_stream&&>(rhs._tcp_stream);
    _host_io_service_ptr = &(_tcp_stream.get_io_service());
    return *this;
}

void xwebsocket_stream::connect(const char* ip_addr, port_t port) throw(...)
{
#pragma warning(disable:4996)
    handshake::client_request request;
    request.request_uri = "GET /chat HTTP/1.1";
    request.host = "127.0.0.1:";
    {
        ::std::array<char, 128> buffer;
        sprintf(buffer.data(), "%d", port);
        request.host.append(buffer.data());
    }
    request.upgrade = "websocket";
    request.connection = "Upgrade";
    request.sec_websocket_key = "dGhlIHNhbXBsZSBub25jZQ=="; //TODO: Randomize
    request.origin = "http://127.0.0.1";
    {
        ::std::array<char, 128> buffer;
        sprintf(buffer.data(), "%d", port);
        request.origin.append(buffer.data());
    }
    request.sec_websocket_protocol = "char, superchat";
    request.sec_web_socket_version = "13";

    handshake::server_response response;
    ::std::array<char, 512> response_buffer;
    {
        ::std::string pack = request.binary_pack();
        _tcp_stream.connect(ip_addr, port);
        _tcp_stream.write_some((unsigned char*)pack.data(), pack.size());
        _tcp_stream.read_some((unsigned char*)response_buffer.data(), response_buffer.size());
    }
    response.parse(response_buffer.data());
    //Temporary validate check process
    if (response.sec_websocket_accept.compare("s3pPLMBiTxaQ9kYGzzhZRbK+xOo=") != 0)
    { // invalid response
        throw exceptions::rfc6455_handshake_failure();
    }
#pragma warning(default:4996)
}

void xwebsocket_stream::async_connect(const char* ip_addr, port_t port, async_connect_callback e)
{
    _tcp_stream.async_connect(ip_addr, port, e);
}

void xwebsocket_stream::disconnect()
{
    std::array<unsigned char, 128> buffer;
    buffer.fill(0);
    write_some(opcode_id::connnection_close, buffer.data(), buffer.size());
    _tcp_stream.disconnect();
}

websocket_stream::websocket_io_result xwebsocket_stream::write_some(opcode_id opcode, const unsigned char* data, const size_t len) throw(...)
{
    auto ws_packet = _convert_to_websocket_form(opcode, data, len);
    _tcp_stream.write_some(ws_packet.second.data(), ws_packet.second.size());
    return ws_packet.first; // return io_result
}

void xwebsocket_stream::async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_callback e) throw(...)
{
    auto packet = _convert_to_websocket_form(opcode, data, len);
    _tcp_stream.async_write_some(packet.second.data(), packet.first.header_size + len, std::move(e));
}

websocket_stream::websocket_io_result xwebsocket_stream::read_some_all(unsigned char* const buffer, const size_t buf_capacity) throw(...)
{
    while (true)
    {
        try
        {
            size_t bytes_transferred = _tcp_stream.read_some(buffer, buf_capacity);
            auto info = _preprocess_received_data(buffer, bytes_transferred);
            return info.first;
        }
        catch (exceptions::rfc6455_heartbeat&)
        {
            continue;
        }
        catch (exceptions::rfc6455_connection_close&)
        {
            throw exceptions::stream_read_failed();
        }
        catch (...)
        {
            throw;
        }
    }
}

websocket_stream::websocket_io_result xwebsocket_stream::read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity) throw(...)
{
    websocket_io_result result = read_some_all(buffer, buf_capacity);
    memmove(buffer, buffer + result.header_size, (uint32_t)(result.payload_size));
    memset(buffer + result.payload_size, 0, (uint32_t)(buf_capacity - result.payload_size));
    return result;
}

void xwebsocket_stream::async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...)
{
    async_read_callback hooker = [this, e](::vee::io::io_result& io_result, unsigned char* const buffer, const size_t buf_capacity) -> void
    {
        if (!io_result.is_success || io_result.eof)
        {
            e(io_result, buffer, buf_capacity);
            return;
        }
        try
        {
            auto info = _preprocess_received_data(buffer, io_result.bytes_transferred);
            memmove(buffer, buffer + info.first.header_size, (uint32_t)(info.first.payload_size));
            memset(buffer + info.first.payload_size, 0, (uint32_t)(buf_capacity - info.first.payload_size));
            io_result.bytes_transferred = info.first.payload_size;
            e(io_result, buffer, buf_capacity);
        }
        catch (exceptions::rfc6455_heartbeat&)
        {
            async_read_some_payload_only(buffer, buf_capacity, e);
            return;
        }
    };
    _tcp_stream.async_read_some(buffer, buf_capacity, hooker);
}

void xwebsocket_stream::async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...)
{
    async_read_callback hooker = [this, e](::vee::io::io_result& io_result, unsigned char* const buffer, const size_t buf_capacity) -> void
    {
        if (!io_result.is_success || io_result.eof)
        {
            e(io_result, buffer, buf_capacity);
            return;
        }
        try
        {
            auto info = _preprocess_received_data(buffer, io_result.bytes_transferred);
            e(io_result, buffer, buf_capacity);
        }
        catch (exceptions::rfc6455_heartbeat&)
        {
            async_read_some_all(buffer, buf_capacity, e);
            return;
        }
    };
    _tcp_stream.async_read_some(buffer, buf_capacity, hooker);
}

void xwebsocket_stream::conversion(tcp_stream&& stream)
{
    _tcp_stream = static_cast<tcp_stream&&>(stream);
    _host_io_service_ptr = &(_tcp_stream.get_io_service());
}

int xwebsocket_stream::socket_id() __noexcept
{
    return _tcp_stream.socket_id();
}

::std::shared_ptr<net_server> create_server(unsigned short port)
{
    ::std::shared_ptr<net_server> server = ::std::make_shared<xwebsocket_server>(port);
    return server;
}

std::pair<websocket_stream::websocket_io_result /*header and payload size*/, ::std::vector<unsigned char> /*data*/> xwebsocket_stream::_convert_to_websocket_form(opcode_id opcode, const unsigned char* data, const size_t len)
{
    dataframe::header header;
    header.fin = true; //TODO: 쪼개서 보내는 함수 지원하기, 지금은 무조건 한번에 다! 보낸다! 스펙상 INT64_MAX만큼 한번에 보낼 수 있지만 브라우저가 뻗을 듯
    header.use_mask = _is_client; //TODO: 커스텀 마스크 사용하는 함수 지원하기, 지금은 고정된 마스크 -> 클라이언트에만 필요, 서버는 절대 마스킹하면 안됨!
    header.payload_len = len;
    uint32_t header_size = header.binary_pack_size();
    header.randomize_mask();
    std::vector<unsigned char> payload((uint32_t)(len + header_size), 0); //TODO: 매번 벡터를 안만드는 방법을 생각해보자.
    memmove(payload.data() + header.binary_pack(opcode, payload.data()), data, (uint32_t)len);
    if (header.use_mask)
    {
        for (unsigned int i = 0; i < len; ++i)
        {
            unsigned char mask = header.masking_key[i % 4];
            unsigned char& transforemd_octet_i = payload[i + header_size];
            transforemd_octet_i ^= mask;
        }
    }

    std::pair<websocket_stream::websocket_io_result /*header and payload size*/, std::vector<unsigned char> /*data*/> ret;
    ret.first.header_size = header_size;
    ret.first.payload_size = (uint32_t)header.payload_len;
    ret.second = std::move(payload);
    return ret;
}

std::pair<websocket_stream::websocket_io_result /*header and payload size*/, dataframe::header /*header*/> xwebsocket_stream::_preprocess_received_data(unsigned char* const data, const size_t len) throw(...)
{
    //ISSUE: FIN 패킷이 아닐 때 인터페이스에 맞춰주는 코드.... 필요할까?
    std::pair<websocket_stream::websocket_io_result /*header and payload size*/, dataframe::header /*header*/> ret;
    size_t bytes_transferred = len;
    websocket_io_result& io_result = ret.first;
    dataframe::header& header = ret.second;

    header.analyze(data, bytes_transferred);
    switch (header.opcode)
    {
    case opcode_id::ping:
    case opcode_id::pong:
        throw exceptions::rfc6455_heartbeat();
    default:
        break;
    }

    // Check the connection close operation
    if (header.opcode == opcode_id::connnection_close)
    {
        throw exceptions::rfc6455_connection_close();
    }

    // Copy binary data from buffer (DO NOT USE EXTEISNION FLAG!)
    // Unmask
    {
        for (unsigned int i = 0; i < header.payload_len; ++i)
        {
            unsigned char mask = header.masking_key[i % 4];
            data[i + header.payload_pos] ^= mask;
        }
    }

    ret.first.header_size = bytes_transferred - (uint32_t)header.payload_len;
    ret.first.payload_size = (uint32_t)header.payload_len;

    return ret;
}

} // !namespace impl

net_server::shared_ptr create_server(port_t port)
{
    ::std::shared_ptr<net_server> server = ::std::static_pointer_cast<net_server>(::std::make_shared<impl::xwebsocket_server>(port));
    return server;
}

websocket_stream::shared_ptr create_stream()
{
    //TODO: IO_SERVICE SELECTION
    websocket_stream::shared_ptr stream = ::std::static_pointer_cast<websocket_stream>(::std::make_shared<impl::xwebsocket_stream>(true));
    return stream;
}

} // !namespace rfc6455

} // !namespace net

} // !namespace vee