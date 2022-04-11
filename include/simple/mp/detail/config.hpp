#ifndef SIMPLE_MP_DETAIL_PUSH_CONFIG_HPP
#define SIMPLE_MP_DETAIL_PUSH_CONFIG_HPP

#if __cplusplus >= 201703L
#define SMP_CPP_VERSION 17
#elif __cplusplus >= 201402L
#define SMP_CPP_VERSION 14
#else
// TODO: not portable
#warning "simple::mp requires at least c++14"
#endif

#if defined __GNUC__
#define SMP_LIKELY__(EXPR) __builtin_expect(!!(EXPR), 1)
#else
#define SMP_LIKELY__(EXPR) (!!(EXPR))
#endif

#if defined NDEBUG
#define SMP_ASSERT(CHECK) void(0)
#else
#include <cassert>
#define SMP_ASSERT(CHECK) (SMP_LIKELY__(CHECK) ? void(0) : [] { assert(!#CHECK); }())
#endif

#define SMP_CPP(version) version == SMP_CPP_VERSION

#if SMP_CPP(14)
#define SMP_INLINE_17
#else
#define SMP_INLINE_17 inline
#endif

#define SMP_INLINE(cpp_version) SMP_INLINE_##cpp_version

#endif  // SIMPLE_MP_DETAIL_CONFIG_HPP
