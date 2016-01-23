#ifndef _VEE_INTERPROCESS_EXCEPTION_H_
#define _VEE_INTERPROCESS_EXCEPTION_H_

#include <vee/io/exception.h>

namespace vee {

namespace exceptions {

class pipe_creation_failed: public stream_open_failed
{
    char _desc[256];
public:
    explicit pipe_creation_failed(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_wait_timeout: public stream_open_failed
{
    char _desc[256];
public:
    explicit pipe_wait_timeout(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_initialize_failed: public stream_open_failed
{
    char _desc[256];
public:
    explicit pipe_initialize_failed(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_stream_reset: public stream_reset
{
    char _desc[256];
public:
    explicit pipe_stream_reset(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_read_failed: public stream_read_failed
{
    char _desc[256];
public:
    explicit pipe_read_failed(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_buffer_too_small: public stream_read_failed
{
    char _desc[256];
public:
    explicit pipe_buffer_too_small(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_write_failed: public stream_write_failed
{
    char _desc[256];
public:
    explicit pipe_write_failed(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

class pipe_accept_failed: public stream_accept_failed
{
    char _desc[256];
public:
    explicit pipe_accept_failed(int _gle);
    virtual const char* to_string()
    {
        return this->what();
    }
    inline const char* what()
    {
        return _desc;
    }
    int  gle;
};

} // !namespace exceptions

} // !namespace vee

#endif // !_VEE_INTERPROCESS_EXCEPTION_H_