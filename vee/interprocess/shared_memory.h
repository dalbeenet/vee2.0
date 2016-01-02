#ifndef _VEE_INTERPROCESS_SHARED_MEMORY_H_
#define _VEE_INTERPROCESS_SHARED_MEMORY_H_

#include <vee/base.h>
#include <memory>

namespace vee {

namespace interprocess {

class shared_memory abstract
{
public:
    using unique_ptr = ::std::unique_ptr<shared_memory>;
    using shared_ptr = ::std::shared_ptr<shared_memory>;
    enum class create_option: int
    {
        create_only = 0,
        //open_only,
        //open_read_only,
        open_or_create,
        //open_copy_on_write
    };
    enum class authority: int
    {
        //TODO: Implementation    
    };
    virtual ~shared_memory() = default;
    virtual void*         get_address() const __noexcept = 0;
    virtual unsigned int  get_size() const __noexcept = 0;
    virtual const char*   get_name() const __noexcept = 0;
};

shared_memory::shared_ptr create_shared_memory(const char* shm_name, unsigned int size, shared_memory::create_option create_opt = shared_memory::create_option::open_or_create);

} // !namespace interprocess

} // !namespace vee

#endif // !_VEE_INTERPROCESS_SHARED_MEMORY_H_
