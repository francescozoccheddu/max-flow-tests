#ifndef INCLUDED_MAX_FLOW_UTILS_MACROS_NON_VOID
#define INCLUDED_MAX_FLOW_UTILS_MACROS_NON_VOID

#include <type_traits>
#include <concepts>

#define MF_U_NV_SA_D(TypeWithoutTPrefix) template <::std::same_as<T##TypeWithoutTPrefix> TNonVoid##TypeWithoutTPrefix = T##TypeWithoutTPrefix> requires (!::std::is_void_v<T##TypeWithoutTPrefix>)
#define MF_U_NV_SA_I(TypeWithoutTPrefix) template <::std::same_as<T##TypeWithoutTPrefix> TNonVoid##TypeWithoutTPrefix> requires (!::std::is_void_v<T##TypeWithoutTPrefix>)
#define MF_U_NV_T(TypeWithoutTPrefix) template <typename TNonVoid##TypeWithoutTPrefix> requires (!::std::is_void_v<TNonVoid##TypeWithoutTPrefix>)

#endif