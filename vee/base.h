#ifndef _VEE_BASE_H_
#define _VEE_BASE_H_

#define _VEE    ::vee::
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&) = delete; \
void operator=(const TypeName&) = delete;
#define DISALLOW_MOVE_AND_ASSIGN(TypeName) \
TypeName(const TypeName&&) = delete; \
void operator=(const TypeName&&) = delete;
#define VEE_EMPTY_FUNCTION {}
#ifndef _VEE_ENABLE_LOGGING
#define _VEE_ENABLE_LOGGING 1
#endif
#ifdef _VEE_ENABLE_LOGGING
#define DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define DEBUG_PRINT(...)
#endif
#ifdef _WIN32
#define VEE_PLATFORM_WINDOWS 1
#define VEE_PLATFORM_X32 1
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif // !_WIN32_WINNT
#elif  _WIN64
#define VEE_PLATFORM_WINDOWS_ 1
#define VEE_PLATFORM_X64 1
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif // !_WIN32_WINNT
#else
#define VEE_PLATFORM_WINDOWS 0
#endif
#define PRINT_LINE for(int i = 0; i < 7; ++i) printf("----------");printf("\n");

class static_class abstract
{
    DISALLOW_COPY_AND_ASSIGN(static_class);
    DISALLOW_MOVE_AND_ASSIGN(static_class);
};
#if (_MSC_VER == 1900) // VS2015
#define __noexcept noexcept
#else
#define __noexcept throw()
#endif

#endif // !_VEE_BASE_H_