#ifndef INCLUDED_MAX_FLOW_NON_VOID
#define INCLUDED_MAX_FLOW_NON_VOID

#include <type_traits>
#include <concepts>

#define MAX_FLOW_NON_VOID_T_SA_DECL(TypeWithoutTPrefix) template <::std::same_as<T##TypeWithoutTPrefix> TNonVoid##TypeWithoutTPrefix = T##TypeWithoutTPrefix> requires (!::std::is_void_v<T##TypeWithoutTPrefix>)
#define MAX_FLOW_NON_VOID_T_SA_IMPL(TypeWithoutTPrefix) template <::std::same_as<T##TypeWithoutTPrefix> TNonVoid##TypeWithoutTPrefix> requires (!::std::is_void_v<T##TypeWithoutTPrefix>)
#define MAX_FLOW_NON_VOID_T(TypeWithoutTPrefix) template <typename TNonVoid##TypeWithoutTPrefix> requires (!::std::is_void_v<TNonVoid##TypeWithoutTPrefix>)

#endif