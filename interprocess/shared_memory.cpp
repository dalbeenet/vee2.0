#include <vee/interprocess/shared_memory.h>
#if VEE_PLATFORM_WINDOWS
#include <boost/interprocess/windows_shared_memory.hpp>
#else
// Boost c++ shared memomy header file
#include <boost/interprocess/shared_memory_object.hpp>
#endif
#include <boost/interprocess/mapped_region.hpp>
#include <cstdlib>
#include <string>

namespace vee {

namespace interprocess {

namespace impl {

class xshared_memory: public shared_memory
{
public:
    static const ::boost::interprocess::create_only_t         create_only;
    static const ::boost::interprocess::open_only_t           open_only;
    static const ::boost::interprocess::open_read_only_t      open_read_only;
    static const ::boost::interprocess::open_or_create_t      open_or_create;
    static const ::boost::interprocess::open_copy_on_write_t  open_copy_on_write;
    typedef      ::boost::interprocess::mode_t access_mode;
    typedef      ::boost::interprocess::mapped_region mapped_region;
    typedef      ::boost::interprocess::offset_t offset_type;
#if VEE_PLATFORM_WINDOWS
    typedef      ::boost::interprocess::windows_shared_memory shared_memory_type;
#else
    typedef      ::boost::interprocess::shared_memory_object shared_memory_type;
#endif
    // ctor (template for optional)
    template <class CreateOpt>
    xshared_memory(const char* name, unsigned int size, CreateOpt create_opt, access_mode mode):
        shm_name(name),
        shm_size(size),
        shm(create_opt,
        name,
        mode
#if VEE_PLATFORM_WINDOWS
        , size),
#else
        ),
#endif
        master_region(nullptr)
    {
        master_region = ::std::make_shared<mapped_region>(shm, access_mode::read_write, 0, shm_size);
    }
    xshared_memory(xshared_memory&& other):
        shm_name(std::move(other.shm_name)),
        shm_size(std::move(other.shm_size)),
        shm(static_cast<shared_memory_type&&>(other.shm)),
        master_region(nullptr)
    {
        master_region = ::std::make_shared<mapped_region>(shm, access_mode::read_write, 0, shm_size);
    }
    virtual ~xshared_memory()
    {
#if VEE_PLATFORM_WINDOWS
        this->remove(shm_name.data());
#endif
    }
    inline const mapped_region get_region(offset_type offset, unsigned int size, access_mode mode) const
    {
        // Mapping Shared Memory Segments
        // Once created or opened, a process just has to map the shared memory object in the process's address space.
        // The user can map the whole shared memory or just part of it.
        // The mapping process is done using the boost::interprocess::mapped_region class.
        // The class represents a memory region that has been mapped from a shared memory or from other devices that have also mapping capabilities. (for example, files).
        // A mapped_region can be created from any memory_mappable object and as you might imagine, shared_memory_object is a memory_mappable object
        boost::interprocess::mapped_region region(
            shm,
            mode,
            offset,
            size
            );
        return region;
    }
    bool remove()
    {
        remove(shm_name.c_str());
    }
#if VEE_PLATFORM_WINDOWS
#pragma warning(disable:4100)
    static bool remove(const char* name)
    {
        return false;
    }
#pragma warning(default:4100)
#else
    static bool remove(const char* name)
    {
        return boost::interprocess::shared_memory_object::remove(name);
    }
#endif

    virtual void*        get_address() const override;
    virtual unsigned int get_size() const override;
    virtual const char*  get_name() const override;
public:
    ::std::string shm_name;
    ::std::shared_ptr<mapped_region> master_region;
    unsigned int shm_size;
    shared_memory_type shm;
};

const ::boost::interprocess::create_only_t         xshared_memory::create_only = ::boost::interprocess::create_only_t();
const ::boost::interprocess::open_only_t           xshared_memory::open_only = ::boost::interprocess::open_only_t();
const ::boost::interprocess::open_read_only_t      xshared_memory::open_read_only = ::boost::interprocess::open_read_only_t();
const ::boost::interprocess::open_or_create_t      xshared_memory::open_or_create = ::boost::interprocess::open_or_create_t();
const ::boost::interprocess::open_copy_on_write_t  xshared_memory::open_copy_on_write = ::boost::interprocess::open_copy_on_write_t();

void* xshared_memory::get_address() const
{
    return master_region->get_address();
}

unsigned int xshared_memory::get_size() const
{
    return shm_size;
}

const char* xshared_memory::get_name() const
{
    return shm_name.data();
}

} // !namespace impl

::std::shared_ptr<shared_memory> create_shared_memory(const char* shm_name, unsigned int size, shared_memory::create_option create_opt /* = shared_memory::create_option::open_or_create */)
{
    ::std::shared_ptr<shared_memory> shm = nullptr;
    switch (create_opt)
    {
    case vee::interprocess::shared_memory::create_option::create_only:
        shm = ::std::make_shared<impl::xshared_memory>(shm_name, size, impl::xshared_memory::create_only, impl::xshared_memory::access_mode::read_write);
        break;
        /*case vee::interprocess::shared_memory::create_option::open_only:
        shm = ::std::make_shared<xshared_memory>(shm_name, size, xshared_memory::open_only, xshared_memory::access_mode::read_write);
        break;*/
        /*case vee::interprocess::shared_memory::create_option::open_read_only:
        shm = ::std::make_shared<xshared_memory>(shm_name, size, xshared_memory::open_read_only, xshared_memory::access_mode::read_write);
        break;*/
    case vee::interprocess::shared_memory::create_option::open_or_create:
        shm = ::std::make_shared<impl::xshared_memory>(shm_name, size, impl::xshared_memory::open_or_create, impl::xshared_memory::access_mode::read_write);
        break;
        /*case vee::interprocess::shared_memory::create_option::open_copy_on_write:
        shm = ::std::make_shared<xshared_memory>(shm_name, size, xshared_memory::open_copy_on_write, xshared_memory::access_mode::read_write);
        break;*/
    default:
        break;
    }
    return shm;
}

} // !namespace interprocess

} // !namespace vee