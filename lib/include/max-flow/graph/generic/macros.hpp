#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_MACROS
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_MACROS

#define MF_GG_M_TT template<typename TVD, typename TED>

#define MF_GG_M_A(AliasWithoutTPrefix) MF_GG_M_C <TVD,TED>::T##AliasWithoutTPrefix

#define MF_GG_M_TS(ReturnType) inline ReturnType MF_GG_M_C <TVD,TED>::

#define MF_GG_M_FS(ReturnType) MF_GG_M_TT inline ReturnType 

#define MF_GG_M_S(ReturnType) MF_GG_M_FS(ReturnType) MF_GG_M_C <TVD,TED>::

#define MF_GG_M_B(ReturnType, Method, ...) { return static_cast< ReturnType >(Base :: MF_GG_M_C :: Method ( __VA_ARGS__ )); }

#define MF_GG_M_P0(ReturnType, Method, Modifier) MF_GG_M_S(ReturnType) Method () Modifier MF_GG_M_B(ReturnType, Method)
#define MF_GG_M_P1(ReturnType, Method, Type1, Modifier) MF_GG_M_S(ReturnType) Method (Type1 _a1) Modifier MF_GG_M_B(ReturnType, Method, _a1)
#define MF_GG_M_P2(ReturnType, Method, Type1, Type2, Modifier) MF_GG_M_S(ReturnType) Method (Type1 _a1, Type2 _a2) Modifier MF_GG_M_B(ReturnType, Method, _a1, _a2)
#define MF_GG_M_P3(ReturnType, Method, Type1, Type2, Type3, Modifier) MF_GG_M_S(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3) Modifier MF_GG_M_B(ReturnType, Method, _a1, _a2, _a3)
#define MF_GG_M_P4(ReturnType, Method, Type1, Type2, Type3, Type4, Modifier) MF_GG_M_S(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3, Type4 _a4) Modifier MF_GG_M_B(ReturnType, Method, _a1, _a2, _a3, _a4)
#define MF_GG_M_P5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Modifier) MF_GG_M_S(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3, Type4 _a4, Type4 _a5) Modifier MF_GG_M_B(ReturnType, Method, _a1, _a2, _a3, _a4, _a5)
#define MF_GG_M_P6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6, Modifier) MF_GG_M_S(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3, Type4 _a4, Type4 _a5, Type4 _a6) Modifier MF_GG_M_B(ReturnType, Method, _a1, _a2, _a3, _a4, _a5, _a6)

#define MF_GG_M_PM0(ReturnType, Method) MF_GG_M_P0(ReturnType, Method, )
#define MF_GG_M_PM1(ReturnType, Method, Type1) MF_GG_M_P1(ReturnType, Method, Type1, )
#define MF_GG_M_PM2(ReturnType, Method, Type1, Type2) MF_GG_M_P2(ReturnType, Method, Type1, Type2, )
#define MF_GG_M_PM3(ReturnType, Method, Type1, Type2, Type3) MF_GG_M_P3(ReturnType, Method, Type1, Type2, Type3, )
#define MF_GG_M_PM4(ReturnType, Method, Type1, Type2, Type3, Type4) MF_GG_M_P4(ReturnType, Method, Type1, Type2, Type3, Type4, )
#define MF_GG_M_PM5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5) MF_GG_M_P5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, )
#define MF_GG_M_PM6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6) MF_GG_M_P6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6, ) 

#define MF_GG_M_PC0(ReturnType, Method) MF_GG_M_P0(ReturnType, Method, const)
#define MF_GG_M_PC1(ReturnType, Method, Type1) MF_GG_M_P1(ReturnType, Method, Type1, const)
#define MF_GG_M_PC2(ReturnType, Method, Type1, Type2) MF_GG_M_P2(ReturnType, Method, Type1, Type2, const)
#define MF_GG_M_PC3(ReturnType, Method, Type1, Type2, Type3) MF_GG_M_P3(ReturnType, Method, Type1, Type2, Type3, const)
#define MF_GG_M_PC4(ReturnType, Method, Type1, Type2, Type3, Type4) MF_GG_M_P4(ReturnType, Method, Type1, Type2, Type3, Type4, const)
#define MF_GG_M_PC5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5) MF_GG_M_P5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, const)
#define MF_GG_M_PC6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6) MF_GG_M_P6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6, const) 

#define MF_GG_M_PMC0(ReturnType, Method) MF_GG_M_PC0(const ReturnType, Method) MF_GG_M_PM0(ReturnType, Method)
#define MF_GG_M_PMC1(ReturnType, Method, Type1) MF_GG_M_PC1(const ReturnType, Method, const Type1) MF_GG_M_PM1(ReturnType, Method, Type1)
#define MF_GG_M_PMC2(ReturnType, Method, Type1, Type2) MF_GG_M_PC2(const ReturnType, Method, const Type1, const Type2) MF_GG_M_PM2(ReturnType, Method, Type1, Type2)
#define MF_GG_M_PMC3(ReturnType, Method, Type1, Type2, Type3) MF_GG_M_PC3(const ReturnType, Method, const Type1, const Type2, const Type3) MF_GG_M_PM3(ReturnType, Method, Type1, Type2, Type3)
#define MF_GG_M_PMC4(ReturnType, Method, Type1, Type2, Type3, Type4) MF_GG_M_PC4(const ReturnType, Method, const Type1, const Type2, const Type3, const Type4) MF_GG_M_PM4(ReturnType, Method, Type1, Type2, Type3, Type4)
#define MF_GG_M_PMC5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5) MF_GG_M_PC5(const ReturnType, Method, const Type1, const Type2, const Type3, const Type4, const Type5) MF_GG_M_PM5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5)
#define MF_GG_M_PMC6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6) MF_GG_M_PC6(const ReturnType, Method, const Type1, const Type2, const Type3, const Type4, const Type5, const Type6) MF_GG_M_PM6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6)

#endif