#include <vee/interprocess/exception.h>
#include <cstdio>

namespace vee {

namespace exceptions {

#pragma warning(disable:4996)
pipe_creation_failed::pipe_creation_failed(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe creation failed (gle: %d)", gle);
}

pipe_wait_timeout::pipe_wait_timeout(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe wait timeout (gle: %d)", gle);
}

pipe_initialize_failed::pipe_initialize_failed(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe intialize failed (gle: %d)", gle);
}

pipe_stream_reset::pipe_stream_reset(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe stream reset (gle: %d)", gle);
}

pipe_read_failed::pipe_read_failed(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe read failed (gle: %d)", gle);
}

pipe_buffer_too_small::pipe_buffer_too_small(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe read failed (gle: %d)", gle);
}


pipe_write_failed::pipe_write_failed(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe write failed (gle: %d)", gle);
}

pipe_accept_failed::pipe_accept_failed(int _gle):
gle(_gle)
{
    sprintf(_desc, "pipe write failed (gle: %d)", gle);
}

} // !namespace exceptions

} // !namespace vee