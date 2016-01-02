#ifndef _VEE_TEST_TEST_OBJECT_H_
#define _VEE_TEST_TEST_OBJECT_H_

#include <cstdio>
#include <atomic>

namespace vee {

namespace test {

static std::atomic<int> cntInstances = 0;

class TEST_OBJECT
{
public:
    TEST_OBJECT(): id(cntInstances++)
    {
        printf("%s(%d)\n", __FUNCSIG__, id);
    }
    ~TEST_OBJECT()
    {
        printf("%s(%d)\n", __FUNCSIG__, id);
    }
    TEST_OBJECT(TEST_OBJECT&): id(cntInstances++)
    {
        printf("%s(%d)\n", __FUNCSIG__, id);
    }
    TEST_OBJECT(TEST_OBJECT&&): id(cntInstances++)
    {
        printf("%s(%d)\n", __FUNCSIG__, id);
    }
    TEST_OBJECT& operator=(TEST_OBJECT&)
    {
        printf("%s(%d)\n", __FUNCSIG__, id);
        return *this;
    }
    TEST_OBJECT& operator=(TEST_OBJECT&&)
    {
        printf("%s(%d)\n", __FUNCSIG__, id);
        return *this;
    }
    int id;
};

} // !namespace test

} // !namespace vee

#endif // !_VEE_TEST_TEST_OBJECT_H_
