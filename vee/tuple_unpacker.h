#ifndef _VEE_TUPLE_UNPACKER_H_
#define _VEE_TUPLE_UNPACKER_H_

#include <utility>
#include <tuple>
#include <vee/base.h>
#include <vee/type_parser.h>

namespace vee {

namespace xtuple_unpacker {
template<int ...>
struct sequence
{
};

template<int N, int ...S>
struct sequence_generator: sequence_generator < N - 1, N - 1, S... >
{
};

template<int ...S>
struct sequence_generator < 0, S... >
{
    typedef sequence<S...> type;
};

template <class RTy, class CallableObj, class Tuple, int ...S>
inline RTy _do_call(CallableObj&& func, Tuple&& tuple, sequence<S...>)
{
    return func(std::forward<decltype(std::get<S>(tuple))>(std::get<S>(tuple)) ...);
}

template <class CallableObj, class Tuple, int ...S>
inline void _do_call_noreturn(CallableObj&& func, Tuple&& tuple, sequence<S...>)
{
    func(std::forward<decltype(std::get<S>(tuple))>(std::get<S>(tuple)) ...);
}
}

template <
class CallableObj,
class Tuple,
class RTy = function_type_parser< std::remove_reference<CallableObj>::type >::return_type >
    RTy call_by_tuple(CallableObj&& func, Tuple&& tuple)
{
    return xtuple_unpacker::_do_call<RTy>(std::forward<CallableObj>(func),
                                            std::forward<Tuple>(tuple),
                                            typename xtuple_unpacker::sequence_generator< std::tuple_size< std::remove_reference<Tuple>::type >::value/*sizeof...(Args)*/>::type());
}

template < class CallableObj, class Tuple >
void call_by_tuple_noreturn(CallableObj&& func, Tuple&& tuple)
{
    xtuple_unpacker::_do_call_noreturn(std::forward<CallableObj>(func),
                                        std::forward<Tuple>(tuple),
                                        typename xtuple_unpacker::sequence_generator< std::tuple_size< std::remove_reference<Tuple>::type >::value/*sizeof...(Args)*/>::type());
}

} // !namespace vee

#endif // !_VEE_TUPLE_UNPACKER_H_