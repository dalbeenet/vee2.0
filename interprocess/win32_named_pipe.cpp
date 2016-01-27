#include <vee/interprocess/pipe.h>
#include <vee/io/boost_io_service.h>
#include <vee/interprocess/exception.h>

namespace vee {

namespace interprocess {

namespace windows {

#ifdef VEE_PLATFORM_WINDOWS

class win32_named_pipe: public named_pipe
{
    DISALLOW_COPY_AND_ASSIGN(win32_named_pipe);
public:
    using asio_stream_handle = ::boost::asio::windows::stream_handle;
    virtual ~win32_named_pipe() __noexcept;
    win32_named_pipe() __noexcept;
    win32_named_pipe(HANDLE handle, const char* pipe_name, bool is_server_side);
    win32_named_pipe(asio_stream_handle&& handle, const char* pipe_name, bool is_server_side) __noexcept;
    win32_named_pipe(win32_named_pipe&& other) __noexcept;
    win32_named_pipe& operator=(win32_named_pipe&& other) __noexcept;
    virtual void connect(const char* pipe_name,
                         const create_option creation_opt,
                         const data_transfer_mode read_mode,
                         const size_t time_out_millisec) throw(...) override;
    virtual void disconnect() __noexcept override;
    virtual size_t write_some(const unsigned char* data, const size_t size) throw(...) override;
    virtual void async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...) override;
    virtual size_t read_some(unsigned char* const buffer, const size_t buf_capacity) throw(...) override;
    virtual void async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...) override;
    inline ::std::string& pipe_name()
    {
        return _pipe_name;
    }
protected:
    ::std::string _pipe_name;
    asio_stream_handle _stream_handler;
    bool   _is_server_side;
};

class win32_named_pipe_acceptor
{
    DISALLOW_COPY_AND_ASSIGN(win32_named_pipe_acceptor);
public:
    ~win32_named_pipe_acceptor() = default;
    win32_named_pipe_acceptor() = default;
    //win32_named_pipe_acceptor(win32_named_pipe_acceptor&& other) = default;
    //win32_named_pipe_acceptor& operator=(win32_named_pipe_acceptor&& other) = default;
    win32_named_pipe accept(const char* pipe_name,
                            const named_pipe::data_transfer_mode mode,
                            const size_t in_buffer_size,
                            const size_t out_buffer_size,
                            const unsigned long timeout = 0) throw(...);
};

class win32_named_pipe_server: public named_pipe_server
{
    DISALLOW_COPY_AND_ASSIGN(win32_named_pipe_server);
public:
    using session_t = win32_named_pipe;
    using session_ptr = ::std::shared_ptr<session_t>;
    using generic_session_ptr = ::std::shared_ptr<named_pipe>;
    virtual ~win32_named_pipe_server() = default;
    win32_named_pipe_server() = default;
    //win32_named_pipe_server(win32_named_pipe_server&&) = default;
    //win32_named_pipe_server& operator=(win32_named_pipe_server&&) = default;
    virtual generic_session_ptr     accept(const char* pipe_name,
                                           const named_pipe::data_transfer_mode mode,
                                           const size_t in_buffer_size,
                                           const size_t out_buffer_size,
                                           const unsigned long timeout = 0) throw(...) override;
    virtual void close() __noexcept override;
protected:
    win32_named_pipe_acceptor _acceptor;
};

win32_named_pipe::~win32_named_pipe() __noexcept
{
    disconnect();
}

win32_named_pipe::win32_named_pipe() __noexcept:
_stream_handler(io_service_sigleton::get().io_service())
{

}

//TODO: IO_SERVICE Selection
win32_named_pipe::win32_named_pipe(HANDLE handle, const char* pipe_name, bool is_server_side) __noexcept:
_pipe_name(pipe_name),
_is_server_side(is_server_side),
_stream_handler(io_service_sigleton::get().io_service(), std::move(handle))
{

}

//TODO: IO_SERVICE Selection
win32_named_pipe::win32_named_pipe(asio_stream_handle&& handle, const char* pipe_name, bool is_server_side) __noexcept:
_pipe_name(pipe_name),
_is_server_side(is_server_side),
_stream_handler(std::move(handle))
{

}

//TODO: IO_SERVICE Selection
win32_named_pipe::win32_named_pipe(win32_named_pipe&& other) __noexcept:
_pipe_name(std::move(other._pipe_name)),
_is_server_side(other._is_server_side),
_stream_handler(std::move(other._stream_handler))
{

}

win32_named_pipe& win32_named_pipe::operator=(win32_named_pipe&& other) __noexcept
{
    _pipe_name = std::move(other._pipe_name);
    _is_server_side = other._is_server_side;
    _stream_handler = std::move(other._stream_handler);
    return *this;
}

void win32_named_pipe::connect(const char* pipe_name, const create_option creation_opt, const data_transfer_mode read_mode, const size_t time_out_millisec) throw(...)
{
    if (_stream_handler.is_open() == true)
        throw exceptions::stream_already_opened();
    HANDLE pipe_handle = NULL;
    {
        DWORD win32_file_craetion_opt = NULL;
        switch (creation_opt)
        {
        case create_option::create_or_truncate:
            win32_file_craetion_opt = CREATE_ALWAYS;
            break;
        case create_option::create_new:
            win32_file_craetion_opt = CREATE_NEW;
            break;
        case create_option::open_or_crate:
            win32_file_craetion_opt = OPEN_ALWAYS;
            break;
        case create_option::open_existing:
            win32_file_craetion_opt = OPEN_EXISTING;
            break;
        case create_option::truncate_existing:
            win32_file_craetion_opt = TRUNCATE_EXISTING;
            break;
        default:
            throw ::std::runtime_error("Invalid parameter error!\nwin32_named_pipe::connect -> creation_opt");
        }

        pipe_handle = CreateFileA(
            pipe_name, // pipe name
            GENERIC_READ | GENERIC_WRITE | FILE_FLAG_OVERLAPPED, // read and write access
            0,              // no sharing
            NULL,           // default secuirty attributes
            win32_file_craetion_opt,  // creation option
            FILE_FLAG_OVERLAPPED,     // overlapped I/O
            NULL            // no template file
            );

        // get an error code
        DWORD error = GetLastError();

        // Check the piple handle validity
        if (pipe_handle == INVALID_HANDLE_VALUE && error != ERROR_PIPE_BUSY)
        {
            // Throw an exception if an error other than ERROR_PIPE_BUSY occurs.    
            throw exceptions::pipe_creation_failed(error);
        }
        else if (error == ERROR_PIPE_BUSY)
        {
            // If a pipe instance is busy, so wait for timeout limit.
            DWORD time_out_arg = 0;
            switch (time_out_millisec)
            {
            case pipe_time_out_constant::wait_default:
                time_out_arg = NMPWAIT_USE_DEFAULT_WAIT;
                break;
            case pipe_time_out_constant::wait_forever:
                time_out_arg = NMPWAIT_WAIT_FOREVER;
                break;
            case pipe_time_out_constant::wait_nowait:
                time_out_arg = NMPWAIT_NOWAIT;
                break;
            default:
                time_out_arg = static_cast<DWORD>(time_out_millisec);
                break;
            }
            if (!WaitNamedPipeA(pipe_name, time_out_arg))
            {
                // Throw an exception if timed out.
                throw exceptions::pipe_wait_timeout(GetLastError());
            }
        }
    }

    // The pipe connected, change to the read mode
    {
        DWORD win32_pipe_read_mode = NULL;
        switch (read_mode)
        {
        case data_transfer_mode::iomode_byte:
            win32_pipe_read_mode = PIPE_READMODE_BYTE;
            break;
        case data_transfer_mode::iomode_message:
            win32_pipe_read_mode = PIPE_READMODE_MESSAGE;
            break;
        default:
            throw ::std::runtime_error("Invalid parameter error!\nwin32_named_pipe::connect -> read_mode");
        }

        BOOL result = SetNamedPipeHandleState(
            pipe_handle,    // pipe handle 
            &win32_pipe_read_mode,  // new pipe mode 
            NULL,     // don't set maximum bytes 
            NULL);    // don't set maximum time 
        if (!result)
        {
            throw exceptions::pipe_initialize_failed(GetLastError());
        }
    }

    // pipe stream is ready
    _pipe_name = pipe_name;
    _is_server_side = false;
    _stream_handler.assign(pipe_handle);
}

void win32_named_pipe::disconnect() __noexcept
{
    if (_stream_handler.is_open() == false)
    return;
    if (_is_server_side)
    {
        FlushFileBuffers(_stream_handler.native_handle());
        DisconnectNamedPipe(_stream_handler.native_handle());
    }
    _is_server_side = false;
    try
    {
        _stream_handler.close();
    }
    catch (boost::exception&) // øπø‹ ªÔ≈¥
    {
        //std::cerr << boost::diagnostic_information(e);
    }
}

size_t win32_named_pipe::read_some(unsigned char* const buffer, const size_t buf_capacity) throw(...)
{
    if (_stream_handler.is_open() == false)
    {
        throw exceptions::stream_corrupted();
    }

    ::boost::system::error_code error;
    memset(buffer, 0, (size_t)buf_capacity);
    size_t bytes_transferred = (size_t)_stream_handler.read_some(::boost::asio::buffer(buffer, (size_t)buf_capacity), error);

    if (error)
    {
        switch (GetLastError())
        {
        case 109:
            throw exceptions::pipe_stream_reset(GetLastError());
        default:
            throw exceptions::pipe_read_failed(GetLastError());
        }
    }
    else if (GetLastError() == ERROR_MORE_DATA)
    {
        throw exceptions::pipe_buffer_too_small(GetLastError());
    }

    return bytes_transferred;
}

void win32_named_pipe::async_read_some(unsigned char* const buffer, const size_t buf_capacity, async_read_callback e) throw(...)
{
    auto handle_read = [buffer, buf_capacity, e](const boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::io_result result;
        /*if ((boost::asio::error::eof == error) ||
            (boost::asio::error::connection_reset == error))
            {
            result.is_success = false;
            result.eof = true;
            result.bytes_transferred = bytes_transferred;
            }*/
        if (bytes_transferred == 0) // Windows Named Pipe does not supports 0byte IO -> Could not catch disconnection by boost asio errors
        {
            result.is_success = false;
            result.eof = true;
            result.bytes_transferred = bytes_transferred;
        }
        else if (error)
        {
            result.is_success = false;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        else
        {
            result.is_success = true;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        e(result, buffer, buf_capacity);
    };
    /*if (_stream_handler.native_handle() == NULL)
    {
        throw vee::exception("invalid connection!", (int)error_code::closed_stream);
    }*/
    _stream_handler.async_read_some(::boost::asio::buffer(buffer, (size_t)buf_capacity), handle_read);
}

size_t win32_named_pipe::write_some(const unsigned char* data, const size_t size) throw(...)
{
    if (_stream_handler.is_open() == false)
    {
        throw exceptions::stream_corrupted();
    }

    ::boost::system::error_code error;
    size_t bytes_transferred = _stream_handler.write_some(::boost::asio::buffer(data, (size_t)size), error);
    if (error)
    {
        throw exceptions::pipe_write_failed(GetLastError());
    }

    return bytes_transferred;
}

void win32_named_pipe::async_write_some(const unsigned char* data, const size_t len, async_write_callback e) throw(...)
{
    auto handle_write = [e](const boost::system::error_code& error, size_t bytes_transferred) -> void
    {
        io::io_result result;
        if ((boost::asio::error::eof == error) ||
            (boost::asio::error::connection_reset == error))
        {
            result.is_success = false;
            result.eof = true;
            result.bytes_transferred = bytes_transferred;
        }
        else if (error)
        {
            result.is_success = false;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        else
        {
            result.is_success = true;
            //result.log = error.message();
            result.bytes_transferred = bytes_transferred;
        }
        e(result);
    };
    /*if (_stream_handler.native_handle() == NULL)
    {
        throw vee::exception("invalid connection!", (int)error_code::closed_stream);
    }*/
    _stream_handler.async_write_some(::boost::asio::buffer(data, (size_t)len), handle_write);
}

//win32_named_pipe_acceptor::~win32_named_pipe_acceptor() __noexcept
//{
//
//}
//
//win32_named_pipe_acceptor::win32_named_pipe_acceptor() __noexcept
//{
//
//}
//
//win32_named_pipe_acceptor::win32_named_pipe_acceptor(win32_named_pipe_acceptor&& other)
//{
//
//}
//
//win32_named_pipe_acceptor& win32_named_pipe_acceptor::operator=(win32_named_pipe_acceptor&& other)
//{
//    return *this;
//}

win32_named_pipe win32_named_pipe_acceptor::accept(const char* pipe_name,
                                                   const named_pipe::data_transfer_mode io_mode,
                                                   const size_t in_buffer_size,
                                                   const size_t out_buffer_size,
                                                   const unsigned long timeout) throw(...)
{
    DWORD win32_pipe_type_arg = NULL;
    switch (io_mode)
    {
    case named_pipe::data_transfer_mode::iomode_byte:
        win32_pipe_type_arg = PIPE_READMODE_BYTE | PIPE_TYPE_BYTE;
        break;
    case named_pipe::data_transfer_mode::iomode_message:
        win32_pipe_type_arg = PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE;
        break;
    default:
        throw ::std::runtime_error("Invalid parameter at win32_named_pipe_acceptor::accept param-2: io_mode");
    }
    HANDLE pipe_handle = CreateNamedPipeA(
        pipe_name,    // pipe name
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,  // read and wirte access
        win32_pipe_type_arg | PIPE_WAIT, // pipe mode | blocking_mode
        PIPE_UNLIMITED_INSTANCES,// max. instances
        static_cast<DWORD>(in_buffer_size),
        static_cast<DWORD>(out_buffer_size),
        timeout, // client time-out
        NULL // default security attribute
        );

    if (pipe_handle == INVALID_HANDLE_VALUE)
    {
        throw exceptions::pipe_creation_failed(GetLastError());
    }

    // Wait for the client to connect
    if (timeout == 0)
    {
        BOOL result = ConnectNamedPipe(pipe_handle, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (!result)
        {
            throw exceptions::pipe_accept_failed(GetLastError());
        }
    }
    else
    {
        OVERLAPPED ol = { 0, 0, 0, 0, NULL };
        ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        BOOL result = ConnectNamedPipe(pipe_handle, &ol);

        if (result == 0)
        {
            switch (GetLastError())
            {
            case ERROR_PIPE_CONNECTED:
                /* client is connected */
                result = TRUE;
                break;

            case ERROR_IO_PENDING:
                /* if pending i wait PIPE_TIMEOUT_CONNECT ms */
                if (WaitForSingleObject(ol.hEvent, timeout) == WAIT_OBJECT_0)
                {
                    DWORD dwIgnore;
                    result = GetOverlappedResult(pipe_handle, &ol, &dwIgnore, FALSE);
                }
                else
                {
                    CancelIo(pipe_handle);
                }
                break;
            }
        }
        CloseHandle(ol.hEvent);

        if (!result)
        {
            throw exceptions::pipe_accept_timeout();
        }
    }

    win32_named_pipe pipe(pipe_handle, pipe_name, true);
    return pipe;
}

//////////////////////////////////////////////////////////////////////////

//win32_named_pipe_server::~win32_named_pipe_server() __noexcept
//{
//
//}
//
//win32_named_pipe_server::win32_named_pipe_server() __noexcept
//{
//
//}
//
//win32_named_pipe_server::win32_named_pipe_server(win32_named_pipe_server&& other) __noexcept
//{
//
//}
//
//win32_named_pipe_server& win32_named_pipe_server::operator=(win32_named_pipe_server&& other) __noexcept
//{
//    return *this;
//}

win32_named_pipe_server::generic_session_ptr win32_named_pipe_server::accept(const char* pipe_name, const named_pipe::data_transfer_mode mode, const size_t in_buffer_size, const size_t out_buffer_size, const unsigned long timeout) throw(...)
{
    generic_session_ptr generic_session = std::make_shared<session_t>(_acceptor.accept(pipe_name, mode, in_buffer_size, out_buffer_size, timeout));
    return generic_session;
}

void win32_named_pipe_server::close() __noexcept
{

}

::std::shared_ptr<named_pipe_server> create_named_pipe_server() __noexcept
{
    ::std::shared_ptr<named_pipe_server> server = ::std::make_shared<win32_named_pipe_server>();
    return server;
}

::std::shared_ptr<named_pipe> create_named_pipe()
{
    ::std::shared_ptr<named_pipe> pipe = ::std::make_shared<win32_named_pipe>();
    return pipe;
}

#endif

} // !namespace windows

} // !namespace interprocess

} // !namespace vee