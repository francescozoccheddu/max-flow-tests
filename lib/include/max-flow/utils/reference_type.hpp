#ifndef INCLUDED_MAX_FLOW_UTILS_REFERENCE_TYPE
#define INCLUDED_MAX_FLOW_UTILS_REFERENCE_TYPE

#include <max-flow/utils/reference_equatable.hpp>

namespace MaxFlow::Utils
{

	class ReferenceType : public ReferenceEquatable
	{

		// Deleted value operations

		ReferenceType (const ReferenceType&) = delete;
		ReferenceType (ReferenceType&&) = delete;
		ReferenceType& operator=  (const ReferenceType&) = delete;
		ReferenceType& operator= (ReferenceType&&) = delete;

	protected:

		ReferenceType () = default;

	};

}

#endif