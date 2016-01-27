#ifndef _VEE_NETWORK_NET_H_
#define _VEE_NETWORK_NET_H_

#include <vee/base.h>
#include <vee/io/stream.h>
#include <vee/network/exception.h>
#include <array>

namespace vee {

namespace net {

#define IPV4_LOCAL_ADDR "127.0.0.1"

struct op_result
{
    bool is_success = false;
    std::string log;
};

using port_t = unsigned short;

#define VEE_NET_ASYNC_CONNECT_CALLBACK_SIGNATURE void(::vee::net::op_result&/*operation result*/)
#define VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE  void(::vee::net::op_result&/*operation result*/, ::vee::net::net_stream::shared_ptr/*stream*/)

class net_stream abstract: public io::stream
{
public:
    using async_connect_delegate = delegate<VEE_NET_ASYNC_CONNECT_CALLBACK_SIGNATURE>;
    using async_connect_callback = ::std::function<VEE_NET_ASYNC_CONNECT_CALLBACK_SIGNATURE>;
    using unique_ptr = ::std::unique_ptr<net_stream>;
    using shared_ptr = ::std::shared_ptr<net_stream>;
#if VEE_PLATFORM_X32
    using socketfd_t = uint32_t;
#elif VEE_PLATFORM_X64
    using socketfd_t = uint64_t;
#endif

    virtual ~net_stream() = default;
    virtual void    connect(const char* ipv4, port_t port) throw(...) = 0;
    virtual void    disconnect() throw(...) = 0;
    virtual void    async_connect(const char* ipv4, port_t port, async_connect_delegate& e);
    virtual void    async_connect(const char* ipv4, port_t port, async_connect_delegate&& e);
    virtual void    async_connect(const char* ipv4, port_t port, async_connect_delegate::shared_ptr& sptr_e);
    virtual void    async_connect(const char* ipv4, port_t port, async_connect_delegate::shared_ptr&& sptr_e);
    virtual void    async_connect(const char* ipv4, port_t port, async_connect_callback callback) = 0;
    virtual socketfd_t  socket_id() __noexcept = 0;
};

class net_server abstract
{
public:
    using async_accept_delegate_t = delegate<VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE>;
    using unique_ptr = ::std::unique_ptr<net_server>;
    using shared_ptr = ::std::shared_ptr<net_server>;
    virtual ~net_server() = default;
    virtual net_stream::shared_ptr  accept() throw(...) = 0;
    virtual void async_accept(async_accept_delegate_t e);
    virtual void async_accept(std::shared_ptr<async_accept_delegate_t> e);
    virtual void async_accept(std::function<VEE_NET_ASYNC_ACCEPT_CALLBACK_SIGNATURE> e) = 0;
    virtual void close() = 0;
};

namespace tcp {

net_server::shared_ptr  create_server(port_t port);
net_stream::shared_ptr  create_stream();

} // !namespace tcp

//TODO: Implementation UDP
namespace udp {

//net_stream::shared_ptr  create_stream();

} // !namespace udp

//TODO: Implementation
namespace rfc6455 {

#define RFC4122_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

enum class opcode_id: unsigned int
{
    undefined = 0,
    continuation_frame,
    text_frame,
    binary_frame,
    connnection_close,
    ping,
    pong,
    reserved_for_further
};

namespace handshake {

struct client_request
{
    void print() const;
    void parse(const char* data);
    void parse(std::string& data);
    bool is_valid() const;
    void clear();
    ::std::string binary_pack() const;
    ::std::string request_uri;
    ::std::string host;
    ::std::string upgrade;
    ::std::string connection;
    ::std::string origin;
    ::std::string sec_websocket_key;
    ::std::string sec_websocket_protocol;
    ::std::string sec_web_socket_version;
};

struct server_response
{
    void parse(const char* data);
    void parse(::std::string& data);
    //TODO: bool is_valid() const; // 현재는 RFC6455의 예시 응답으로 비교
    void print() const;
    void clear();
    ::std::string binary_pack() const;
    ::std::string http_status;
    ::std::string upgrade;
    ::std::string connection;
    ::std::string sec_websocket_accept;
};

} // !namespace handshake

namespace dataframe {

struct header
{
    // extensions
    bool     fin = true;
    bool     use_mask = false;
    uint64_t payload_len = 0;
    uint32_t payload_pos = 0;
    //x bool     use_extended16_payload = false;
    //x bool     use_extended64_payload = false;

    // rec6455 header data sections
    int8_t  fin_opcode = 0;
    int8_t  mask_payload_len = 0;
    int16_t extended_payload_len16 = 0;
    int64_t extended_payload_len64 = 0;
    opcode_id opcode = opcode_id::undefined;
    std::array<unsigned char, 4> masking_key;
    //std::vector<unsigned char> payload;
    bool analyze(const unsigned char* raw_data, const size_t length);
    uint32_t binary_pack(opcode_id opcode, unsigned char* out_buffer) const;
    uint32_t binary_pack_size() const;
    void randomize_mask();
};

} // namespace dataframe

class websocket_stream abstract: public net_stream
{
public:
    using unique_ptr = ::std::unique_ptr<websocket_stream>;
    using shared_ptr = ::std::shared_ptr<websocket_stream>;
    struct websocket_io_result
    {
        size_t header_size = 0;
        size_t payload_size = 0;
    };
    virtual ~websocket_stream() = default;
    virtual void        connect(const char* ip_addr, port_t port) throw(...) = 0;
    virtual void        async_connect(const char* ip_addr, port_t port, async_connect_callback e) = 0;
    virtual void        disconnect() = 0;
    virtual size_t      write_some(const unsigned char* data, const size_t len) throw(...) override;
    virtual websocket_io_result   write_some(opcode_id opcode, const unsigned char* data, const size_t len) throw(...) = 0;
    virtual void        async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...) override;
    virtual void        async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_delegate& e) throw(...);
    virtual void        async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_delegate&& e) throw(...);
    virtual void        async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_delegate::shared_ptr& e_ptr) throw(...);
    virtual void        async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_delegate::shared_ptr&& e_ptr) throw(...);
    virtual void        async_write_some(opcode_id opcode, const unsigned char* data, const size_t len, async_write_callback e) throw(...) = 0;
    virtual size_t    read_some(unsigned char* const buffer, const size_t buf_capacity) throw(...) override;
    virtual websocket_io_result   read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity) throw(...) = 0;
    virtual websocket_io_result   read_some_all(unsigned char* const buffer, const size_t buf_capacity) throw(...) = 0;
    virtual void        async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) override;
    virtual void        async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate& e) throw(...);
    virtual void        async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate&& e) throw(...);
    virtual void        async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr& e_ptr) throw(...);
    virtual void        async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr&& e_ptr) throw(...);
    virtual void        async_read_some_payload_only(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) = 0;
    virtual void        async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate& e) throw(...);
    virtual void        async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate&& e) throw(...);
    virtual void        async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr& e_ptr) throw(...);
    virtual void        async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_delegate::shared_ptr&& e_ptr) throw(...);
    virtual void        async_read_some_all(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) = 0;
};

net_server::shared_ptr          create_server(port_t port);
websocket_stream::shared_ptr    create_stream();

} // !namespace rfc6455

} // !namespace net

} // !namespace vee

#endif // !_VEE_NETWORK_NET_H_
