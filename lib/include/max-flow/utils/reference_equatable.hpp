#ifndef INCLUDED_MAX_FLOW_REFERENCE_EQUATABLE
#define INCLUDED_MAX_FLOW_REFERENCE_EQUATABLE

#include <memory>

namespace MaxFlow::Utils
{

#pragma region Declaration

	class ReferenceEquatable
	{

	protected:

		// Construction

		ReferenceEquatable () = default;

	public:

		// Comparison

		bool operator==(const ReferenceEquatable& _other) const;

	};

#pragma endregion

#pragma region Implementation

	inline bool ReferenceEquatable::operator==(const ReferenceEquatable& _other) const
	{
		return this == std::addressof (_other);
	}

#pragma endregion

}

#endif